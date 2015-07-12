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

#ifndef MUSIC_H
#define MUSIC_H

#include <QObject>

#include "include/artist.h"
#include "include/album.h"

class Music : public QObject
{
Q_OBJECT
public:
    Music(QString URL, Album *alb);
    void setTitle(QString);
    void setAlbum(Album *);
    void setDate(int);
    void setTrackNumber(uint);
    void setGenre(QString);
    void setDescription(QString);

    QString title();
    QString genre();
    QString description();
    QString URL();
    int date();
    uint trackNumber();
    Album *album();
    Artist *artist();

private:
    Album *m_alb;
    int m_date;
    uint m_trackNumber;
    QString m_title, m_genre, m_description, m_URL;
};

#endif // MUSIC_H
