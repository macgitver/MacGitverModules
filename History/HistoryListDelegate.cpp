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

#include <QPainter>

#include "HistoryModel.h"
#include "HistoryListDelegate.h"

void HistoryListDelegate::paintGraphLane( QPainter* p, GraphGlyphs glyph, GraphGlyphs lastGlyph,
                                          int x1, int x2, int height, const QColor& col,
                                          const QColor& activeCol, const QBrush& back ) const
{
    const int padding = 2;
    x1 += padding;
    x2 += padding;

    int h = height / 2;
    int m = (x1 + x2) / 2;
        int r = (x2 - x1) * 1 / 3;
    int d =  2 * r;

    #define P_CENTER m , h
    #define P_0      x2, h      // >
    #define P_90     m , 0      // ^
    #define P_180    x1, h      // <
    #define P_270    m , height  // v
    #define DELTA_UR 2*(x1 - m), height ,   0*16, 90*16  // -,
    #define DELTA_DR 2*(x1 - m), -height, 270*16, 90*16  // -'
    #define DELTA_UL 2*(x2 - m), height ,  90*16, 90*16  //  ,-
    #define DELTA_DL 2*(x2 - m), -height, 180*16, 90*16  //  '-
    #define CENTER_UR x1, height, 225
    #define CENTER_DR x1, 0  , 135
    #define CENTER_UL x2, height, 315
    #define CENTER_DL x2, 0  ,  45
    #define R_CENTER m - r, h - r, d, d

    QPen lanePen( Qt::black, 2 );

    // arc
    switch( glyph )
    {
    case ggJoin:
    case ggJoinRight:
    case ggHead:
    case ggHeadRight:
    {
        QConicalGradient gradient(CENTER_UR);
        gradient.setColorAt(0.375, col);
        gradient.setColorAt(0.625, activeCol);
        lanePen.setBrush(gradient);
        p->setPen(lanePen);
        p->drawArc(P_CENTER, DELTA_UR);
        break;
    }
    case ggJoinLeft:
    {
        QConicalGradient gradient(CENTER_UL);
        gradient.setColorAt(0.375, activeCol);
        gradient.setColorAt(0.625, col);
                lanePen.setBrush(gradient);
                p->setPen(lanePen);
        p->drawArc(P_CENTER, DELTA_UL);
        break;
    }
    case ggTail:
    case ggTailRight:
    {
        QConicalGradient gradient(CENTER_DR);
        gradient.setColorAt(0.375, activeCol);
        gradient.setColorAt(0.625, col);
                lanePen.setBrush(gradient);
                p->setPen(lanePen);
        p->drawArc(P_CENTER, DELTA_DR);
        break;
    }
    default:
        break;
    }

    lanePen.setColor(col);
    p->setPen(lanePen);

    // vertical line
    switch( glyph )
    {
    case ggCurrent:
    case ggNotCurrent:
    case ggMergeFork:
    case ggMergeForkRight:
    case ggMergeForkLeft:
    case ggJoin:
    case ggJoinRight:
    case ggJoinLeft:
    case ggCross:
        p->drawLine(P_90, P_270);
        break;
    case ggHeadLeft:
    case ggBranch:
        p->drawLine(P_CENTER, P_270);
        break;
    case ggTailLeft:
    case ggInitial:
        p->drawLine(P_90, P_CENTER);
        break;
    default:
        break;
    }

    lanePen.setColor(activeCol);
    p->setPen(lanePen);

    // horizontal line
    switch( glyph )
    {
    case ggMergeFork:
    case ggJoin:
    case ggHead:
    case ggTail:
    case ggCross:
    case ggCrossUnused:
        p->drawLine(P_180, P_0);
        break;
    case ggMergeForkRight:
        p->drawLine(P_180, P_CENTER);
        break;
    case ggMergeForkLeft:
    case ggHeadLeft:
    case ggTailLeft:
        p->drawLine(P_CENTER, P_0);
    default:
        break;
    }

    // horizontal line, if continued
    switch( glyph )
    {
    case ggJoin:
    case ggJoinRight:
    case ggHead:
    case ggHeadRight:
        switch( lastGlyph )
        {
        case ggMergeFork:
        case ggJoin:
        case ggHead:
        case ggTail:
        case ggCross:
        case ggCrossUnused:
        case ggMergeForkRight:
        case ggHeadRight:
        case ggTailRight:
            p->drawLine(P_180, P_0);
        default:
            break;
        }
    default:
        break;
    }

    QPen blackPen(col.darker());
    blackPen.setWidth(0);
    p->setPen(blackPen);
    // center symbol, e.g. rect or ellipse
    switch( glyph )
    {
    case ggCurrent:
    case ggInitial:
    case ggBranch:
        p->setBrush(col);
        p->drawEllipse(R_CENTER);
        break;
    case ggMergeFork:
    case ggMergeForkLeft:
    case ggMergeForkRight:
        p->setBrush(col);
        p->drawRect(R_CENTER);
        break;
    default:
        break;
    }
    #undef P_CENTER
    #undef P_0
    #undef P_90
    #undef P_180
    #undef P_270
    #undef DELTA_UR
    #undef DELTA_DR
    #undef DELTA_UL
    #undef DELTA_DL
    #undef CENTER_UR
    #undef CENTER_DR
    #undef CENTER_UL
    #undef CENTER_DL
    #undef R_CENTER
}

void HistoryListDelegate::paintGraph( QPainter* p, const QStyleOptionViewItem& opt,
                                      const QModelIndex& i ) const
{
    const HistoryModel* m = qobject_cast< const HistoryModel* >( i.model() );
    HistoryEntry* e = m->at( i.row() );

    if( !e )
    {
        // If we're still required to populate that entry, don't do anyhting here
        return;
    }

    p->save();
    p->setRenderHint( QPainter::Antialiasing );
    p->setClipRect(opt.rect, Qt::IntersectClip);
    p->translate(opt.rect.topLeft());

    QBrush back = opt.palette.base();
    GraphGlyphVector lanes = e->glyphs();
    uint laneNum = lanes.count();
    uint activeLane = 0;
    for (uint i = 0; i < laneNum; i++)
        if (lanes[i] == ggCurrent || lanes[i] == ggInitial || lanes[i] == ggBranch ||
                lanes[i] == ggMergeFork || lanes[i] == ggMergeForkLeft || lanes[i] == ggMergeForkRight )
        {
            activeLane = i;
            break;
        }

    int x1 = 0, x2 = 0;
    int height = opt.rect.height();
    int maxWidth = opt.rect.width();
    int lw = 3 * 18 / 4;
    QColor activeColor = laneColor( activeLane );
//	if (opt.state & QStyle::State_Selected)
//		activeColor = blend(activeColor, opt.palette.highlightedText().color(), 208);
    for (uint i = 0; i < laneNum && x2 < maxWidth; i++) {

        x1 = x2;
        x2 += lw;

        GraphGlyphs ln = lanes[i];
        if( ln != ggUnused )
        {
            QColor color = i == activeLane ? activeColor : laneColor( i );
            GraphGlyphs nextGlyph = ( i < laneNum - 1 ) ? lanes[ i + 1 ] : ggUnused;
            paintGraphLane(p, ln, nextGlyph, x1, x2, height, color, activeColor, back);
        }
    }
    p->restore();
}

QColor HistoryListDelegate::laneColor(int lane)
{
    static const QColor laneColors[] =
    {
        QColor::fromHsl( 200, 255, 128 ), // blue
        QColor::fromHsl( 280, 255, 128 ), // violett
        QColor::fromHsl(   8, 255, 128 ), // red
        QColor::fromHsl(  56, 255, 128 ), // yellow
        QColor::fromHsl( 133, 255, 128 ), // green

        QColor::fromHsl(  93, 255, 128 ), // green
        QColor::fromHsl(  30, 255, 128 ), // orange
        QColor::fromHsl(  44, 255, 128 ), // yellow
        QColor::fromHsl( 226, 255, 128 ), // blue
        QColor::fromHsl(  69, 255, 128 ), // yellow-green

        QColor::fromHsl( 273, 255, 128 ), // violett
        QColor::fromHsl( 135, 255, 128 )  // green
    };

    return laneColors[lane % 12];
}

void HistoryListDelegate::paintMessage( QPainter* p, const QStyleOptionViewItem& opt,
                                        const QModelIndex& i ) const
{
    const HistoryModel* m = qobject_cast< const HistoryModel* >( i.model() );
    HistoryEntry* e = m->at( i.row() );

    if( !e )
    {
        // If we're still required to populate that entry, don't do anyhting here
        return;
    }

    QRect r = opt.rect;
    p->save();
    p->setClipRect( opt.rect );

    HistoryInlineRefs refs = e->refs();
    if( refs.count() )
    {
        for( int refIdx = 0; refIdx < refs.count(); refIdx++ )
        {
            const HistoryInlineRef& ref = refs.at( refIdx );
            int w = opt.fontMetrics.width( ref.mRefName ) + 6;
            p->setFont( opt.font );

            QRect refRect( r.left(), r.top() + 1, w, r.height() - 3 );

            QColor back = QColor( 0xD9D9D9 );
            if( ref.mIsTag )
            {
                back = Qt::yellow;
            }
            else if( ref.mIsBranch )
            {
                if ( ref.mIsRemote )
                    back = QColor( 0xB8E4FF );
                else if ( ref.mIsCurrent )
                    back = QColor( 0xFFB54F );
                else
                    back = QColor( 0x9BFD51 );
            }

            QLinearGradient gradient( refRect.left(), 0, refRect.right(), 0 );
            gradient.setColorAt( 0.0, back.lighter(125) );
            gradient.setColorAt( 0.33, back );

            p->setBrush( gradient );
            p->setPen( QPen(Qt::transparent) );
            p->drawRoundedRect( refRect, 2.5, 2.5 );

            refRect.adjust( 2, 0, -2, 0 );
            p->setPen( QPen(Qt::black) );
            p->drawText( refRect, Qt::AlignCenter, ref.mRefName );

            r.setLeft( r.left() + w + 3 );
        }
    }

    p->restore();

    // draw text in original color
    QStyleOptionViewItem newOpt(opt);
    newOpt.rect = r;
    newOpt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    QItemDelegate::paint( p, newOpt, i );
}

void HistoryListDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option,
                                 const QModelIndex& index ) const
{
    if( option.state & QStyle::State_Selected )
        painter->fillRect( option.rect, option.palette.highlight() );
    else if( index.row() & 1 )
        painter->fillRect( option.rect, option.palette.alternateBase() );
    else
        painter->fillRect( option.rect, option.palette.base() );

    if( index.column() > 1 )
    {
        QItemDelegate::paint( painter, option, index );
        return;
    }

    drawBackground( painter, option, index );

    if( index.column() == 0 )
    {
        paintGraph( painter, option, index );
        return;
    }

    if( index.column() == 1 )
    {
        paintMessage( painter, option, index );
    }
}


QSize HistoryListDelegate::sizeHint( const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const
{
    QSize s = QItemDelegate::sizeHint( option, index );

    if( index.column() == 1 )
    {
        const HistoryModel* m = qobject_cast< const HistoryModel* >( index.model() );
        HistoryEntry* e = m->at( index.row() );

        // e might be NULL, if it's not populated yet. In this case there is no sense in calculating
        // the size anyhow; assume defaults.
        if( e )
        {
            HistoryInlineRefs refs = e->refs();
            int totalWidth = 0;

            if( refs.count() )
            {
                for( int refIdx = 0; refIdx < refs.count(); refIdx++ )
                {
                    const HistoryInlineRef& ref = refs.at( refIdx );
                    int w;
                    if( ref.mIsCurrent )
                    {
                        QFont f = option.font;
                        f.setBold( true );
                        w = QFontMetrics( f ).width( ref.mRefName ) + 6;

                        s.rheight() += 2;
                    }
                    else
                    {
                        w = option.fontMetrics.width( ref.mRefName ) + 6;
                    }

                    totalWidth += w + 3;
                }
                totalWidth += 3;
            }

            QString txt = index.data().toString();
            s.rwidth() = option.fontMetrics.width( txt ) + totalWidth;
            s.rheight() += 2;
        }
    }

    return s;
}
