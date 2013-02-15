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

#include "IndexDlg.h"

#include "IndexDelegate.h"

#include "libGitWrap/ObjectId.hpp"
#include "libGitWrap/Result.hpp"
#include "libGitWrap/Repository.hpp"
#include "libGitWrap/DiffList.hpp"
#include "libGitWrap/Reference.hpp"
#include "libGitWrap/ObjectTree.hpp"


IndexDlg::IndexDlg(QWidget *parent)
{
    setupUi( this );

    setWindowTitle( trUtf8( "Stage" ) );

    listUnstaged->setModel( &mTestModel );
}

void IndexDlg::updateIndex( Git::Repository repo)
{
    Git::Result r;
    Git::DiffList diffIndex = repo.diffIndexToWorkingDir(r);

    foreach( const Git::ChangeListEntry &c, diffIndex.changeList(r))
    {
        QStandardItem * it = new QStandardItem( c.newPath );
        it->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

        mTestModel.appendRow( it );
    }
}
