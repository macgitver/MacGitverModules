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

#ifndef MGV_INDEX_TREE_ITEM_VIEW_H
#define MGV_INDEX_TREE_ITEM_VIEW_H

#include "libMacGitverCore/Widgets/TreeViewCtxMenu.hpp"

#include "hic_StageViewCtxMenu.h"

class HeaderView;
class WorkingTreeModel;

class IndexTreeItemView : public TreeViewCtxMenu, private StageViewCtxMenu
{
    Q_OBJECT
public:
    IndexTreeItemView(QWidget *parent = 0);

public:
    void setModel(QAbstractItemModel *model);

private slots:
    // hid actions
    void onWtCtxUnstage();

    void contextMenu(const QModelIndex &index, const QPoint &globalPos);

private:
    QModelIndex deeplyMapToSource(QModelIndex current) const;

private:
    HeaderView*             mHeader;
    WorkingTreeModel*       mModel;
};

#endif // MGV_INDEX_TREE_ITEM_VIEW_H
