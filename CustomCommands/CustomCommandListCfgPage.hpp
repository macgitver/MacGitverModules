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

#ifndef MGV_CUSCOM_CONFIG_PAGE_HPP
#define MGV_CUSCOM_CONFIG_PAGE_HPP

#include "libMacGitverCore/Config/Ui/ConfigDialog.hpp"

#include "CustomCommandDef.hpp"
#include "ui_CustomCommandListCfgPage.h"

class QStandardItemModel;

class CustomCommandListCfgPage : public ConfigPage, Ui::CustomCommandListCfgPage
{
    Q_OBJECT

public:
    CustomCommandListCfgPage( ConfigDialog* dlg );
    ~CustomCommandListCfgPage();

public:
    void apply();
    void init();

    QByteArray pageId() const;
    QByteArray groupId() const;

    QString pageName() const;
    QString groupName() const;

private slots:
    void onAdd();
    void onEdit();
    void onRemove();
    void onCopy();
    void onSelectionChanged();

private:
    void readCommands();
    CustomCommandDef::Ptr findCommand( const QString& id ) const;
    void addCommand( CustomCommandDef::Ptr cmd, bool select = false );
    void updateCommand( CustomCommandDef::Ptr cmd );
    static QString execText( CustomCommandDef::ExecuteOn exec );

private:
    CustomCommandDef::List  mCommands;
    QStandardItemModel*     mModel;
};

#endif
