/*
 * MacGitver: Index Module
 * Copyright (C) 2013 Nils Fenner <nilsfenner@web.de>
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

#ifndef MGV_INDEXDLG_H
#define MGV_INDEXDLG_H

#include "ui_IndexDlg.h"

#include <QStandardItemModel>

#include "libGitWrap/ChangeListConsumer.hpp"

namespace Git
{
    class Repository;
}

struct Change
{
    QString oldPath;
    QString newPath;
    Git::ChangeListConsumer::Type type;
    unsigned int similarity;
    bool isBinary;

    Change(const QString &theOldPath, const QString &theNewPath, Git::ChangeListConsumer::Type theType
           , unsigned int theSimilarity, bool theIsBinary)
        : oldPath(theOldPath), newPath(theNewPath), type(theType)
        , similarity(theSimilarity), isBinary(theIsBinary)
    {}
};

class UnstagedConsumer : public Git::ChangeListConsumer
{
public:
    ~UnstagedConsumer()
    {
        qDeleteAll( mChanges );
    }

    bool raw(const QString &oldPath, const QString &newPath, Type type, unsigned int similarity, bool isBinary);

//private:
    QList<Change *>     mChanges;
};

class IndexDlg : public QDialog, private Ui::IndexDlg
{
    Q_OBJECT
public:
    IndexDlg(QWidget *parent = 0);

    void updateIndex( Git::Repository repo );

private slots:

private:

private:
    QStandardItemModel mTestModel;

};

#endif
