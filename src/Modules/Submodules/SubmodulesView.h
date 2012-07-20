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

#ifndef MGV_SUBMODULES_VIEW_H
#define MGV_SUBMODULES_VIEW_H

#include "Heaven/Views/GlobalView.h"

#include "hic_SubmodulesViewActions.h"

class QTreeView;

class SubmodulesView : public Heaven::GlobalView, private SubmodulesViewActions
{
	Q_OBJECT
public:
	SubmodulesView();

private:
	QTreeView*			mTree;
};

#endif

