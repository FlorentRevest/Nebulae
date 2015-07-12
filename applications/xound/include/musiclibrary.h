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

#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include <QWidget>
#include <QPainter>
#include <QListWidget>

#include "include/core.h"
#include "include/artist.h"
#include "include/artistwidget.h"
#include "include/album.h"

class MusicLibrary : public QWidget
{
Q_OBJECT
public:
    MusicLibrary(QWidget *parent = 0);

public slots:
    void showArtistWidget(QListWidgetItem *);
    void artistWidgetClosed();
    void add();
    void addFolder();
    void search(QString);

private:
    QLabel *m_infoLabel;
    Core *m_c;
    ArtistWidget *m_aw;
    QListWidget *m_artistList;
    QHash<QListWidgetItem *, Artist *> *m_itemHash;
    QPushButton *m_addButton, *m_addFolderButton;
    QLineEdit *m_searchLine;

protected:
    void resizeEvent(QResizeEvent *);
};

#endif // MUSICLIBRARY_H
