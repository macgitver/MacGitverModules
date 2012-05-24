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

#include <QFileInfo>
#include <QDir>
#include <QPluginLoader>

#include "MacGitver/Modules.h"

#include "Main/MacGitverMain.h"
#include "Main/MainWindow.h"

MacGitverMain::MacGitverMain( int argc, char** argv )
	: MacGitver( argc, argv )
{
}

void MacGitverMain::loadModules()
{
	QFileInfo fi( arguments().at( 0 ) );
	QDir binDir = fi.absoluteDir();

	QStringList modFiles;
	modFiles << "Mod*.mgv";
	foreach( QString modName, binDir.entryList( modFiles ) )
	{
		QPluginLoader* loader = new QPluginLoader( modName, this );
		QObject* o = loader->instance();
		Module* mod = qobject_cast< Module* >( o );
		if( mod )
		{
			modules()->addModule( mod );
		}
		else
		{
			delete loader;
		}
	}
}

int MacGitverMain::exec()
{
	loadModules();

	MainWindow* mw = new MainWindow;
	setMainWindow( mw );

	QMetaObject::invokeMethod( mw, "show", Qt::QueuedConnection );

	return MacGitver::exec();
}
