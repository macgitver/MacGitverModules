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

#include <QtPlugin>
#include <QStringBuilder>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>

#include "libHeaven/App/Application.hpp"

#include "CustomCommandsModule.hpp"
#include "CustomCommandListCfgPage.hpp"

CustomCommandsModule::CustomCommandsModule()
{
}

CustomCommandsModule& CustomCommandsModule::self()
{
    Q_ASSERT( sSelf );
    return *sSelf;
}

CustomCommandsModule* CustomCommandsModule::sSelf = NULL;

void CustomCommandsModule::initialize()
{
    Q_ASSERT( sSelf == NULL );
    sSelf = this;
    setupActions( this );
    loadCommands();

    acCustComAC->mergeInto( "CustomToolsMP" );
}

void CustomCommandsModule::deinitialize()
{
    sSelf = NULL;
}

void CustomCommandsModule::setupConfigPages( ConfigDialog* dialog )
{
    dialog->addPage( new CustomCommandListCfgPage( dialog ) );
}

void CustomCommandsModule::onMergeExecuteOnBranch( Heaven::DynamicActionMerger* dam )
{
    dam->addAction( new QAction( QLatin1String( "&Foo" ), this ) );
}

void CustomCommandsModule::loadCommands()
{
    QString fn = commandsFileName();
    QFile f( fn );
    if( !f.open( QFile::ReadOnly ) )
    {
        return;
    }

    QDomDocument doc;
    doc.setContent( &f );
    QDomElement el = doc.documentElement().firstChildElement();
    while( el.isElement() )
    {
        CustomCommandDef::Ptr cmd = CustomCommandDef::Ptr( new CustomCommandDef( el ) );
        mCommands.append( cmd );

        el = el.nextSiblingElement();
    }
}

void CustomCommandsModule::saveCommands()
{
    QString fn = commandsFileName();

    QDomDocument doc( QLatin1String( "CustomCommands" ) );
    QDomElement elRoot = doc.createElement( QLatin1String( "CustomCommands" ) );
    doc.appendChild( elRoot );

    foreach( CustomCommandDef::Ptr cmd, mCommands )
    {
        cmd->saveTo( elRoot );
    }

    QString xml = doc.toString();

    QFile f( fn );
    if( !f.open( QFile::WriteOnly ) )
    {
        return;
    }
    f.write( xml.toUtf8() );
}

QString CustomCommandsModule::commandsFileName() const
{
    QString base = Heaven::Application::dataPath();
    return base % QLatin1Literal( "/commands.xml" );
}

CustomCommandDef::List CustomCommandsModule::commands() const
{
    return mCommands;
}

void CustomCommandsModule::setCommands( const CustomCommandDef::List& commands )
{
    mCommands = commands;
    saveCommands();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( CustomCommands, CustomCommandsModule )
#endif
