/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Cunz RaD Ltd.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QElapsedTimer>
#include <QStringBuilder>

#include "libMacGitverCore/App/MacGitver.hpp"

#include "libGitWrap/ObjectId.hpp"
#include "libGitWrap/Reference.hpp"
#include "libGitWrap/Result.hpp"

#include "HistoryBuilder.h"
#include "HistoryEntry.h"

HistoryBuilder::HistoryBuilder( Git::Repository repo, HistoryModel* model )
    : mRepo( repo )
    , mModel( model )
{
    Git::Result r;
    mWalker = Git::RevisionWalker::create(r, mRepo);
    mWalker.setSorting(r, true, false);
}

HistoryBuilder::~HistoryBuilder()
{
}

void HistoryBuilder::addHEAD()
{
    Git::Result r;
    mWalker.pushHead( r );
}

void HistoryBuilder::addAllRefs()
{
    Git::Result r;

    // In any case, put head at the top.
    mWalker.pushHead(r);

    QStringList sl = mRepo.allReferenceNames(r);
    foreach( QString s, sl )
    {
        mWalker.pushRef( r, s );
    }
}

void HistoryBuilder::addBranches(bool includeRemotes)
{
    Git::Result r;

    // In any case, put head at the top.
    mWalker.pushHead(r);

    QStringList sl = mRepo.branchNames(r, true, false);
    foreach (const QString& s, sl) {
        mWalker.pushRef(r, QStringLiteral("refs/heads/") % s);
    }

    if (includeRemotes) {
        sl = mRepo.branchNames(r, false, true);

        foreach (const QString& s, sl) {
            mWalker.pushRef(r, QStringLiteral("refs/remotes/") % s);
        }
    }
}

int HistoryBuilder::nextParent( const Git::ObjectId& sha1, int start )
{
    for( int i = start; i < mCurrentGlyphs.count(); i++ )
    {
        if( mNextParent.at( i ) == sha1 )
        {
            return i;
        }
    }

    return -1;
}

int HistoryBuilder::createGlyphSlot( GraphGlyphs glyph, const Git::ObjectId& nextSha1InSlot,
                                     int start )
{
    if( start < mCurrentGlyphs.count() )
    {
        while( start < mCurrentGlyphs.count() )
        {
            if( mCurrentGlyphs[ start ] == ggCrossUnused || mCurrentGlyphs[ start ] == ggUnused )
            {
                mCurrentGlyphs[ start ] = glyph;
                mNextParent[ start ] = nextSha1InSlot;
                return start;
            }
            start++;
        }
    }

    mCurrentGlyphs.append( glyph );
    mNextParent.append( nextSha1InSlot );

    return mCurrentGlyphs.count() - 1;
}



void HistoryBuilder::start()
{
    QVector< Git::ObjectId >	commits;
    Git::ObjectId				currentSHA1;
    Git::Commit                 curCommit;
    Git::Result					r;
    qint64						dur;
    double						avg;
    QElapsedTimer				timer;

    timer.start();
    commits = mWalker.all( r );

    dur = timer.nsecsElapsed();
    avg = double( dur ) / double( commits.count() );
    MacGitver::log(Log::Information,
                   trUtf8("Walked %1 commits in %2 ns = %3 ns per commit.")
                        .arg(commits.count())
                        .arg(dur)
                        .arg(avg, 10, 'f', 2));

    timer.restart();

    mModel->beforeClear();

    for( int curCommitIdx = 0; curCommitIdx < commits.count(); curCommitIdx++ )
    {
        mModel->append( new HistoryEntry( commits[ curCommitIdx ] ) );
    }

    mNextParent.clear();
    mCurrentGlyphs.clear();
    mCurrentLine = -1;

    for( int curCommitIdx = 0; curCommitIdx < commits.count(); curCommitIdx++ )
    {
        currentSHA1 = commits[ curCommitIdx ];
        if( mCurrentLine == -1 )
        {
            createGlyphSlot( ggBranch, currentSHA1 );
            mCurrentLine++;
        }
        curCommit = mRepo.lookupCommit( r, currentSHA1 );
        if( !r )
        {
            MacGitver::log(Log::Error, r, "Could not find a commit the RevWalker gave us.");
            break;
        }

        HistoryEntry* entry = mModel->at( curCommitIdx, false );

        int numParents = curCommit.numParentCommits();
        bool didFork = false;

        int nextLine = nextParent( currentSHA1 );
        if( nextLine != mCurrentLine )
        {
            GraphGlyphs& curGlyph = mCurrentGlyphs[ mCurrentLine ];

            if( curGlyph == ggInitial )
                curGlyph = ggUnused;
            else
                curGlyph = ggNotCurrent;

            if( nextLine != -1 )
                mCurrentGlyphs[ nextLine ] = ggCurrent;
            else
                nextLine = createGlyphSlot( ggBranch, currentSHA1, mCurrentLine );

            mCurrentLine = nextLine;
        }

        if( nextLine != -1 && nextParent( currentSHA1, nextLine + 1 ) != -1 )
        {
            didFork = true;
            int start, end, cur;
            start = end = cur = nextParent( currentSHA1 );

            while( cur != -1 )
            {
                end = cur;
                mCurrentGlyphs[ cur ] = ggTail;
                cur = nextParent( currentSHA1, cur + 1 );
            }

            mCurrentGlyphs[ mCurrentLine ] = ggMergeFork;

            GraphGlyphs& startGlyph = mCurrentGlyphs[ start ];
            GraphGlyphs& endGlyph = mCurrentGlyphs[ end ];

            if( startGlyph == ggMergeFork )
                startGlyph = ggMergeForkLeft;

            if( endGlyph == ggMergeFork )
                endGlyph = ggMergeForkRight;

            if( startGlyph == ggTail )
                startGlyph = ggTailLeft;

            if( endGlyph == ggTail )
                endGlyph = ggTailRight;

            for( int i = start + 1; i < end; i++ )
            {
                GraphGlyphs& gl = mCurrentGlyphs[ i ];

                if( gl == ggNotCurrent )
                    gl = ggCross;
                else if( gl == ggUnused )
                    gl = ggCrossUnused;
            }
        }

        if( numParents > 1 )
        {
            GraphGlyphs& gl = mCurrentGlyphs[ mCurrentLine ];

            bool wasFork	= gl == ggMergeFork;
            bool wasForkR	= gl == ggMergeForkRight;
            bool wasForkL	= gl == ggMergeForkLeft;
            bool startedCross = false, endedCross = false;

            gl = ggMergeFork;
            int start = mCurrentLine, end = mCurrentLine;

            for( int j = 1; j < numParents; j++ )
            {
                Git::ObjectId parentSha1 = curCommit.parentCommitId( r, j );
                int idx = nextParent( parentSha1 );
                if( idx != -1 )
                {
                    if( idx > end )
                    {
                        end = idx;
                        endedCross = mCurrentGlyphs[ idx ] == ggCross;
                    }
                    if( idx < start )
                    {
                        start = idx;
                        startedCross = mCurrentGlyphs[ idx ] == ggCross;
                    }

                    mCurrentGlyphs[ idx ] = ggJoin;
                }
                else
                {
                    end = createGlyphSlot( ggHead, parentSha1, end + 1 );
                }
            }

            GraphGlyphs& startGlyph = mCurrentGlyphs[ start ];
            GraphGlyphs& endGlyph = mCurrentGlyphs[ end ];

            if( startGlyph == ggMergeFork && !wasFork && !wasForkR )
                startGlyph = ggMergeForkLeft;

            if( endGlyph == ggMergeFork && !wasFork && !wasForkL )
                endGlyph = ggMergeForkRight;

            if( startGlyph == ggJoin && !startedCross )
                startGlyph = ggJoinLeft;

            if( endGlyph == ggJoin && !endedCross )
                endGlyph = ggJoinRight;

            if( startGlyph == ggHead )
                startGlyph = ggHeadLeft;

            if( endGlyph == ggHead )
                endGlyph = ggHeadRight;

            for( int i = start + 1; i < end; i++ )
            {
                GraphGlyphs& gl = mCurrentGlyphs[ i ];

                if( gl == ggNotCurrent )
                    gl = ggCross;
                else if( gl == ggUnused )
                    gl = ggCrossUnused;
                else if( gl == ggTailLeft || gl == ggTailRight )
                    gl = ggTail;
            }
        }

        if( numParents == 0 )
        {
            GraphGlyphs& gl = mCurrentGlyphs[ mCurrentLine ];
            if( gl != ggMergeFork && gl != ggMergeForkLeft && gl != ggMergeForkRight )
            {
                gl = ggInitial;
            }
        }

        entry->setGlyphs( mCurrentGlyphs );

        if( numParents == 0 )
        {
            mNextParent[ mCurrentLine ] = Git::ObjectId();
        }
        else
        {
            mNextParent[ mCurrentLine ] = curCommit.parentCommitId( r, 0 );
        }

        if( numParents > 1 )
        {
            for( int i = 0; i < mCurrentGlyphs.count(); i++ )
            {
                GraphGlyphs& gl = mCurrentGlyphs[ i ];
                if( gl == ggCross || gl == ggHead || gl == ggHeadLeft || gl == ggHeadRight ||
                    gl == ggJoin || gl == ggJoinRight || gl == ggJoinLeft )
                    gl = ggNotCurrent;
                else if( gl == ggCrossUnused )
                    gl = ggUnused;
                else if( gl == ggMergeFork || gl == ggMergeForkRight || gl == ggMergeForkLeft )
                    gl = ggCurrent;
            }
        }

        if( didFork )
        {
            for( int i = 0; i < mCurrentGlyphs.count(); i++ )
            {
                GraphGlyphs& gl = mCurrentGlyphs[ i ];
                if( gl == ggCross )
                    gl = ggNotCurrent;
                else if( gl == ggTail || gl == ggTailLeft || gl == ggTailRight || gl == ggCrossUnused )
                    gl = ggUnused;
                else if( gl == ggMergeFork || gl == ggMergeForkRight || gl == ggMergeForkLeft )
                    gl = ggCurrent;
            }

            while( mCurrentGlyphs.last() == ggUnused )
            {
                mCurrentGlyphs.pop_back();
                mNextParent.pop_back();
            }
        }

        if( mCurrentGlyphs[ mCurrentLine ] == ggBranch )
            mCurrentGlyphs[ mCurrentLine ] = ggCurrent;

    }

    mModel->afterClear();

    dur = timer.nsecsElapsed();
    avg = double(dur) / double(mModel->rowCount());

    MacGitver::log(Log::Information,
                   trUtf8("Glyphed %1 commits in %2 ns = %3 ns per Commit")
                        .arg(mModel->rowCount())
                        .arg(dur)
                        .arg(avg, 10, 'f', 2));
}
