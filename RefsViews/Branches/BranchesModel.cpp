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

#include "BranchesModel.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/RepoMan/Branch.hpp"
#include "libMacGitverCore/RepoMan/CollectionNode.hpp"
#include "libMacGitverCore/RepoMan/RefTreeNode.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Tag.hpp"


BranchesModel::BranchesModel( BranchesViewData* parent )
    : QAbstractItemModel( parent )
    , mData( parent )
    , mRoot( new RefItem )
    , mHeaderLocal( NULL )
    , mHeaderRemote( NULL )
    , mHeaderTags( NULL )
{
    RM::RepoMan& rm = MacGitver::repoMan();
    connect( &rm, SIGNAL(refCreated(RM::Repo*,RM::Ref*)), this, SLOT(onRefCreated(RM::Repo*,RM::Ref*)) );
    connect( &rm, SIGNAL(refAboutToBeDeleted(RM::Repo*,RM::Ref*)), this, SLOT(onRefDestroyed(RM::Repo*,RM::Ref*)) );
    connect( &rm, SIGNAL(refMoved(RM::Repo*,RM::Ref*)), this, SLOT(onRefMoved(RM::Repo*,RM::Ref*)) );
}

BranchesModel::~BranchesModel()
{
    delete mRoot;
}

int BranchesModel::rowCount( const QModelIndex& parent ) const
{
    RefItem* parentItem;
    if( parent.column() > 0 )
    {
        return 0;
    }

    if( !parent.isValid() )
        parentItem = mRoot;
    else
        parentItem = static_cast< RefItem* >( parent.internalPointer() );

    return parentItem->children.count();
}

int BranchesModel::columnCount( const QModelIndex& parent ) const
{
    return 1;
}

QVariant BranchesModel::data( const QModelIndex& index, int role ) const
{
    if( !index.isValid() )
    {
        return QVariant();
    }

    RefItem* item = static_cast< RefItem* >( index.internalPointer() );
    return item->data( index.column(), role );
}

Qt::ItemFlags BranchesModel::flags( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return Qt::NoItemFlags;

    Qt::ItemFlags result = Qt::ItemIsEnabled;
    const RefItem *item = static_cast<const RefItem *>( index.internalPointer() );

    RefItem::ItemType t = static_cast<RefItem::ItemType>(
                item->data( 0, RefItem::TypeRole ).toInt() );
    if ( (t == RefItem::Reference) || (t == RefItem::Namespace) )
        result |= Qt::ItemIsSelectable;

    return result;
}

QModelIndex BranchesModel::index( int row, int column, const QModelIndex& parent ) const
{
    if( !hasIndex( row, column, parent ) )
    {
        return QModelIndex();
    }

    RefItem* parentItem;

    if( !parent.isValid() )
        parentItem = mRoot;
    else
        parentItem = static_cast< RefItem* >( parent.internalPointer() );

    RefItem* childItem = parentItem->children.at( row );
    if( childItem )
        return createIndex( row, column, childItem );
    else
        return QModelIndex();
}

QModelIndex BranchesModel::parent( const QModelIndex& child ) const
{
    if( !child.isValid() )
    {
        return QModelIndex();
    }

    RefItem* childItem = static_cast< RefItem* >( child.internalPointer() );
    RefItem* parentItem = childItem->parent;

    if( parentItem == mRoot )
        return QModelIndex();

    int row = parentItem->parent->children.indexOf( parentItem );
    return createIndex( row, 0, parentItem );
}

bool BranchesModel::hasChildren( const QModelIndex& parent ) const
{
    RefItem* parentItem;
    if( parent.column() > 0 )
    {
        return 0;
    }

    if( !parent.isValid() )
        parentItem = mRoot;
    else
        parentItem = static_cast< RefItem* >( parent.internalPointer() );

    return parentItem->children.count() > 0;
}

void BranchesModel::insertRef(bool notify, const RM::Ref *ref)
{
    Git::RefName analyzer( ref->fullName() );
    RefScope* scope = scopeForRef( analyzer );
    QStringList namespaces = analyzer.isNamespaced()
                             ? analyzer.namespaces()
                             : analyzer.shorthand().split( QChar( L'/' ) );
    if ( !analyzer.isNamespaced() ) {
        // remove the branch name
        namespaces.removeLast();
    }

    RefItem* ns = scope;
    foreach( QString nsName, namespaces )
    {
        RefItem* next = NULL;
        foreach( RefItem* nsChild, ns->children )
        {
            if( nsChild->text() == nsName )
            {
                next = nsChild;
                break;
            }
        }

        if( !next )
        {
            next = insertNamespace( notify, ns, nsName );
        }
        ns = next;
    }

    insertBranch( notify, ns, ref );
}

void BranchesModel::insertRefs(bool notify, const RM::CollectionNode* cn)
{
    foreach ( RM::Ref* ref, cn->childObjects<RM::Ref>() ) {
        insertRef( notify, ref );
    }

    foreach ( RM::RefTreeNode* rtn, cn->childObjects<RM::RefTreeNode>() ) {
        insertRefs( notify, rtn );
    }
}

void BranchesModel::insertRefs(bool notify, const RM::RefTreeNode* ns)
{
    foreach (RM::Ref* b, ns->childObjects<RM::Ref>() ) {
        insertRef( notify, b );
    }

    // recurse into sub-nodes
    foreach ( RM::RefTreeNode* sub, ns->childObjects<RM::RefTreeNode>() ) {
        insertRefs( notify, sub );
    }
}

void BranchesModel::rereadBranches()
{
    beginResetModel();

    qDeleteAll( mRoot->children );
    mRoot->children.clear();

    mHeaderLocal    = new RefScope( mRoot, tr( "Local" ) );
    mHeaderRemote   = new RefScope( mRoot, tr( "Remote" ) );
    mHeaderTags     = new RefScope( mRoot, tr( "Tags" ) );

    RM::Repo* repo = mData->repository();
    if( repo )
    {
        insertRefs( false, repo->branches() );
        insertRefs( false, repo->tags() );
    }

    endResetModel();
}

/**
 * @internal
 *
 * @brief   Workaround: temporary method to recursively destroy invalid tree items
 *
 * @param   item    the current item to check
 *
 * @param   ref     when given, the ref name will be compared with the RefItem
 */
void BranchesModel::findInvalidRefItems(QVector<RefItem*>& invalidItems, RefItem* item, const RM::Ref* ref )
{
    if ( !item->isValid() || item->sameReference( ref ) )
    {
        invalidItems << item;
        return;
    }

    for ( int i = item->children.count() - 1; i > -1 ; i-- ) {
        findInvalidRefItems( invalidItems, item->children[i], ref );
    }
}

/**
 * @internal
 * @see RM::EventInterface
 */
///@{
void BranchesModel::onRefCreated(RM::Repo* repo, RM::Ref* ref)
{
    if ( repo != mData->repository() ) {
        return;
    }

    insertRef( true, ref );
}

void BranchesModel::onRefDestroyed(RM::Repo* repo, RM::Ref* ref)
{
    if ( repo != mData->repository() ) {
        return;
    }

    // TODO: This is an ugly workaround to find a matching RefItem!
    // We simply recursively search for invalid objects and delete them.
    QVector<RefItem*> invalidItems;
    findInvalidRefItems( invalidItems, mRoot, ref );

    while ( !invalidItems.isEmpty() ) {
        RefItem* ri = invalidItems.takeFirst();
        QModelIndex idx = index( ri );
        beginRemoveRows( idx.parent(), idx.row(), idx.row() );
        // RefItem unlinks itself from its parent
        delete ri;
        endRemoveRows();
    }
}

void BranchesModel::onRefMoved(RM::Repo* repo, RM::Ref* ref)
{
    Q_UNUSED( ref )

    if ( repo != mData->repository() ) {
        return;
    }

    // TODO: scan for changes in RefItems instead of performing a full update.
    QVector<int> updateRoles;
    updateRoles << Qt::DisplayRole << Qt::BackgroundRole
                << Qt::FontRole << Qt::DecorationRole;
    emit dataChanged( index(0, 0), index( rowCount( QModelIndex() ) - 1, 0 ), updateRoles );
}
///@}

QModelIndex BranchesModel::index(RefItem* item) const
{
    if ( !item || (item == mRoot) )
    {
        return QModelIndex();
    }

    RefItem* parent = item->parent ? item->parent : mRoot;
    int row = parent->children.indexOf( item );
    return createIndex( row, 0, item );
}
