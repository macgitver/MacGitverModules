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
#include <QFileDialog>

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/MacGitver/RepoManager.hpp"
#include "libMacGitverCore/MacGitver/RepositoryInfo.hpp"

#include "GitConfigModule.h"
#include "GitConfigDialog.h"

GitConfigModule::GitConfigModule()
{
}

void GitConfigModule::initialize()
{
    setupActions( this );
    acGitConfigAC->mergeInto( "ToolsMenuMP" );
}

void GitConfigModule::deinitialize()
{
}

void GitConfigModule::onToolsGitConfig()
{
    Git::Repository repo;
    RepositoryInfo* info = MacGitver::repoMan().activeRepository();
    if (info) {
        repo = info->gitRepo();
    }
    GitConfigDialog( repo ).exec();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( GitConfig, GitConfigModule )
#endif
