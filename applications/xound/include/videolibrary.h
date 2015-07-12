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

#ifndef VIDEOLIBRARY_H
#define VIDEOLIBRARY_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHash>
#include <QListWidgetItem>

#include <phonon/videowidget.h>
#include <phonon/mediaobject.h>

#include <libffmpegthumbnailer/videothumbnailer.h>

#include "include/core.h"

class VideoLibrary : public QWidget
{
Q_OBJECT
public:
    VideoLibrary(QWidget *parent = 0);
    QIcon thumbnail(QString);

public slots:
    void add();
    void addFolder();
    void search(QString);
    void play(QListWidgetItem*);

protected:
    void resizeEvent(QResizeEvent *);

private:
    QListWidget *m_videoList;
    QHash<QListWidgetItem *, QString> *m_URLHash;
    QPushButton *m_addButton, *m_addFolderButton;
    QLineEdit *m_searchLine;
    Core *m_c;
    ffmpegthumbnailer::VideoThumbnailer m_thumbnailer;
};

#endif // VIDEOLIBRARY_H
