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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QList>
#include <QString>
#include <QTime>
#include <QDirIterator>
#include <QSettings>
#include <QTextStream>
#include <QDomDocument>

#include "include/music.h"

class Core;

class Playlist : public QObject
{
Q_OBJECT
public:
    Playlist(QString name = "");
    QList<Music *> musicList();
    QString name();
    Music *currentMusic();
    int currentIndex();
    bool isNull();

public slots:
    void setName(QString);
    void addMusic(Music *);
    void removeAll();
    void removeMusicAt(int);
    void moveFromTo(uint, uint);
    void load(QString);
    void save(QString);
    Music *next();
    Music *previous();
    Music *randomMusic();
    void setCurrentIndex(uint);

signals:
    void musicAdded(Music *);
    void musicRemoved(int index);
    void musicMoved(int from, int to);
    void currentIndexChanged();
    void refresh();
    void nameChanged();

private:
    QList<Music *> m_musicList;
    QString m_name;
    int m_currentIndex;
};

#endif // PLAYLIST_H
