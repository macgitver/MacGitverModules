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

#include "libMacGitverCore/App/MacGitver.hpp"

#include "IndexModule.h"
#include "IndexView.h"

IndexModule::IndexModule()
{
}

Heaven::View* IndexModule::createIndexView()
{
    return new IndexView;
}

void IndexModule::initialize()
{
    registerView( QLatin1String( "Index" ), Heaven::GlobalViewType,
                  & IndexModule::createIndexView );
}

void IndexModule::deinitialize()
{
    unregisterView( QLatin1String( "Index" ) );
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( Index, IndexModule )
#endif
