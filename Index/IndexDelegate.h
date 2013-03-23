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

#ifndef INDEXVIEWDELEGATE_H
#define INDEXVIEWDELEGATE_H

#include <QItemDelegate>

/**
 * @brief The IndexViewDelegate class
 */
class IndexViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    IndexViewDelegate( QObject* parent );

public:
    void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;

    /**
     * @brief Internal function to draw the item.
     * @param painter
     * @param option
     * @param rect
     * @param index
     */
    void doDrawDisplay( QPainter* painter, const QStyleOptionViewItem& option, const QRect& rect,
                        const QModelIndex& index ) const;

public slots:
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);

};

#endif // INDEXVIEWDELEGATE_H
