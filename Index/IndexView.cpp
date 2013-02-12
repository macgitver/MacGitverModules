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

#include <QVBoxLayout>
#include <QToolBar>

#include "libMacGitverCore/App/MacGitver.hpp"

#include "IndexView.h"
#include "IndexDlg.h"


IndexView::IndexView()
    : GlobalView( QLatin1String( "Stage" ) )
    , mIndexView( new IndexDlg( this ) )
{
    setViewName( mIndexView->windowTitle() );
    setupActions( this );

    QVBoxLayout* l = new QVBoxLayout;
    l->setSpacing( 0 );
    l->setMargin( 0 );

    l->addWidget( tbIndexViewToolbar->toolBarFor( this ) );
    l->addWidget( mIndexView );

    setLayout( l );


    connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
             this, SLOT(repositoryChanged(Git::Repository)) );

    repositoryChanged( MacGitver::self().repository() );
}

void IndexView::repositoryChanged( Git::Repository repo )
{
    if( repo.isValid() )
    {
        mIndexView->updateIndex( repo );
    }
}
