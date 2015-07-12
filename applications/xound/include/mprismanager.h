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

#ifndef MPRISMANAGER_H
#define MPRISMANAGER_H

#include <QObject>
#include <QApplication>
#include <QDBusConnection>

#include "core.h"
#include "playlist.h"

struct Version
{
    quint16 major;
    quint16 minor;
};

//It manages the / on DBus, and it makes Xound compatible to the MPRIS specification
class MPRISManager : public QObject
{
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.freedesktop.MediaPlayer")

public:
    MPRISManager();

private:
    QDBusConnection *connection;

public slots:
    QString Identity();
    Version MprisVersion();
    void Quit();
};

struct Status
{
    int state;  // 0 = Playing, 1 = Paused, 2 = Stopped.
    int random;
    int repeat;
    int repeatPlayList;
};

// /Player on the DBus interface
class MPRISPlayer : public QObject
{
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.freedesktop.MediaPlayer")

public:
    MPRISPlayer();

public slots:
    void Next();
    void Prev();
    void Pause();
    void Stop();
    void Play();
    void Repeat(bool in0);
    Status GetStatus();
    QVariantMap GetMetadata();
    int GetCaps();
    void VolumeSet(int in0);
    int VolumeGet();
    void PositionSet(int in0);
    int PositionGet();

signals:
    void CapsChange(int);
    void TrackChange(QVariantMap);
    void StatusChange(Status);

private:
    Core *m_c;
};

// /TrackList on DBus interface
class MPRISTrackList : public QObject
{
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.freedesktop.MediaPlayer")

public:
    MPRISTrackList();

private:
    Core *m_c;

public slots:
    QVariantMap GetMetadata(int in0);
    int GetCurrentTrack();
    int GetLength();
    int AddTrack(const QString &in0, bool in1);
    void DelTrack(int in0);
    void SetLoop(bool in0);
    void SetRandom(bool in0);

signals:
    void TrackListChange(int in0);
};
#endif // MPRISMANAGER_H
