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

#include <QButtonGroup>

#include "EditCustomCommandDlg.hpp"

EditCustomCommandDlg::EditCustomCommandDlg( QWidget*                parent,
                                            const QString&          title,
                                            CustomCommandDef::Ptr   cmdTemplate )
    : QDialog( parent )
    , mCmdTemplate( cmdTemplate )
{
    init();
    setWindowTitle( title );
}

void EditCustomCommandDlg::init()
{
    setupUi( this );

    mRefreshGroup = new QButtonGroup( this );
    mRefreshGroup->addButton( chkRefreshAlways, CustomCommandDef::RefreshAlways );
    mRefreshGroup->addButton( chkRefreshFailure, CustomCommandDef::RefreshSuccess );
    mRefreshGroup->addButton( chkRefreshNever, CustomCommandDef::RefreshNever );
    mRefreshGroup->addButton( chkRefreshSuccess, CustomCommandDef::RefreshSuccess );

    mRunGroup = new QButtonGroup( this );
    mRunGroup->addButton( chkRunBackground, false );
    mRunGroup->addButton( chkRunModal, true );

    chkRefreshNever->setChecked( true );
    chkRunModal->setChecked( true );

    txtWorkingDirectory->setEnabled( false );
    connect( chkCustomWD, SIGNAL(toggled(bool)), txtWorkingDirectory, SLOT(setEnabled(bool)) );

    cboContext->addItem( trUtf8( "Execute on Repository (root)" ),
                         CustomCommandDef::ExecRootRepo );
    cboContext->addItem( trUtf8( "Execute on Repository (submodule)" ),
                         CustomCommandDef::ExecSubRepo );
    cboContext->addItem( trUtf8( "Execute on Repository (root or submodule)" ),
                         CustomCommandDef::ExecRootOrSubRepo );
    cboContext->addItem( trUtf8( "Execute for all submodules" ),
                         CustomCommandDef::ExecForEachSubmodule );
    cboContext->addItem( trUtf8( "Execute for all submodules (recursive)" ),
                         CustomCommandDef::ExecForEachSubmoduleDeep );
    cboContext->addItem( trUtf8( "Execute on Branch" ),
                         CustomCommandDef::ExecBranch );
    cboContext->addItem( trUtf8( "Execute in Working tree" ),
                         CustomCommandDef::ExecInWorkingTree );
    cboContext->addItem( trUtf8( "Execute gobally" ),
                         CustomCommandDef::ExecGlobally );

    loadTemplate();
}

void EditCustomCommandDlg::loadTemplate()
{
    if( mCmdTemplate.data() )
    {
        txtCommandName->setText( mCmdTemplate->name() );
        txtCommands->setPlainText( mCmdTemplate->command() );
        cboContext->setCurrentIndex( cboContext->findData( mCmdTemplate->executeOn() ) );
        mRefreshGroup->button( mCmdTemplate->refreshType() )->setChecked( true );
        mRunGroup->button( mCmdTemplate->runModal() )->setChecked( true );
        chkCustomWD->setChecked( mCmdTemplate->useCustomWorkingDir() );
        txtWorkingDirectory->setText( mCmdTemplate->customWorkingDir() );
    }
}

CustomCommandDef::Ptr EditCustomCommandDlg::getData( bool overWriteTemplate )
{
    CustomCommandDef::Ptr data = mCmdTemplate;
    if( !overWriteTemplate )
    {
        data = new CustomCommandDef;
    }

    data->setName( txtCommandName->text() );
    data->setCommand( txtCommands->toPlainText() );
    data->setRunModal( chkRunModal->isChecked() );
    data->setRefreshType( CustomCommandDef::RefreshType( mRefreshGroup->checkedId() ) );
    data->setExecuteOn( CustomCommandDef::ExecuteOn( cboContext->itemData(
                                                         cboContext->currentIndex() ).toInt() ) );
    data->setUseCustomWorkingDir( chkCustomWD->isChecked() );
    data->setCustomWorkingDir( txtWorkingDirectory->text() );

    return data;
}
