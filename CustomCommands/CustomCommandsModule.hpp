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

#ifndef MGV_MODULE_CUSTOM_COMMANDS_HPP
#define MGV_MODULE_CUSTOM_COMMANDS_HPP

#include "libMacGitverCore/MacGitver/Module.h"

#include "CustomCommandDef.hpp"
#include "hic_CustomCommandActions.h"

class CustomCommandsModule : public Module, private CustomCommandActions
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID "org.macgitver.Module/0.1" FILE "Module.json" )
    Q_INTERFACES( Module )
public:
    CustomCommandsModule();
    static CustomCommandsModule& self();

public:
    void initialize();
    void deinitialize();
    void setupConfigPages( ConfigDialog* dialog );

    CustomCommandDef::List commands() const;
    void setCommands( const CustomCommandDef::List& commands );

private slots:
    void onMergeExecuteOnBranch( Heaven::DynamicActionMerger* dam );

private:
    void loadCommands();
    void saveCommands();
    QString commandsFileName() const;

private:
    static CustomCommandsModule* sSelf;
    CustomCommandDef::List  mCommands;
};

#endif
