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

#include <QDebug>
#include <QStandardItemModel>

#include "CustomCommandListCfgPage.hpp"
#include "CustomCommandsModule.hpp"
#include "EditCustomCommandDlg.hpp"

CustomCommandListCfgPage::CustomCommandListCfgPage( ConfigDialog* dlg )
    : ConfigPage( dlg )
{
    init();
}

CustomCommandListCfgPage::~CustomCommandListCfgPage()
{
}

void CustomCommandListCfgPage::apply()
{
    CustomCommandsModule::self().setCommands( mCommands );
    setModified( false );
}

void CustomCommandListCfgPage::init()
{
    setupUi( this );

    connect( cmdAdd, SIGNAL(clicked()), this, SLOT(onAdd()) );
    connect( cmdCopy, SIGNAL(clicked()), this, SLOT(onCopy()) );
    connect( cmdEdit, SIGNAL(clicked()), this, SLOT(onEdit()) );
    connect( cmdRemove, SIGNAL(clicked()), this, SLOT(onRemove()) );

    mModel = new QStandardItemModel( 0, 2, this );

    mModel->setHorizontalHeaderLabels( QStringList()
                                       << trUtf8( "Command" )
                                       << trUtf8( "Scope" ) );
    treeView->setModel( mModel );

    mCommands = CustomCommandsModule::self().commands();
    readCommands();
}

QByteArray CustomCommandListCfgPage::pageId() const
{
    return "Commands";
}

QByteArray CustomCommandListCfgPage::groupId() const
{
    return "General";
}

QString CustomCommandListCfgPage::pageName() const
{
    return trUtf8( "Commands" );
}

QString CustomCommandListCfgPage::groupName() const
{
    return trUtf8( "Gerneral" );
}

void CustomCommandListCfgPage::onAdd()
{
    EditCustomCommandDlg d( this, trUtf8( "Create custom command" ) );
    if( d.exec() )
    {
        CustomCommandDef::Ptr cmd = d.getData( false );
        mCommands.append( cmd );
        addCommand( cmd, true );
        setModified();
    }
}

void CustomCommandListCfgPage::onEdit()
{
    QModelIndex idx = treeView->selectionModel()->currentIndex();
    if( !idx.isValid() )
    {
        return;
    }

    QStandardItem* parent = mModel->invisibleRootItem();
    QStandardItem* it = parent->child( idx.row() );
    Q_ASSERT( it );

    QString id = it->data().toString();
    CustomCommandDef::Ptr cmd = findCommand( id );
    Q_ASSERT( cmd );

    EditCustomCommandDlg d( this, trUtf8( "Edit custom command" ), cmd );
    if( d.exec() )
    {
        d.getData( true );
        updateCommand( cmd );
        setModified();
    }
}

void CustomCommandListCfgPage::onRemove()
{
    QModelIndex idx = treeView->selectionModel()->currentIndex();
    if( !idx.isValid() )
    {
        return;
    }

    QStandardItem* parent = mModel->invisibleRootItem();
    QStandardItem* it = parent->child( idx.row() );
    Q_ASSERT( it );

    QString id = it->data().toString();
    for( int i = 0; i < mCommands.count(); ++i )
    {
        if( mCommands[ i ]->id() == id )
        {
            mCommands.removeAt( i );
            setModified();

            parent->removeRow( idx.row() );
            return;
        }
    }
}

void CustomCommandListCfgPage::onCopy()
{
    QModelIndex idx = treeView->selectionModel()->currentIndex();
    if( !idx.isValid() )
    {
        return;
    }

    QStandardItem* parent = mModel->invisibleRootItem();
    QStandardItem* it = parent->child( idx.row() );
    Q_ASSERT( it );

    QString id = it->data().toString();
    CustomCommandDef::Ptr cmd = findCommand( id );
    Q_ASSERT( cmd );

    EditCustomCommandDlg d( this, trUtf8( "Copy custom command" ), cmd );
    if( d.exec() )
    {
        cmd = d.getData( false );
        mCommands.append( cmd );
        addCommand( cmd, true );
        setModified();
    }
}

void CustomCommandListCfgPage::readCommands()
{
    foreach( CustomCommandDef::Ptr cmd, mCommands )
    {
        addCommand( cmd );
    }
}

QString CustomCommandListCfgPage::execText( CustomCommandDef::ExecuteOn exec )
{
    switch( exec )
    {
    case CustomCommandDef::ExecBranch:                  return trUtf8( "Branch" );
    case CustomCommandDef::ExecRootRepo:                return trUtf8( "Repository (Root)" );
    case CustomCommandDef::ExecSubRepo:                 return trUtf8( "Repository (Submodules)" );
    case CustomCommandDef::ExecRootOrSubRepo:           return trUtf8( "Repository (Root and"
                                                                       " Submodules)" );
    case CustomCommandDef::ExecForEachSubmodule:        return trUtf8( "For each submodule" );
    case CustomCommandDef::ExecForEachSubmoduleDeep:    return trUtf8( "For each submodule"
                                                                       " (Recursive)" );
    case CustomCommandDef::ExecGlobally:                return trUtf8( "Global Command" );
    case CustomCommandDef::ExecInWorkingTree:           return trUtf8( "In working tree" );
    default:                                            return trUtf8( "Unknown" );
    }
}

void CustomCommandListCfgPage::addCommand( CustomCommandDef::Ptr cmd, bool select )
{
    QList< QStandardItem* > row;
    row << new QStandardItem( cmd->name() );
    row << new QStandardItem( execText( cmd->executeOn() ) );
    row[ 0 ]->setData( cmd->id() );
    mModel->appendRow( row );
}

void CustomCommandListCfgPage::updateCommand( CustomCommandDef::Ptr cmd )
{
    if( !cmd )
    {
        return;
    }

    QString id = cmd->id();
    QStandardItem* parent = mModel->invisibleRootItem();
    for( int i = 0; i < parent->rowCount(); ++i )
    {
        QStandardItem* it = parent->child( i );
        if( it->data().toString() == id )
        {
            it->setText( cmd->name() );
            it = parent->child( i, 1 );
            it->setText( execText( cmd->executeOn() ) );
            return;
        }
    }

    addCommand( cmd );
}

CustomCommandDef::Ptr CustomCommandListCfgPage::findCommand( const QString& id ) const
{
    for( int i = 0; i < mCommands.count(); ++i )
    {
        if( mCommands[ i ]->id() == id )
        {
            return mCommands[ i ];
        }
    }

    return CustomCommandDef::Ptr();
}
