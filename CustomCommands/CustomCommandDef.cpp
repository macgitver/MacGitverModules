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

#include "CustomCommandDef.hpp"

CustomCommandDef::CustomCommandDef()
{
}

CustomCommandDef::CustomCommandDef( const CustomCommandDef& other )
{
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
