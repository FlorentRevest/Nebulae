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

#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>

class EditorWidget : public QWidget
{
    Q_OBJECT
public:
    EditorWidget(QString filepath = 0, QWidget *parent = 0);
    void save();
    void saveAs();
    bool closeFile();

    void print()
    {
        QPrinter *p = new QPrinter();
        QPrintDialog printDialog(p, this);
        if (printDialog.exec() == QDialog::Accepted) {
            mEdit->print(p);
        }
    }
    void undo()
    {
        mEdit->undo();
    }
    void redo()
    {
        mEdit->redo();
    }
    void copy()
    {
        mEdit->redo();
        mEdit->copy();
    }
    void cut()
    {
        mEdit->cut();
    }
    void paste()
    {
        mEdit->paste();
    }
    void search()
    {

    }

    QString title();
    QString file();
    bool isSaved();
    bool canUndo()
    {
        return bUndo;
    }
    bool canRedo()
    {
        return bRedo;
    }
    bool canCopy()
    {
        return bCopy;
    }
signals:
    void titleChanged();
    void undoAvailable(bool);
    void redoAvailable(bool);
    void copyAvailable(bool);
private slots:
    void textModified();
    void undoEnabled(bool e)
    {
        bUndo = e;
        emit undoAvailable(e);
    }
    void redoEnabled(bool e)
    {
        bRedo = e;
        emit redoAvailable(e);
    }
    void copyEnabled(bool e)
    {
        bCopy = e;
        emit copyAvailable(e);
    }
private:
    void saveFile();
    QString sFilePath;
    QString sTitle;
    bool bSaved;
    QTextEdit *mEdit;
    bool bUndo, bRedo, bCopy;
};

#endif // EDITORWIDGET_H
