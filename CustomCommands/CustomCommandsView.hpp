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

#ifndef MGV_CUSTOM_COMMANDS_VIEW_HPP
#define MGV_CUSTOM_COMMANDS_VIEW_HPP

#include "libHeaven/CentralUI/Views/View.hpp"

class QTextBrowser;

class CustomCommandsView : public Heaven::View
{
    Q_OBJECT
public:
    CustomCommandsView();

public:
    QSize sizeHint() const;

private:
    QTextBrowser* mBrowser;
};

#endif

