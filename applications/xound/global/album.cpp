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

#include "include/album.h"
#include "include/music.h"

Album::Album(QString name, Artist *art) : QObject()
{
    m_name = name;
    m_art = art;
    m_musics = QList<Music *>();
}

void Album::setArtist(Artist *art)
{
    m_art = art;
}


QPixmap Album::cover()
{

}

Artist *Album::artist()
{
    return m_art;
}

void Album::setName(QString name)
{
    m_name = name;
}

QString Album::name()
{
    return m_name;
}

void Album::addMusic(Music *m)
{
    if(!m_musics.contains(m))
        m_musics.append(m);
}

QList<Music *> Album::musics()
{
    return m_musics;
}
