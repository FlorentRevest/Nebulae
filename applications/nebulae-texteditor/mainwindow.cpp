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

#include "mainwindow.h"
#include "nebulae/nebulae-recentfiles.h"

#include <QtCore/QFile>

#include <QtGui/QClipboard>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QStatusBar>
#include <QtGui/QApplication>
#include <QtGui/QFileIconProvider>

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("Text Editor"));
    resize(700, 400);
    move(50, 50);

    tabs = new TabWidget(this);
    setCentralWidget(tabs);
    tabs->setTabsClosable(true);
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect(tabs, SIGNAL(textDropped(QString)), this, SLOT(textDropped(QString)));

    QStatusBar *sBar = new QStatusBar(this);
    setStatusBar(sBar);

    set = new QSettings("settings.ini", QSettings::IniFormat);
    QList<RecentFilesItem> files =  RecentFiles().allItems();
    QFileIconProvider *pvd = new QFileIconProvider();

    QMenuBar *bar = new QMenuBar(this);
    setMenuBar(bar);
    QMenu *mFile = bar->addMenu(tr("&File"));
    QMenu *mEdit = bar->addMenu(tr("&Edit"));
    QMenu *mView = bar->addMenu(tr("&View"));
    QMenu *mTool = bar->addMenu(tr("&Tool"));
    QMenu *mHelp = bar->addMenu(tr("&Help"));
    QAction *aNew   = mFile->addAction(QIcon::fromTheme("document-new"), tr("New"), this, SLOT(newTab()));
    QAction *aOpen  = mFile->addAction(QIcon::fromTheme("document-open"), tr("Open"), this, SLOT(open()));
    QMenu *mRecents = mFile->addMenu(QIcon::fromTheme(""), tr("Recently used"));
    foreach(RecentFilesItem file, files)
    {
        qDebug() << file.path();
        if(file.mimeType().startsWith("text/"))
            mRecents->addAction(QIcon(), file.path(), this, SLOT(openRecent()));
        if(mRecents->actions().count() == 5)
            break;
    }
    QAction *aSave  = mFile->addAction(QIcon::fromTheme("document-save"), tr("Save"), this, SLOT(save()));
    QAction *aSas   = mFile->addAction(QIcon::fromTheme("document-save-as"), tr("Save as..."), this, SLOT(saveAs()));
    QAction *aSall  = mFile->addAction(QIcon::fromTheme(""), tr("Save all"), this, SLOT(saveAll()));
    QAction *aClose = mFile->addAction(QIcon::fromTheme("edit-delete"), tr("Close"), this, SLOT(closeFile()));
    mFile->addSeparator();
    QAction *aPrint = mFile->addAction(QIcon::fromTheme("document-print"), tr("Print"), this, SLOT(print()));
    QAction *aQuit  = mFile->addAction(QIcon::fromTheme("window-close"), tr("Quit"), this, SLOT(quit()));
    aUndo  = mEdit->addAction(QIcon::fromTheme("back"), tr("Undo"), this, SLOT(undo()));
    aRedo  = mEdit->addAction(QIcon::fromTheme("next"), tr("Redo"), this, SLOT(redo()));
    mEdit->addSeparator();
    aCopy  = mEdit->addAction(QIcon::fromTheme("edit-copy"), tr("Copy"), this, SLOT(copy()));
    aCut   = mEdit->addAction(QIcon::fromTheme("edit-cut"), tr("Cut"), this, SLOT(cut()));
    aPaste = mEdit->addAction(QIcon::fromTheme("edit-paste"), tr("Paste"), this, SLOT(paste()));
    mEdit->addSeparator();
    QAction *aFind  = mEdit->addAction(QIcon::fromTheme("edit-find-replace"), tr("Find and replace"), this, SLOT(find()));
    mEdit->addSeparator();
    QAction *aHide  = mView->addAction(QIcon::fromTheme(""), tr("Hide toolbar"), this, SLOT(hideToolbar(bool)));
    QAction *aFix   = mView->addAction(QIcon::fromTheme(""), tr("Fix toolbar"), this, SLOT(fixToolbar(bool)));
    QAction *aOpt   = mTool->addAction(QIcon::fromTheme(""), tr("Options"), this, SLOT(options()));
    QAction *aAbout = mHelp->addAction(QIcon::fromTheme("help-about"), tr("About"), this, SLOT(about()));

    aNew->setStatusTip(tr("Creates a new empty document"));
    aNew->setShortcut(QKeySequence::New);
    aOpen->setStatusTip(tr("Opens a document from your files"));
    aOpen->setShortcut(QKeySequence::Open);
    mRecents->setStatusTip(tr("Fast access to recently opened documents"));
    aSave->setStatusTip(tr("Save modifications made to the current document"));
    aSas->setStatusTip(tr("Save the document as a new file"));
    aSave->setShortcut(QKeySequence::Save);
    aSall->setStatusTip(tr("Save modifications made to all documents"));
    aSall->setShortcut(QKeySequence("Ctrl + Shift + S"));
    aClose->setStatusTip(tr("Close the current document"));
    aClose->setShortcut(QKeySequence::Close);
    aPrint->setStatusTip(tr("Send the current document to your printer"));
    aPrint->setShortcut(QKeySequence::Print);
    aQuit->setStatusTip(tr("Quit the program"));
    aQuit->setShortcut(QKeySequence("Ctrl + Q"));
    aUndo->setStatusTip(tr("Cancel last action"));
    aUndo->setShortcut(QKeySequence::Undo);
    aRedo->setStatusTip(tr("Redo last cancelled action"));
    aRedo->setShortcut(QKeySequence::Redo);
    aCopy->setStatusTip(tr("Copy selection to clipboard"));
    aCopy->setShortcut(QKeySequence::Copy);
    aCut->setStatusTip(tr("Copy selection and delete it"));
    aCut->setShortcut(QKeySequence::Cut);
    aPaste->setStatusTip(tr("Paste clipboard contents"));
    aPaste->setShortcut(QKeySequence::Paste);
    aFind->setStatusTip(tr("Find words in the text"));
    aFind->setShortcut(QKeySequence::Find);
    aHide->setStatusTip(tr("Hide or show the main toolbar"));
    aHide->setCheckable(true);
    aFix->setStatusTip(tr("Disable/Enable moving toolbar"));
    aFix->setCheckable(true);
    aOpt->setStatusTip(tr("Display options dialog"));
    aOpt->setShortcut(QKeySequence("Ctrl + Shift + O"));
    aAbout->setStatusTip(tr("Displays informations about this software"));

    set->endGroup();
    set->beginGroup("Toolbar");

    tBar = addToolBar(tr("Tool bar"));
    tBar->setAllowedAreas(Qt::TopToolBarArea|Qt::BottomToolBarArea);
    tBar->addAction(aNew);
    tBar->addAction(aOpen);
    tBar->addAction(aSave);
    tBar->addSeparator();
    tBar->addAction(aUndo);
    tBar->addAction(aRedo);
    tBar->addSeparator();
    tBar->addAction(aCopy);
    tBar->addAction(aCut);
    tBar->addAction(aPaste);
    tBar->addSeparator();
    tBar->addAction(aFind);
    aFix->setChecked(false);
    tBar->setMovable(true);
    aHide->setChecked(false);
    tBar->setHidden(false);
    if(set->value("Fixed", 0) == 1)
    {
        aFix->setChecked(true);
        tBar->setMovable(false);
    }
    if(set->value("Enabled", 1) == 0)
    {
        tBar->setHidden(true);
        aHide->setChecked(true);
    }

    set->endGroup();

    newTab();
    clipboardChanged();
    connect(qApp->clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardChanged()));
}

void MainWindow::clipboardChanged()
{
    if(!qApp->clipboard()->text().isNull() || !qApp->clipboard()->text().isEmpty())
        aPaste->setEnabled(true);
    else
        aPaste->setEnabled(false);
}

void MainWindow::newTab()
{
    EditorWidget *ed = new EditorWidget(QString(), this);
    int nIndex = tabs->addTab(ed, QString());
    tabs->setCurrentIndex(nIndex);
    tabs->setTabText(nIndex, ed->title());
    tabs->setTabWhatsThis(nIndex, tr("This is a tab, which contains a text document. Switch tab by clicking on their title to view the document contained in it"));
    tabs->setTabToolTip(nIndex, ed->file());
    setWindowTitle(ed->title());
    aUndo->setEnabled(ed->canUndo());
    aRedo->setEnabled(ed->canRedo());
    aCopy->setEnabled(ed->canCopy());
    aCut->setEnabled(ed->canCopy());
    connect(ed, SIGNAL(titleChanged()), this, SLOT(tabTitleChanged()));
    connect(ed, SIGNAL(undoAvailable(bool)), aUndo, SLOT(setEnabled(bool)));
    connect(ed, SIGNAL(redoAvailable(bool)), aRedo, SLOT(setEnabled(bool)));
    connect(ed, SIGNAL(copyAvailable(bool)), aCopy, SLOT(setEnabled(bool)));
    connect(ed, SIGNAL(copyAvailable(bool)), aCut, SLOT(setEnabled(bool)));
}

void MainWindow::closeTab(int index)
{
    tabs->setCurrentIndex(index);
    closeFile();
}

void MainWindow::newTab(const QString &filePath)
{
    EditorWidget *ed = new EditorWidget(filePath, this);
    int nIndex = tabs->addTab(ed, QString());
    tabs->setCurrentIndex(nIndex);
    tabs->setTabText(nIndex, ed->title());
    tabs->setTabWhatsThis(nIndex, tr("This is a tab, which contains a text document. Switch tab by clicking on their title to view the document contained in it"));
    tabs->setTabToolTip(nIndex, ed->file());
    setWindowTitle(ed->title());
    aUndo->setEnabled(ed->canUndo());
    aRedo->setEnabled(ed->canRedo());
    aCopy->setEnabled(ed->canCopy());
    aCut->setEnabled(ed->canCopy());
    connect(ed, SIGNAL(titleChanged()), this, SLOT(tabTitleChanged()));
    connect(ed, SIGNAL(undoAvailable(bool)), aUndo, SLOT(setEnabled(bool)));
    connect(ed, SIGNAL(redoAvailable(bool)), aRedo, SLOT(setEnabled(bool)));
    connect(ed, SIGNAL(copyAvailable(bool)), aCopy, SLOT(setEnabled(bool)));
    connect(ed, SIGNAL(copyAvailable(bool)), aCut, SLOT(setEnabled(bool)));
}

void MainWindow::tabTitleChanged()
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(sender());
    int nIndex = tabs->indexOf(ed);
    tabs->setTabText(nIndex, ed->title());
    tabs->setTabToolTip(nIndex, ed->file());
    if(tabs->currentIndex() == nIndex)
        setWindowTitle(ed->title());
}

void MainWindow::tabChanged(int index)
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->widget(index));
    setWindowTitle(tabs->tabText(index));
    aUndo->setEnabled(ed->canUndo());
    aRedo->setEnabled(ed->canRedo());
    aCopy->setEnabled(ed->canCopy());
    aCut->setEnabled(ed->canCopy());
}

void MainWindow::textDropped(QString text)
{
    if(QFile(text).exists())
        newTab(text);
}

void MainWindow::openRecent()
{
    QAction *act = qobject_cast<QAction *>(sender());
    newTab(act->text());
}
