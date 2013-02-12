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


IndexDlg::IndexDlg(QWidget *parent)
{
    setupUi( this );

    setWindowTitle( trUtf8( "Stage" ) );
}
