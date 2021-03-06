/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#include "libGitWrap/Result.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"

#include "HistoryEntry.h"

HistoryEntry::HistoryEntry( const Git::ObjectId& sha1 )
    : mSha1( sha1 )
    , mPopulated( false )
    , mPopulationQueued(false)
{
}

bool HistoryEntry::isPopulationQueued() const
{
    return mPopulationQueued;
}

void HistoryEntry::setPopulationQueued()
{
    Q_ASSERT(!mPopulated);
    mPopulationQueued = true;
}

void HistoryEntry::populate( const Git::Commit &commit )
{
    Q_ASSERT( !mPopulated );

    Git::Result r;

    mCommiter = commit.committer();
    mAuthor = commit.author();
    mCommitMessage = commit.shortMessage();

    mPopulationQueued = false;
    mPopulated = r;

    if( !mPopulated )
    {
        MacGitver::log(Log::Error, r, "Populating a HistoryEntry");
    }
}

bool HistoryEntry::isPopulated() const
{
    return mPopulated;
}

QString HistoryEntry::message() const
{
    Q_ASSERT( mPopulated );
    return mCommitMessage;
}

const Git::Signature& HistoryEntry::committer() const
{
    Q_ASSERT( mPopulated );
    return mCommiter;
}

const Git::Signature& HistoryEntry::author() const
{
    Q_ASSERT( mPopulated );
    return mAuthor;
}

const Git::ObjectId& HistoryEntry::id() const
{
    return mSha1;
}

void HistoryEntry::setGlyphs( const GraphGlyphVector& glyphs )
{
    mGlyphs = glyphs;
}

const GraphGlyphVector&	HistoryEntry::glyphs() const
{
    return mGlyphs;
}

void HistoryEntry::setInlineRefs( const HistoryInlineRefs& refs )
{
    mRefs = refs;
}

const HistoryInlineRefs& HistoryEntry::refs() const
{
    return mRefs;
}
