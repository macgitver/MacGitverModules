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

#include "IndexDelegate.h"

#include <QAbstractItemView>
#include <QPainter>
#include <QToolTip>
#include <QHelpEvent>

#include "libGitWrap/ObjectId.hpp"
#include "libGitWrap/Result.hpp"

IndexViewDelegate::IndexViewDelegate( QObject* parent )
    : QItemDelegate( parent )
{
}

void IndexViewDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option,
                                    const QModelIndex& index ) const
{
    if( index.column() != 0 )
    {
        QItemDelegate::paint( painter, option, index );
        return;
    }

    drawBackground( painter, option, index );

    doDrawDisplay( painter, option, option.rect.adjusted( 1, 1, -1, -1 ), index );

    QIcon theDecoration( index.data(Qt::DecorationRole).value<QIcon>() );
    QRect decorationRect(QPoint(0, 0), option.decorationSize);
    if ( option.decorationAlignment.testFlag(Qt::AlignVCenter) )
    {
        QPoint newCenter( decorationRect.center().x(), option.rect.center().y() );
        decorationRect.moveCenter( newCenter );
    }
    drawDecoration( painter, option, decorationRect, theDecoration.pixmap(decorationRect.size()) );

    drawFocus( painter, option, option.rect );
}

void IndexViewDelegate::doDrawDisplay(QPainter *painter, const QStyleOptionViewItem &option,
                                           const QRect &rect, const QModelIndex &index) const
{
//    const QFontMetrics& fm = option.fontMetrics;

//    QRect textRect = option.rect;
//    textRect.moveLeft( option.decorationSize.width() );
//    textRect.setBottom( textRect.top() + fm.lineSpacing() );

//    QVariant submoduleData = index.data(Qt::UserRole + 1);
//    if ( submoduleData.canConvert<Git::Submodule>() )
//    {
//        Git::Submodule submodule = index.data(Qt::UserRole + 1).value<Git::Submodule>();

//        QFont f( option.font );
//        f.setBold( true );
//        painter->setFont( f );
//        painter->drawText( textRect, trUtf8("Name: %1").arg(submodule.name()) );

//        textRect.moveTop( textRect.top() + fm.lineSpacing() );
//        painter->setFont( option.font );
//        painter->drawText( textRect, trUtf8("Revision: %1").arg(submodule.wdOid().toString()) );
//    }
}

bool IndexViewDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *view,
                                       const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if ( !event || !view )
        return false;

//    QVariant submoduleData = index.data(Qt::UserRole + 1);
//    if ( !submoduleData.canConvert<Git::Submodule>() )
//        return false;

//    Git::Submodule submodule = index.data(Qt::UserRole + 1).value<Git::Submodule>();

//    if ( event->type() == QEvent::ToolTip )
//    {
//        Git::Result r;
//        QString tooltip =
//                trUtf8("URL: %1\nPath: %2")
//                .arg(submodule.url(r))
//                .arg(submodule.path(r));

//        QToolTip::showText( event->globalPos(), tooltip, view );

//        return true;
//    }

//    return QItemDelegate::helpEvent( event, view, option, index );
}

QSize IndexViewDelegate::sizeHint( const QStyleOptionViewItem& option,
                                        const QModelIndex& index ) const
{
    const QFontMetrics& fm = option.fontMetrics;

    return QSize( 200, 2 + 2 * fm.lineSpacing() );
}
