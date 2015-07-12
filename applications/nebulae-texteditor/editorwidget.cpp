/*
 * This file is part of Nebulae.
 * Copyright (C) 2011 - Leo Testard <leo.testard@gmail.com>
 *
 * Nebulae is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nebulae is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nebulae. If not, see <http://www.gnu.org/licenses/>.
 */

#include "editorwidget.h"
#include "chighlighter.h"

#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QTextStream>
#include <QtCore/QByteArray>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QApplication>


/* TODO
   -> return and algo keywords are not colored
*/

EditorWidget::EditorWidget(QString filePath, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *l = new QVBoxLayout(this);
    mEdit = new QTextEdit(this);
    //CHighlighter *hl = new CHighlighter(mEdit->document());
    mEdit->setFontPointSize(12);
    mEdit->setFontFamily("Courrier");
    l->setContentsMargins(0,0,0,0);
    sFilePath = filePath;
    QFile f(filePath);
    QString text;
    if(filePath.isEmpty() && filePath.isNull())
    {
        sFilePath = QString();
        sTitle = tr("New document");
        text = QString();
    }
    else {
        if(f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            text = f.readAll();
            sTitle = filePath.split("/").takeLast();
            f.close();
        }
        else
        {
            sFilePath = QString();
            sTitle = tr("New document");
            text = QString();
            QMessageBox::critical(this, tr("Error"), tr("An error occured, the file ") + filePath + tr(" could not be opened. Please check that this file is readable and no corrupted and try again."), QMessageBox::Ok);
        }
    }

    bSaved = true;
    mEdit->setText(text);
    connect(mEdit, SIGNAL(textChanged()), this, SLOT(textModified()));
    connect(mEdit, SIGNAL(undoAvailable(bool)), this, SLOT(undoEnabled(bool)));
    connect(mEdit, SIGNAL(redoAvailable(bool)), this, SLOT(redoEnabled(bool)));
    connect(mEdit, SIGNAL(copyAvailable(bool)), this, SLOT(copyEnabled(bool)));
    l->addWidget(mEdit);
    setLayout(l);

    bUndo = false;
    bRedo = false;
    bCopy = false;
}

void EditorWidget::saveAs()
{
    QString oldFilePath = sFilePath;
    sFilePath = QFileDialog::getSaveFileName(this, "Save file", QDir::homePath());
    if(sFilePath.isEmpty() && sFilePath.isNull())
    {
        sFilePath = oldFilePath;
        return;
    }
    sTitle = sFilePath.split("/").takeLast();
    QSettings set(QDir::homePath() + "/settings.ini", QSettings::IniFormat);
    set.beginGroup("RecentFiles");
    QStringList files = set.value("Files").toString().split('|');
    if(files.count() > 5)
        files.removeLast();
    files.append(sFilePath);
    set.setValue("Files", files.join("|"));
    set.endGroup();
    saveFile();
    emit titleChanged();
}

void EditorWidget::save()
{
    if(sFilePath.isEmpty() && sFilePath.isNull())
    {
        sFilePath = QFileDialog::getSaveFileName(this, "Save file", QDir::homePath());
        if(sFilePath.isEmpty() && sFilePath.isNull())
            return;
        sTitle = sFilePath.split("/").takeLast();
        QSettings set(QDir::homePath() + "/settings.ini", QSettings::IniFormat);
        set.beginGroup("RecentFiles");
        QStringList files = set.value("Files").toString().split('|');
        if(files.count() > 5)
            files.removeLast();
        files.append(sFilePath);
        set.setValue("Files", files.join("|"));
        set.endGroup();
    }
    saveFile();
    emit titleChanged();
}

bool EditorWidget::closeFile()
{
    if(!bSaved)
    {
        int ret = QMessageBox::question(this, tr("Valid changes"), tr("Current file has been modified. Close it may lost changes. Save now ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(ret == QMessageBox::Cancel)
            return false;
        if(ret == QMessageBox::Yes)
            save();
    }
    sFilePath = QString();
    sTitle = QString();
    bSaved = true;
    mEdit->setText(QString());
    return true;
}

QString EditorWidget::title()
{
    return sTitle;
}

QString EditorWidget::file()
{
    return sFilePath;
}

bool EditorWidget::isSaved()
{
    return bSaved;
}

void EditorWidget::saveFile()
{
    QFile f(sFilePath);
    QTextStream out(&f);
    if(!f.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Error"), tr("An error occured, the file ") + sFilePath + tr(" could not be saved. Please check that this file is wrtieable and no corrupted and try again."), QMessageBox::Ok);
        return;
    }
    else
    {
        out << mEdit->toPlainText();
        f.close();
        sTitle = sFilePath.split("/").takeLast();
        bSaved = true;
    }
    emit titleChanged();
}

void EditorWidget::textModified()
{
    if(bSaved)
    {
        sTitle = "*" + sTitle;
        bSaved = false;
        emit titleChanged();
    }
}
