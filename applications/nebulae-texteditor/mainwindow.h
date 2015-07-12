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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tabwidget.h"
#include "editorwidget.h"

#include <QtCore/QSettings>

#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    //actions
    void open();
    void save();
    void saveAs();
    void saveAll();
    void closeFile();
    void print();
    void quit();
    //edit
    void undo();
    void redo();
    void copy();
    void cut();
    void paste();
    void search();
    // view
    void hideToolbar(bool);
    void fixToolbar(bool);
    // tools
    void options();
    // help
    void about();
private slots:
    void newTab();
    void closeTab(int);
    void tabChanged(int);
    void tabTitleChanged();
    void clipboardChanged();
    void textDropped(QString);
    void openRecent();
private:
    void newTab(const QString &filePath);
    TabWidget *tabs;
    QAction *aUndo, *aRedo;
    QAction *aCopy, *aCut, *aPaste;
    QToolBar *tBar;
    QSettings *set;
protected:
    void closeEvent(QCloseEvent *event)
    {
        event->ignore();
        quit();
    }
};
#endif // MAINWINDOW_H
