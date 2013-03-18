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

#ifndef MGV_CUSTOM_COMMANDS_DEF_HPP
#define MGV_CUSTOM_COMMANDS_DEF_HPP

#include <QSharedData>
#include <QString>
#include <QList>

class QDomElement;

class CustomCommandDef : public QSharedData
{
public:
    enum ExecuteOn
    {
        ExecRootRepo                = 1,
        ExecSubRepo                 = 2,
        ExecRootOrSubRepo           = 3,
        ExecBranch                  = 4,
        ExecForEachSubmodule        = 5,
        ExecForEachSubmoduleDeep    = 6,
        ExecInWorkingTree           = 7,
        ExecGlobally                = 8
    };

    enum RefreshType
    {
        RefreshNever,
        RefreshSuccess,
        RefreshFailure,
        RefreshAlways
    };

public:
    typedef QExplicitlySharedDataPointer< CustomCommandDef > Ptr;
    typedef QList< Ptr > List;

public:
    CustomCommandDef();
    CustomCommandDef( const QDomElement& elSelf );
    CustomCommandDef( const CustomCommandDef& other );
    ~CustomCommandDef();

public:
    QString id() const;
    QString name() const;
    QString command() const;
    ExecuteOn executeOn() const;
    bool runModal() const;
    RefreshType refreshType() const;
    bool useCustomWorkingDir() const;
    QString customWorkingDir() const;

    void newId();
    void setId( const QString& id );
    void setName( const QString& name );
    void setCommand( const QString& command );
    void setExecuteOn( ExecuteOn execOn );
    void setRunModal( bool runModal );
    void setRefreshType( RefreshType type );
    void setUseCustomWorkingDir( bool useCustomWorkingDir );
    void setCustomWorkingDir( const QString& dir );

public:
    void saveTo( QDomElement& elParent );

private:
    QString     mId;
    QString     mName;
    QString     mCommand;
    ExecuteOn   mExecute;
    bool        mRunModal;
    RefreshType mRefresh;
    bool        mCustomWorkDir;
    QString     mWorkDir;
};

#endif
