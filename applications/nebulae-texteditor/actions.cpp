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

#include <QtGui/QFileDialog>
#include <QtGui/QTextEdit>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>

void MainWindow::open()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
    newTab(file);
}

void MainWindow::save()
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->currentWidget());
    ed->save();
}

void MainWindow::saveAs()
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->currentWidget());
    ed->saveAs();
}

void MainWindow::saveAll()
{
    for(int i = 0 ; i < tabs->count() ; i++)
    {
        EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->widget(i));
        ed->save();
    }
}

void MainWindow::closeFile()
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->currentWidget());
    bool close = ed->closeFile();
    if(close)
        tabs->removeTab(tabs->currentIndex());
}

void MainWindow::print()
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->currentWidget());
    ed->print();
}

void MainWindow::quit()
{
    for(int i = 0 ; i < tabs->count() ; i++)
    {
        EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->widget(i));
        if(!ed->closeFile())
        {
            return;
        }
    }
    qApp->quit();
}

void MainWindow::undo()
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->currentWidget());
    ed->undo();
}

void MainWindow::redo()
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->currentWidget());
    ed->redo();
}

void MainWindow::copy()
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->currentWidget());
    ed->copy();
}

void MainWindow::cut()
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->currentWidget());
    ed->cut();
}

void MainWindow::paste()
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->currentWidget());
    ed->paste();
}

void MainWindow::search()
{
    EditorWidget *ed = qobject_cast<EditorWidget *>(tabs->currentWidget());
    ed->search();
}

void MainWindow::options()
{
}

void MainWindow::about()
{
    QMessageBox::information(this, tr("About this software..."), tr("Nebulae text editor. Copyright 2011 (C) Leo Testard <leo.testard@gmail.com>. See COPYING for informations about licensing."), QMessageBox::Ok);
}


