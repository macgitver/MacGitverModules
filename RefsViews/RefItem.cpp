/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Nils Fenner <nilsfenner@web.de>
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

#include "RefItem.hpp"

#include "libGitWrap/Result.hpp"

#include "libMacGitverCore/RepoMan/Ref.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"

#include <QFont>
#include <QFileIconProvider>

RefItem::RefItem()
    : parent( NULL )
{
}

RefItem::RefItem(RefItem *p)
    : parent( p )
{
    Q_ASSERT( p );
    p->children.append( this );
}

RefItem::~RefItem()
{
    if( parent )
    {
        parent->children.removeOne( this );
    }
    qDeleteAll( children );
}

/**
 * @brief   Checks the validity of internal data.
 *
 * @return  the default implementation returns always true
 */
bool RefItem::isValid() const
{
    return true;
}

QVariant RefItem::data(int col, int role) const
{
    Q_UNUSED( col )
    Q_UNUSED( role )
    return QVariant();
}


QString RefItem::text() const
{
    return QString();
}


RefScope::RefScope(RefItem *p, const QString &t)
    : RefItem( p )
    , mText ( t )
{
}

QVariant RefScope::data(int col, int role) const
{
    switch( role )
    {
    case Qt::DisplayRole:
        return mText;

    case RefItem::RowBgRole:
        return QColor(216, 233, 255);
    }

    if ( role == RefItem::TypeRole )
        return RefItem::Scope;

    return QVariant();
}

QString RefScope::text() const
{
    return mText;
}


RefNameSpace::RefNameSpace(RefItem *p, const QString &t)
    : RefScope( p, t )
{
}

QVariant RefNameSpace::data(int col, int role) const
{
    switch( role )
    {
    case Qt::DisplayRole:
        return mText;

    case Qt::DecorationRole:
        return QFileIconProvider().icon( QFileIconProvider::Folder );
    }

    if ( role == RefItem::TypeRole )
        return RefItem::Namespace;

    return QVariant();
}

RefBranch::RefBranch(RefItem* p, const RM::Ref* refInfo)
    : RefItem( p )
    , mRefInfo( refInfo )
{
    Q_ASSERT( refInfo );
}

Git::Reference RefBranch::lookupGitReference(const RefBranch* item)
{
    if ( !item ) {
        return Git::Reference();
    }

    const RM::Ref* ref = item->referenceInfo();
    if ( !ref ) {
        return Git::Reference();
    }

    Git::Result r;
    return ref->repository()->gitLoadedRepo().reference( r, ref->fullName() );
}

/**
 * @brief   Am I pointing to a valid Git::Reference object?
 *
 * @return  true, if the owned reference is valid; false otherwise
 */
bool RefBranch::isValid() const
{
    Git::Result r;
    Git::Reference gref = lookupGitReference( this );
    return (gref.isValid() && !gref.wasDestroyed());
}

/**
 * @brief   Workaround to compare the reference name.
 *
 *          This method will be deleted when migrating to RM::RepoMan.
 *
 * @param   ref the RM::Ref to compare with
 *
 * @return  true when both names match; false otherwise
 */
bool RefBranch::sameReference(const RM::Ref* ref) const
{
    return ref == mRefInfo || mRefInfo->fullName() == ref->fullName();
}

QVariant RefBranch::data(int col, int role) const
{
    if ( role == Qt::DisplayRole )
        return mRefInfo->displayName();

    else if ( role == Qt::FontRole )
    {
        Git::Result r;
        const Git::Reference headRef = lookupHEAD( r, mRefInfo );
        if( headRef.target() == mRefInfo->fullName() )
        {
            QFont f;
            f.setBold( true );
            return f;
        }
    }

    else if ( role == RefItem::RowBgGradientRole )
    {
        Git::Result r;
        const Git::Reference headRef = lookupHEAD( r, mRefInfo );
        if ( headRef.resolveToObjectId( r ) == mRefInfo->id() )
        {
            const QColor c = QColor::fromHsl(35, 255, 190);
            return ( headRef.target() == mRefInfo->fullName() ) ? c : c.lighter(130);
        }
    }

    else if ( role == RefItem::TypeRole )
        return RefItem::Reference;

    else if ( role == Qt::EditRole)
        return mRefInfo->name();

    return QVariant();
}

const RM::Ref*RefBranch::referenceInfo() const
{
    return mRefInfo;
}

Git::Reference RefBranch::lookupHEAD(Git::Result& result, const RM::Ref* ref)
{
    const RM::Repo* repo = ref->repository();
    return repo->gitLoadedRepo().reference( result, QStringLiteral("HEAD") );
}
