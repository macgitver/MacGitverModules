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

#include <QSignalMapper>

#include "libMacGitverCore/Config/Config.h"

#include "IndexConfigPage.h"

IndexConfigPage::IndexConfigPage( ConfigDialog* dlg )
    : ConfigPage( dlg )
{
    setupUi( this );
    init();
}

IndexConfigPage::~IndexConfigPage()
{
}

void IndexConfigPage::apply()
{
    int i = 0;
    if( chkLayoutCommitBottom->isChecked() )
        i = 0;
    else if( chkLayoutCommitTop->isChecked() )
        i = 1;
    else if( chkLayoutStageLeft->isChecked() )
        i = 2;
    else if( chkLayoutStageLeftCommitTop->isChecked() )
        i = 3;

    Config::self().set( "Index/SplitLayout", i );
}

void IndexConfigPage::init()
{
    QSignalMapper* map = new QSignalMapper( this );
    map->setMapping( chkLayoutCommitBottom, 0 );
    map->setMapping( chkLayoutCommitTop, 1 );
    map->setMapping( chkLayoutStageLeft, 2 );
    map->setMapping( chkLayoutStageLeftCommitTop, 3 );

    connect( chkLayoutCommitBottom, SIGNAL(clicked()), map, SLOT(map()) );
    connect( chkLayoutCommitTop, SIGNAL(clicked()), map, SLOT(map()) );
    connect( chkLayoutStageLeft, SIGNAL(clicked()), map, SLOT(map()) );
    connect( chkLayoutStageLeftCommitTop, SIGNAL(clicked()), map, SLOT(map()) );

    int i = Config::self().get( "Index/SplitLayout", 0 ).toInt();
    chkLayoutCommitBottom->setChecked( i == 0 );
    chkLayoutCommitTop->setChecked( i == 1 );
    chkLayoutStageLeft->setChecked( i == 2 );
    chkLayoutStageLeftCommitTop->setChecked( i==3 );

    connect( map, SIGNAL(mapped(int)), this, SLOT(onLayoutChanged(int)) );
}

QByteArray IndexConfigPage::pageId() const
{
    return "Index";
}

QByteArray IndexConfigPage::groupId() const
{
    return "Views";
}

QString IndexConfigPage::pageName() const
{
    return trUtf8( "Stage" );
}

QString IndexConfigPage::groupName() const
{
    return trUtf8( "Views" );
}

void IndexConfigPage::onLayoutChanged( int newPos )
{
    setModified( this );
}
