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

#include "include/mprismanager.h"

// The fonctions's names are explicit and you can see the MPRIS documentation here: http://xmms2.org/wiki/MPRIS

MPRISManager::MPRISManager() : QObject() // This class manages all the DBus layer
                                         // of Xound
{
    connection = new QDBusConnection(QDBusConnection::sessionBus());
    connection->registerObject("/", this, QDBusConnection::ExportAllContents);

    MPRISPlayer *player = new MPRISPlayer();
    connection->registerObject("/Player", player, QDBusConnection::ExportAllContents);

    MPRISTrackList *tracklist = new MPRISTrackList();
    connection->registerObject("/TrackList", tracklist, QDBusConnection::ExportAllContents);

    connection->registerService("org.mpris.xound");
}

QString MPRISManager::Identity() //Return the name of the MPRIS Player... Xound!
{
    return "Xound";
}

Version MPRISManager::MprisVersion() //This method must return two uint16 so we return a struct of two uint16
{
    struct Version v;
    v.major = 1;
    v.minor = 0;
    return v;
}

void MPRISManager::Quit()
{
    qApp->quit();
}

  ///////////////////////////////////////////////////////////////////////
 /////////////////////////// MPRISPlayer ///////////////////////////////
///////////////////////////////////////////////////////////////////////

MPRISPlayer::MPRISPlayer()
{
    m_c = Core::instance();
}

void MPRISPlayer::Next()
{
    m_c->next();
}

void MPRISPlayer::Prev()
{
    m_c->previous();
}

void MPRISPlayer::Pause()
{
    if(m_c->isPlaying())
        m_c->pause();
    else
        m_c->play();
}

void MPRISPlayer::Stop()
{
    m_c->pause();
}

void MPRISPlayer::Play()
{
    m_c->play();
}

void MPRISPlayer::Repeat(bool b)
{
//    m_c->setRepeat(b);
}

Status MPRISPlayer::GetStatus()
{
    Status s;
    s.state = !m_c->isPlaying();
//    s.repeat = m_c->isRepeat();
//    s.random = m_c->isRandom();
    s.repeatPlayList = true;
    return s;
}

QVariantMap MPRISPlayer::GetMetadata()
{
}

int MPRISPlayer::GetCaps()
{
}

void MPRISPlayer::VolumeSet(int newVol)
{
    m_c->setVolume(newVol);
}

int MPRISPlayer::VolumeGet()
{
    return m_c->volume();
}

void MPRISPlayer::PositionSet(int p)
{
    m_c->setPosition(p);
}

int MPRISPlayer::PositionGet()
{
    return m_c->position();
}

  ///////////////////////////////////////////////////////////////////////
 ////////////////////////// MPRISTrackList /////////////////////////////
///////////////////////////////////////////////////////////////////////

MPRISTrackList::MPRISTrackList()
{
    m_c = Core::instance();
}

int MPRISTrackList::AddTrack(const QString &URL, bool mustPlay)
{
    Music *m = m_c->addMedia(URL);
    if(mustPlay)
        m_c->play(m);
}

void MPRISTrackList::DelTrack(int at)
{
}

int MPRISTrackList::GetCurrentTrack()
{
    if(m_c->currentPlaylist())
        return m_c->currentPlaylist()->currentIndex();
}

int MPRISTrackList::GetLength()
{
    if(m_c->currentPlaylist())
        return m_c->currentPlaylist()->musicList().count();
    else
        return 0;
}

QVariantMap MPRISTrackList::GetMetadata(int at)
{
}

void MPRISTrackList::SetLoop(bool)
{
    // We always loop a playlist
}

void MPRISTrackList::SetRandom(bool b)
{
//    m_c->setRandom(b);
}
