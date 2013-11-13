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

#include <QUuid>
#include <QDomElement>

#include "CustomCommandDef.hpp"

CustomCommandDef::CustomCommandDef()
{
    newId();
}

CustomCommandDef::CustomCommandDef( const QDomElement& elSelf )
{
    mId = elSelf.attribute( QLatin1String( "Id" ), QUuid::createUuid().toString() );

    mName = elSelf.attribute( QLatin1String( "Name" ) );
    mRunModal = elSelf.attribute( QLatin1String( "RunModal" ), QLatin1String( "0" ) )
            == QLatin1String( "1" );

    mRefresh = RefreshType( elSelf.attribute( QLatin1String( "RefreshType" ),
                                              QLatin1String( "0" ) ).toInt() );

    mExecute = ExecuteOn( elSelf.attribute( QLatin1String( "ExecuteOn" ),
                                            QLatin1String( "0" ) ).toInt() );

    mWorkDir = elSelf.attribute( QLatin1String( "WD" ) );
    mCustomWorkDir = elSelf.attribute( QLatin1String( "UseWD" ), QLatin1String( "0" ) )
            == QLatin1String( "1" );

    QDomNode child = elSelf.firstChild();
    if( child.isCDATASection() )
    {
        QDomCDATASection cds = child.toCDATASection();
        mCommand = cds.data();
    }
    else if( child.isText() )
    {
        QDomText txt = child.toText();
        mCommand = txt.data();
    }
}

CustomCommandDef::CustomCommandDef( const CustomCommandDef& other )
{
    mId             = other.mId;
    mName           = other.mName;
    mCommand        = other.mCommand;
    mExecute        = other.mExecute;
    mRunModal       = other.mRunModal;
    mRefresh        = other.mRefresh;
    mCustomWorkDir  = other.mCustomWorkDir;
    mWorkDir        = other.mWorkDir;
}

CustomCommandDef::~CustomCommandDef()
{
}

void CustomCommandDef::newId()
{
    mId = QUuid::createUuid().toString();
}

QString CustomCommandDef::id() const
{
    return mId;
}

QString CustomCommandDef::name() const
{
    return mName;
}

QString CustomCommandDef::command() const
{
    return mCommand;
}

CustomCommandDef::ExecuteOn CustomCommandDef::executeOn() const
{
    return mExecute;
}

bool CustomCommandDef::runModal() const
{
    return mRunModal;
}

CustomCommandDef::RefreshType CustomCommandDef::refreshType() const
{
    return mRefresh;
}

bool CustomCommandDef::useCustomWorkingDir() const
{
    return mCustomWorkDir;
}

QString CustomCommandDef::customWorkingDir() const
{
    return mWorkDir;
}

void CustomCommandDef::setId( const QString& id )
{
    mId = id;
}

void CustomCommandDef::setName( const QString& name )
{
    mName = name;
}

void CustomCommandDef::setCommand( const QString& command )
{
    mCommand = command;
}

void CustomCommandDef::setExecuteOn( CustomCommandDef::ExecuteOn execOn )
{
    mExecute = execOn;
}

void CustomCommandDef::setRunModal( bool runModal )
{
    mRunModal = runModal;
}

void CustomCommandDef::setRefreshType( CustomCommandDef::RefreshType type )
{
    mRefresh = type;
}

void CustomCommandDef::setUseCustomWorkingDir( bool useCustomWorkingDir )
{
    mCustomWorkDir = useCustomWorkingDir;
}

void CustomCommandDef::setCustomWorkingDir( const QString& dir )
{
    mWorkDir = dir;
}

void CustomCommandDef::saveTo( QDomElement& elParent )
{
    QDomDocument doc = elParent.ownerDocument();
    QDomElement el = doc.createElement( QLatin1String( "Command" ) );

    el.setAttribute( QLatin1String( "Name" ), mName );
    el.setAttribute( QLatin1String( "RunModal" ), mRunModal ? 1 : 0 );
    el.setAttribute( QLatin1String( "RefreshType" ), int( mRefresh ) );
    el.setAttribute( QLatin1String( "ExecuteOn" ), int( mExecute ) );
    el.setAttribute( QLatin1String( "UseWD" ), mCustomWorkDir ? 1 : 0 );
    el.setAttribute( QLatin1String( "WD" ), mWorkDir );
    el.setAttribute( QLatin1String( "Id" ), mId );

    elParent.appendChild( el );

    QDomCDATASection cmd = doc.createCDATASection( mCommand );
    el.appendChild( cmd );
}
