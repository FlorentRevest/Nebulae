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

#include "include/artist.h"
#include "include/album.h"

Artist::Artist() : QObject()
{
    m_name = "";
    m_albumHash = QHash<QString, Album *>();
}

void Artist::setName(QString name)
{
    m_name = name;
}

QString Artist::name()
{
    return m_name;
}

QString Artist::biography()
{

}

QPixmap Artist::photo()
{
    if(QFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.cache/Xound/" + name() + "/photo.jpg").exists())
    {
        QPixmap pix = QPixmap(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.cache/Xound/" + name() + "/photo.jpg");
        if(!pix.isNull())
            return pix;
        else
            return QPixmap(":/Artist.png");
    }
    else
        return QPixmap(":/Artist.png");
}

void Artist::addAlbum(Album *a)
{
    if(!m_albumHash.values().contains(a))
        m_albumHash.insert(a->name(), a);
}

Album *Artist::album(QString name)
{
    Album *alb = m_albumHash.value(name);
    if(alb == 0) //If the album doesn't already exists
    {
        alb = new Album(name, this);
        m_albumHash.insert(name, alb);
    }
    return alb;
}

QList<Album *> Artist::albums()
{
    return m_albumHash.values();
}
