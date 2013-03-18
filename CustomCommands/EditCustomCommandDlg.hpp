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

#ifndef MGV_CUSCOM_EDIT_DLG_HPP
#define MGV_CUSCOM_EDIT_DLG_HPP

#include "ui_EditCustomCommandDlg.h"

#include "CustomCommandDef.hpp"

class EditCustomCommandDlg : public QDialog, private Ui::EditCustomCommandDlg
{
    Q_OBJECT
public:
    EditCustomCommandDlg( QWidget* parent,
                          CustomCommandDef::Ptr cmdTemplate = CustomCommandDef::Ptr() );

private:
    void init();
    void loadTemplate();

public:
    CustomCommandDef::Ptr getData( bool overWriteTemplate = true );

private:
    CustomCommandDef::Ptr   mCmdTemplate;
    QButtonGroup*           mRunGroup;
    QButtonGroup*           mRefreshGroup;
};

#endif


