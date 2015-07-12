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

#include "include/music.h"

Music::Music(QString URL, Album *alb) : QObject()
{
    m_URL = URL;
    m_alb = alb;
    m_alb->addMusic(this);
}

QString Music::URL()
{
    return m_URL;
}

QString Music::title()
{
    return m_title;
}

void Music::setTitle(QString title)
{
    m_title = title;
}

QString Music::genre()
{
    return m_genre;
}

void Music::setGenre(QString genre)
{
    m_genre = genre;
}

QString Music::description()
{
    return m_description;
}

void Music::setDescription(QString description)
{
    m_description = description;
}

int Music::date()
{
    return m_date;
}

void Music::setDate(int date)
{
    m_date = date;
}

uint Music::trackNumber()
{
    return m_trackNumber;
}

void Music::setTrackNumber(uint trackNumber)
{
    m_trackNumber = trackNumber;
}

Album *Music::album()
{
    return m_alb;
}

void Music::setAlbum(Album *alb)
{
    m_alb = alb;
}

Artist *Music::artist()
{
    return m_alb->artist();
}
