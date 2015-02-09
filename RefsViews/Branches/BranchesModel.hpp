/*
 * MacGitver
 * Copyright (C) 2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#ifndef MGV_BRANCHES_MODEL_HPP
#define MGV_BRANCHES_MODEL_HPP

#include <QAbstractItemModel>

#include "libMacGitverCore/RepoMan/Ref.hpp"

#include "Branches/BranchesViewData.hpp"
#include "RefItem.hpp"

namespace RM
{
    class Repo;
}

class RefItem;
class RefScope;

class BranchesModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    BranchesModel( BranchesViewData* parent );
    ~BranchesModel();

public:
    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    QModelIndex index( int row, int column = 0, const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& child ) const;
    bool hasChildren( const QModelIndex& parent ) const;

public:
    void rereadBranches();

signals:
    void gitError( const Git::Result& error );

private slots:
    void onRefCreated(RM::Repo* repo, RM::Ref* ref);
    void onRefDestroyed(RM::Repo* repo, RM::Ref* ref);
    void onRefMoved(RM::Repo* repo, RM::Ref* ref);

private:
    QModelIndex index(RefItem* item) const;

    void insertRef(bool notify, const RM::Ref* ref);

    inline RefItem* insertNamespace(const bool notify, RefItem* parent, const QString& name);
    inline void insertBranch(const bool notify, RefItem *parent, const RM::Ref* ref);
    inline RefScope* scopeForRef( const RM::Ref* ref ) const;

private:
    BranchesViewData*   mData;
    RefItem*            mRoot;

    RefScope*           mHeaderLocal;
    RefScope*           mHeaderRemote;
    RefScope*           mHeaderTags;

private:
    static void findInvalidRefItems(QVector<RefItem*>& invalidItems, RefItem* item, const RM::Ref* ref);
};


// -- INLINED PRIVATE METHODS BEGIN --8>

RefItem* BranchesModel::insertNamespace(const bool notify, RefItem* parent, const QString& name)
{
    RefItem* next = NULL;
    if ( notify ) {
        int fr = parent->children.count();
        beginInsertRows( index( parent ), fr, fr );
    }

    next = new RefNameSpace( parent, name );

    if ( notify ) {
        endInsertRows();
    }
    return next;
}

void BranchesModel::insertBranch(const bool notify, RefItem* parent, const RM::Ref* ref)
{
    if ( notify ) {
        int row = parent->children.count();
        beginInsertRows( index( parent ), row, row );
    }

    new RefBranch( parent, ref );

    if (notify) {
        endInsertRows();
    }
}

RefScope*BranchesModel::scopeForRef(const RM::Ref* ref) const
{
    RefItem* scope = NULL;
    if ( ref->type() == RM::BranchType )        scope = mHeaderLocal;
    // TODO: how to find the "remote branches"?
    //    else if ( ref->isA<RM::RemoteObject>() )    scope = mHeaderRemote;
    else scope = mHeaderTags;

    return static_cast< RefScope* >( scope );
}

// <8-- INLINED PRIVATE METHODS END --

#endif
