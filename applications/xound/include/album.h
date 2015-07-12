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

#ifndef ALBUM_H
#define ALBUM_H

#include <QObject>
#include <QList>
#include <QPixmap>

#include "include/artist.h"

class Music;

class Album : public QObject
{
Q_OBJECT
public:
    Album(QString name, Artist *art);

private:
    QString m_name;
    QList<Music *> m_musics;
    Artist *m_art;

public:
    QList<Music *> musics();
    QString name();
    QPixmap cover();
    Artist *artist();
    void setArtist(Artist *art);
    void setName(QString);
    void addMusic(Music *m);
};

#endif // ALBUM_H
