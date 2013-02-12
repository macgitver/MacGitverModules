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

#ifndef MGV_INDEX_VIEW_H
#define MGV_INDEX_VIEW_H

#include "libHeaven/Views/GlobalView.hpp"

#include "libGitWrap/Repository.hpp"

#include "hic_IndexViewActions.h"

class IndexDlg;


class IndexView : public Heaven::GlobalView, private IndexViewActions
{
    Q_OBJECT
public:
    IndexView();

private slots:
    void repositoryChanged( Git::Repository repo );

private:
    IndexDlg *                          mIndexView;
};

#endif

