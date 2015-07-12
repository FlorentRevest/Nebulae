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

#ifndef ARTISTWIDGET_H
#define ARTISTWIDGET_H

#include <QScrollArea>
#include <QListWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>

#include "core.h"
#include "include/artist.h"
#include "include/album.h"
#include "include/music.h"

class ArtistWidget : public QScrollArea
{
Q_OBJECT
public:
    ArtistWidget(Artist *, QWidget *parent = 0);

public slots:
    void itemDoubleClicked(QListWidgetItem *);
    void itemClicked(QListWidgetItem *);
    void returnToArtists();
    void showMenu(QPoint);
    void addToPlaylist();
    void currentIndexChanged();

signals:
    void closed();

protected:
    void resizeEvent(QResizeEvent *);

private:
    Core *m_c;
    QHash<QListWidget *, Album *>  m_albumHash;
    QHash<QListWidgetItem *, Music *> m_musicHash;
    QHash<Music *, uint > m_indexHash;
    QHash<QAction *, Playlist *> m_playlistHash;
    QGridLayout *m_albumLayout;
    QListWidgetItem *m_currentItem;
    Playlist *m_playlist;
};

#endif // ARTISTWIDGET_H
