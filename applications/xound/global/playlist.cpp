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

#include "include/playlist.h"
#include "include/core.h"

Playlist::Playlist(QString name) : QObject() // A realy important class, a playlist!
{
    m_musicList = QList<Music *>();
    m_currentIndex = -1;
    m_name = name;
}

QString Playlist::name() // Returns the name of the playlist
{
    return m_name;
}

void Playlist::setName(QString newName) // Changes the name of the playlist
{
    m_name = newName;
    emit nameChanged();
}

QList<Music *> Playlist::musicList() // Returns a list of the musics in the playlist
{
    return m_musicList;
}

void Playlist::addMusic(Music *m) // Adds a Music
{
    m_musicList.append(m);
    emit musicAdded(m);
}

void Playlist::removeMusicAt(int index) // Returns the URL which is at the position "index" in the URL list
{
    m_musicList.removeAt(index);
    if(index <= m_currentIndex)
    {
        m_currentIndex --;
        emit(currentIndexChanged());
    }

    emit musicRemoved(index);
}

Music *Playlist::currentMusic() // Returns the current music
{
    return m_musicList.at(m_currentIndex);
}

Music *Playlist::next() // Returns the next music in the list
{
    m_currentIndex = (m_currentIndex + 1)% m_musicList.count();
    emit currentIndexChanged();
    return currentMusic();
}

Music *Playlist::previous() // Returns the previous music in the list
{
    if(m_currentIndex == 0)
        m_currentIndex = m_musicList.count() - 1;
    else
        m_currentIndex --;
    emit(currentIndexChanged());
    return currentMusic();
}

void Playlist::setCurrentIndex(uint index) // Change the current music
{
    m_currentIndex = index;
    emit currentIndexChanged();
}

int Playlist::currentIndex() // Return the "adress" of the current music
{
    return m_currentIndex;
}

void Playlist::moveFromTo(uint currentPosition, uint newPosition) // Move a music in the list
{
    m_musicList.move(currentPosition, newPosition);
    emit musicMoved(currentPosition, newPosition);
}

bool Playlist::isNull() // Returns true if the playlist is empty
{
    return m_musicList.isEmpty();
}

void Playlist::load(QString playlistURL)  // This method imports a playlist
{
    if(playlistURL.endsWith(".pls"))
    {
        QSettings settings(playlistURL, QSettings::NativeFormat);
        settings.beginGroup("playlist");
        foreach(QString key, settings.allKeys())
        {
            Core *c = Core::instance();
            if(key.startsWith("File"))
            {
                QString URL = settings.value(key).toString().replace("%20", " ");
                if(URL.startsWith("file://"))
                    URL.remove(0, 7);
                QFileInfo finfo = QFileInfo(URL);
                if(finfo.exists())
                    addMusic(c->addMedia(URL));
            }
        }
        setName(tr("New playlist"));
    }
}

void Playlist::save(QString playlistURL) // This method exports a playlist in different extensions
{
    if(playlistURL.endsWith(".pls"))
    {
        QSettings settings(playlistURL, QSettings::NativeFormat);
        int i= 1;
        settings.beginGroup("playlist");
        settings.setValue("NumberOfEntries", m_musicList.count());
        foreach(Music *m, m_musicList)
        {
            settings.setValue("File" + QString::number(i), m->URL());
            i++;
        }
        settings.setValue("Version", 2);
    }
    if(playlistURL.endsWith(".m3u"))
    {
        QFile file(playlistURL);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        file.write("#EXTM3U\n#PLAYLIST:" + name().toUtf8() + "\n");
        foreach(Music *m, m_musicList)
            file.write(m->URL().toAscii() + "\n");
        file.close();
    }
    if(playlistURL.endsWith(".xspf"))
    {
        QFile file(playlistURL);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QString text = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                       "<playlist version=\"1\" xmlns=\"http://xspf.org/ns/0/\">\n"
                       "  <title>" + name() + "</title>\n"
                       "  <trackList>\n";                         //TODO: remake this fonction with QtXML!

        foreach(Music *m, m_musicList)
            text.append("    <track>\n      <location>" + m->URL() + "</location>\n    </track>\n");

        text.append("  </trackList>\n"
                    "</playlist>");
        file.write(text.toUtf8());
        file.close();
    }
    if(playlistURL.endsWith(".asx"))
    {
        QFile file(playlistURL);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QString text = "<asx version=\"3.0\">\n"
                       "  <title>" + name() + "</title>\n";                         //TODO: remake this fonction with QtXML!

        foreach(Music *m, m_musicList)
            text.append("<entry>\n"
                        "  <ref href=\"" + m->URL() + "\" />\n"
                        "</entry>\n");

        text.append("</asx>");
        file.write(text.toUtf8());
        file.close();
    }
}

Music *Playlist::randomMusic() // Returns a random URL and sets
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    m_currentIndex = qrand() % m_musicList.count();
    emit currentIndexChanged();
    return currentMusic();
}

void Playlist::removeAll() // Removes all the content of the playlist!
{
    while(!m_musicList.empty())
        emit musicRemoved(0);

    m_musicList.clear();
}
