/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
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

#include "CustomCommandListCfgPage.hpp"
#include "EditCustomCommandDlg.hpp"

CustomCommandListCfgPage::CustomCommandListCfgPage( ConfigDialog* dlg )
    : ConfigPage( dlg )
{
    setupUi( this );
    init();
}

CustomCommandListCfgPage::~CustomCommandListCfgPage()
{
}

void CustomCommandListCfgPage::apply()
{
}

void CustomCommandListCfgPage::init()
{
    connect( cmdAdd, SIGNAL(clicked()), this, SLOT(onAdd()) );
    connect( cmdCopy, SIGNAL(clicked()), this, SLOT(onCopy()) );
    connect( cmdEdit, SIGNAL(clicked()), this, SLOT(onEdit()) );
    connect( cmdRemove, SIGNAL(clicked()), this, SLOT(onRemove()) );
}

QByteArray CustomCommandListCfgPage::pageId() const
{
    return "Commands";
}

QByteArray CustomCommandListCfgPage::groupId() const
{
    return "General";
}

QString CustomCommandListCfgPage::pageName() const
{
    return trUtf8( "Commands" );
}

QString CustomCommandListCfgPage::groupName() const
{
    return trUtf8( "Gerneral" );
}

void CustomCommandListCfgPage::onAdd()
{
    EditCustomCommandDlg d( this );
    if( d.exec() )
    {
        /* update this */
    }
}

void CustomCommandListCfgPage::onEdit()
{
}

void CustomCommandListCfgPage::onRemove()
{
}

void CustomCommandListCfgPage::onCopy()
{
}
