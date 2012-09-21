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

#include <QStringBuilder>
#include <QFontMetrics>
#include <QPainter>
#include <QVBoxLayout>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QToolBar>
#include <QTreeView>
#include <QFileInfo>

#include "GitWrap/ObjectId.h"
#include "GitWrap/MetaTypes.h"

#include "MacGitver/MacGitver.h"
#include "MacGitver/FSWatcher.h"

#include "SubmodulesView.h"
#include "SubmodulesCreateEditDlg.h"


class SubmodulesViewDelegate : public QItemDelegate
{
public:
	SubmodulesViewDelegate( QObject* parent );
public:
	void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;

    inline void drawDisplay(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QModelIndex &index) const;
};


SubmodulesViewDelegate::SubmodulesViewDelegate( QObject* parent )
	: QItemDelegate( parent )
{
}

void SubmodulesViewDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option,
									const QModelIndex& index ) const
{
	if( index.column() != 0 )
	{
		QItemDelegate::paint( painter, option, index );
		return;
	}

	drawBackground( painter, option, index );

    drawDisplay( painter, option, option.rect.adjusted( 1, 1, -1, -1 ), index );

    QIcon theDecoration( index.data(Qt::DecorationRole).value<QIcon>() );
    QRect decorationRect(QPoint(0, 0), option.decorationSize);
    if ( option.decorationAlignment.testFlag(Qt::AlignVCenter) )
    {
         QPoint newCenter( decorationRect.center().x(), option.rect.center().y() );
         decorationRect.moveCenter( newCenter );
    }
    QItemDelegate::drawDecoration( painter, option, decorationRect, theDecoration.pixmap(decorationRect.size()) );

    drawFocus( painter, option, option.rect );
}

/**
 * @brief Internal function to draw the item data requested on Qt::DisplayRole.
 * @param painter
 * @param option
 * @param rect
 * @param index
 */
void SubmodulesViewDelegate::drawDisplay(QPainter *painter, const QStyleOptionViewItem &option,
                                         const QRect &rect, const QModelIndex &index) const
{
    const QFontMetrics& fm = option.fontMetrics;

    QRect textRect = option.rect;
    textRect.moveLeft( option.decorationSize.width() );
    textRect.setBottom( textRect.top() + fm.lineSpacing() );

    QFont f( option.font );
    f.setBold( true );
    painter->setFont( f );
    painter->drawText( textRect, QLatin1String("Name: ") % index.data(Qt::DisplayRole).toString() );

    textRect.moveTop( textRect.top() + fm.lineSpacing() );
    painter->setFont( option.font );

    const QVariant &submoduleData = index.data(Qt::UserRole + 1);
    if ( submoduleData.canConvert<Git::Submodule>() )
    {
        const Git::Submodule &submodule = index.data(Qt::UserRole + 1).value<Git::Submodule>();
        painter->drawText( textRect, QLatin1String("Version: ") % submodule.currentSHA1().toString() );
    }
}

QSize SubmodulesViewDelegate::sizeHint( const QStyleOptionViewItem& option,
										const QModelIndex& index ) const
{
	const QFontMetrics& fm = option.fontMetrics;

    return QSize( 50, 2 + 2 * fm.lineSpacing() );
}

SubmodulesView::SubmodulesView()
	: GlobalView( QLatin1String( "Submodules" ) )
{
	setupActions( this );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 0 );
	l->setMargin( 0 );

	l->addWidget( tbSMViewToolbar->toolBarFor( this ) );

	mTree = new QTreeView;
	mTree->setRootIsDecorated( false );
	mTree->setHeaderHidden( true );
	mTree->setFrameShape( QFrame::NoFrame );
    mTree->setWordWrap( false );

    //! @todo make this configurable in settings for tree views
    mTree->setIconSize( QSize(24, 24) );

	l->addWidget( mTree );

	mModel = new QStandardItemModel( this );
	mTree->setModel( mModel );

	mTree->setItemDelegate( new SubmodulesViewDelegate( this ) );

	setLayout( l );

    setViewName( trUtf8( "Submodules") );

	connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
			 this, SLOT(repositoryChanged(Git::Repository)) );

	Git::Repository repo = MacGitver::self().repository();
    if( repo.isValid() )
	{
		repositoryChanged( repo );
	}

	connect( MacGitver::self().watcher(), SIGNAL(repoGitFileChanged()),
			 this, SLOT(readSubmodules()) );
}

void SubmodulesView::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;
	readSubmodules();
}

void SubmodulesView::readSubmodules()
{
    if ( !mRepo.isValid() )
    {
        //! @todo: add error handling
        return;
    }

    QIcon decoration( QLatin1String(":/icons/subrepo.svg") );
    foreach( Git::Submodule module, mRepo.submodules() )
	{
		if( !module.isValid() )
			continue;

        QStandardItem * it = new QStandardItem( decoration, QFileInfo(module.name()).baseName() );
        it->setData( QVariant::fromValue(module), Qt::UserRole + 1 );
        it->setToolTip( QLatin1String("URL: ") % module.url() %
                        QLatin1String( "\n" ) %
                        QLatin1String("Path: ") % module.path() );

        it->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
        mModel->appendRow( it );
    }
}

void SubmodulesView::addSubmodule()
{
	SubmodulesCreateEditDlg d;
	d.exec();
}
