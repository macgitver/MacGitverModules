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

#include <QtPlugin>

#include "CustomCommandsModule.hpp"
#include "CustomCommandsView.hpp"
#include "CustomCommandListCfgPage.hpp"

CustomCommandsModule::CustomCommandsModule()
{
}

Heaven::View* CustomCommandsModule::createCustomCommandsView()
{
    return new CustomCommandsView();
}

void CustomCommandsModule::initialize()
{
    registerView( QLatin1String( "Custom_Commands" ),
                  Heaven::GlobalViewType,
                  &CustomCommandsModule::createCustomCommandsView );
}

void CustomCommandsModule::deinitialize()
{
    unregisterView( QLatin1String( "Custom_Commands" ) );
}

void CustomCommandsModule::setupConfigPages( ConfigDialog* dialog )
{
    dialog->addPage( new CustomCommandListCfgPage( dialog ) );
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( CustomCommands, CustomCommandsModule )
#endif
