/*Copyright (C) 2010 Revest Florent <florent.revest666@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef WORKZONE_H
#define WORKZONE_H

#include <QSplitter>
#include <QTreeWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QInputDialog>
#include <QHashIterator>

#include "include/core.h"
#include "include/playlistwidget.h"
#include "include/musiclibrary.h"
#include "include/videolibrary.h"

class WorkZone : public QSplitter
{
Q_OBJECT

public:
    WorkZone(QWidget *parent = 0);

private:
    Core *m_c;
    QTreeWidget *m_treeMenu;
    QPushButton *m_removeButton, *m_addButton, *m_openButton, *m_saveButton;
    QTreeWidgetItem *videoLibraryItem, *musicLibraryItem, *playlist;
    QHash<QTreeWidgetItem*, QWidget*> *m_widgetHash;
    QWidget *m_widget;
    QSizePolicy mainWidgetSP;
    MusicLibrary *m_ml;
    VideoLibrary *m_vl;

public slots:
    void itemIsClicked(QTreeWidgetItem*, int);

private slots:
    void save();
    void open();
    void add();
    void remove();
};

#endif // WORKZONE_H
