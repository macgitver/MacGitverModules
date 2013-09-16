/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Nils Fenner <nilsfenner@web.de>
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

#include "RefRenameDialog.hpp"
#include "ui_RefRenameDialog.h"

#include "RefItem.hpp"

#include "libGitWrap/Reference.hpp"

RefRenameDialog::RefRenameDialog(QWidget *parent)
    : QDialog( parent )
    , ui( new Ui::RefRenameDialog )
    , mRefInfo( 0 )
{
    ui->setupUi( this );

    setFixedSize( size() );
}

RefRenameDialog::~RefRenameDialog()
{
    delete ui;
}

void RefRenameDialog::init( RefBranch* refInfo )
{
    mRefInfo = refInfo;

    updateValues();
}

const Git::Result &RefRenameDialog::gitResult() const
{
    return mGitResult;
}

void RefRenameDialog::accept()
{
    if ( !mRefInfo )
    {
        reject();
        return;
    }

    Git::Reference ref = mRefInfo->reference();

    QString newRefName = ui->textRefName->text();
    if ( !newRefName.isEmpty() && (ref.shorthand() != newRefName) )
    {
        ref.rename( mGitResult, newRefName );
    }

    if ( mGitResult )
        QDialog::accept();
    else
        reject();
}

void RefRenameDialog::updateValues()
{
    if ( !mRefInfo ) return;

    const Git::Reference& ref = mRefInfo->reference();
    Q_ASSERT( ref.isValid() );
    ui->textRefName->setText( ref.shorthand() );
}
