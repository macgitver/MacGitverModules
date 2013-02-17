/*
 * MacGitver: Index Module
 * Copyright (C) 2013 Nils Fenner <nilsfenner@web.de>
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

#include "IndexView.h"

#include <QVBoxLayout>
#include <QToolBar>

#include "libMacGitverCore/App/MacGitver.hpp"

#include "libHeaven/Widgets/MiniSplitter.h"

#include "libGitWrap/Result.hpp"
#include "libGitWrap/Repository.hpp"
#include "libGitWrap/DiffList.hpp"
#include "libGitWrap/ChangeListConsumer.hpp"


IndexView::IndexView()
    : GlobalView( QLatin1String( "Index" ) )
    , ConfigUser( "History" )
    , mVertSplit( new Heaven::MiniSplitter( Qt::Vertical ) )
    , mHorzSplit( new Heaven::MiniSplitter( Qt::Horizontal ) )
{
    setViewName( trUtf8("Stage") );
    setupActions( this );

    mDlgCommit.layout()->setMargin( 0 );

    QVBoxLayout* l = new QVBoxLayout;
    l->setSpacing( 0 );
    l->setMargin( 0 );

    l->addWidget( tbIndexViewToolbar->toolBarFor( this ) );

    setLayout( l );
    initSplitters();

    mListUnstaged.setModel( &mUnstagedModel );

    connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
             this, SLOT(repositoryChanged(Git::Repository)) );

    repositoryChanged( MacGitver::self().repository() );
}

void IndexView::repositoryChanged( Git::Repository repo )
{
    if( !repo.isValid() )
    {
        return;
    }

    Git::Result r;
    Git::DiffList diffIndex = repo.diffIndexToWorkingDir(r);

    foreach( const Git::ChangeListEntry &c, diffIndex.changeList(r) )
    {
        QStandardItem * it = new QStandardItem( c.newPath );
        it->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

        mUnstagedModel.appendRow( it );
    }
}

void IndexView::initSplitters()
{
    quint32 i = configGet( "SplitLayout", 0 );
    switch( i )
    {
    default:
    case 0:
        mHorzSplit->addWidget( &mListUnstaged );
        mHorzSplit->addWidget( &mDlgCommit );
        mHorzSplit->addWidget( &mListStaged );

        mVertSplit->addWidget( &mListDiff );
        mVertSplit->addWidget( mHorzSplit );

        layout()->addWidget( mVertSplit );
        break;
    case 1:
        mHorzSplit->addWidget( &mListUnstaged );
        mHorzSplit->addWidget( &mDlgCommit );
        mHorzSplit->addWidget( &mListStaged );

        mVertSplit->addWidget( mHorzSplit );
        mVertSplit->addWidget( &mListDiff );

        layout()->addWidget( mVertSplit );
        break;
    }
}
