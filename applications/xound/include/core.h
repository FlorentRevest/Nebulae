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

#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QHash>
#include <QColor>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QFileSystemWatcher>
#include <QDesktopServices>
#include <QDomElement>
#include <QDomNode>
#include <QPluginLoader>

#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <phonon/videowidget.h>

#include <taglib/fileref.h>
#include <taglib/tag.h>

#include "include/artist.h"
#include "include/album.h"
#include "include/music.h"
#include "include/playlist.h"
#include "include/contentplugin.h"
#include "include/infothread.h"

class Core : public QObject
{
Q_OBJECT
public:
    static Core *instance();
    double version();
    bool hasVideo();
    int volume();
    int length();
    int position();
    QMap<QString, QString> metaData();
    Playlist *currentPlaylist();
    QList<Playlist *> playlists();
    void setPlaylist(Playlist *);
    QList<ContentPlugin *> contentPlugins();
    void setMainColor(QColor);
    QColor mainColor();
    void setSplitterGeometry(QByteArray);
    QByteArray splitterGeometry();
    uint note(QString);
    void setNote(QString, uint);
    void random();
    QRect geometry();
    void setGeometry(QRect);
    Artist *artist(QString);
    Album *album(QString);
    QList<Artist *> artists();
    Music *addMedia(QString);
    bool isPlaying();
    bool isMuted();
    void setMuted(bool);
    QString currentURL();
    Music *currentMusic();
    void setVideoWidget(Phonon::VideoWidget *v);
    void play();
    void play(QString);
    void play(Music *);
    void pause();
    void next();
    void previous();
    bool isFirstLaunch();
    void saveMusicLibrary();
    void saveVideoLibrary();
    void savePlaylists();
    void addVideo(QString);
    void removeVideo(QString);
    void addPlaylist(Playlist *p);
    void removePlaylist(Playlist *p);
    QList<QString> videos();
    bool hasVideoAppearance();
    void setVideoAppearance(bool);
    void loadPlugins();

private:
    Core();
    QSettings *m_settings;
    QHash<QString, Artist *> *m_artistHash;
    Phonon::AudioOutput *audioOutput;
    Phonon::MediaObject *media;
    Playlist *m_playlist;
    QFileSystemWatcher *m_tagsWatcher;
    QList<Playlist *> m_playlistList;
    QList<ContentPlugin *> m_contentPlugList;
    QList<QString> m_videoURLList;
    InfoThread *m_it;
    bool m_videoAppearance;
    Music *m_currentMusic;

public slots:
    void setPosition(int);
    void setVolume(int);

private slots:
    void changeLength();
    void hasVideoChanged(bool);
    void changeTags();
    void finished();
    void tick(qint64);
    void stateChanged(Phonon::State state,Phonon::State);
    void currentIndexChanged();
    void updateFile(QString);

signals:
    void lengthChanged();
    void isPlayingChanged();
    void videoAppearanceChanged(bool);
    void mainColorChanged();
    void currentPlaylistChanged();
    void currentMusicChanged();
    void volumeChanged();
    void posChanged(int);
    void tagsChanged();
    void videoRemoved(QString);
    void videoAdded(QString);
};

#endif // CORE_H
