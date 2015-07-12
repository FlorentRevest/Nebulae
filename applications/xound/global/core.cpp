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

#include "include/core.h"

Core::Core()
{
    m_settings = new QSettings("Xound", "Xound");
    media = new Phonon::MediaObject(this);
    media->setTickInterval(1000);
    connect(media, SIGNAL(totalTimeChanged(qint64)), this, SLOT(changeLength()));
    connect(media, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
    connect(media, SIGNAL(metaDataChanged()), this, SLOT(changeTags()));
    connect(media, SIGNAL(hasVideoChanged(bool)), SLOT(hasVideoChanged(bool)));
    connect(media, SIGNAL(finished()), SLOT(finished()));
    connect(media, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    audioOutput->setVolume(50);
    audioOutput->setName("Xound");
    media->stop();
    Phonon::createPath(media, audioOutput);
    m_videoAppearance = false;
    m_playlist = NULL;

    m_artistHash = new QHash<QString, Artist *>();
    m_tagsWatcher = new QFileSystemWatcher();
    connect(m_tagsWatcher, SIGNAL(fileChanged(QString)), this, SLOT(updateFile(QString)));

    // Loads the music library
    QDomDocument *doc = new QDomDocument();
    QFile file(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.config/Xound/MusicLibrary.xml");
    if(file.open(QIODevice::ReadOnly))
    {
        doc->setContent(&file);
        file.close();

        QDomNode root = doc->documentElement();
        for(int i = 0 ; i < root.childNodes().count() ; i ++)
        {
            QDomNode artistNode = root.childNodes().at(i);
            QDomElement artistElem = artistNode.toElement();
            Artist *art = artist(artistElem.attribute("name"));
            for(int j = 0; j < artistNode.childNodes().count(); j ++)
            {
                QDomNode albumNode = artistNode.childNodes().at(j);
                QDomElement albumElem = albumNode.toElement();
                Album *alb = art->album(albumElem.attribute("name"));
                for(int k = 0; k < albumNode.childNodes().count(); k ++)
                {
                    QDomNode musicNode = albumNode.childNodes().at(k);
                    QDomElement musicElem = musicNode.toElement();
                    Music *m = new Music(musicElem.attribute("URL"), alb);
                    if(QFileInfo(m->URL()).exists())
                    {
                        m->setTitle(musicElem.attribute("title"));
                        m->setGenre(musicElem.attribute("genre"));
                        m->setDescription(musicElem.attribute("description"));
                        m->setDate(musicElem.attribute("date").toInt());
                        m->setTrackNumber(musicElem.attribute("trackNumber").toUInt());
                        m_tagsWatcher->addPath(m->URL());
                    }
                    else
                        delete m;
                }
            }
        }
    }
    delete doc;

    // Loads the video Library
    file.setFileName(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.config/Xound/VideoLibrary");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream flux(&file);
        while(!flux.atEnd())
            m_videoURLList << flux.readLine();
        file.close();
    }

    // Loads the playlists
    doc = new QDomDocument();
    file.setFileName(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.config/Xound/Playlists.xml");
    if(file.open(QIODevice::ReadOnly))
    {
        doc->setContent(&file);
        file.close();

        QDomNode root = doc->documentElement();
        for(int i = 0 ; i < root.childNodes().count() ; i ++)
        {
            QDomNode playlistNode = root.childNodes().at(i);
            Playlist *pl = new Playlist(playlistNode.toElement().attribute("name"));
            for(int j = 0; j < playlistNode.childNodes().count(); j ++)
            {
                QDomNode musicNode = playlistNode.childNodes().at(j);
                QDomElement musicElem = musicNode.toElement();
                foreach(Music *m, artist(musicElem.attribute("artist"))->album(musicElem.attribute("album"))->musics())
                    if(m->title() == musicElem.attribute("title"))
                        pl->addMusic(m);
            }
            m_playlistList.append(pl);
        }
    }
    delete doc;

    m_it = new InfoThread();
    m_it->start(QThread::IdlePriority);
}

Core *Core::instance()
{
    static Core *c = new Core();
    return c;
}

void Core::loadPlugins()    // Loads the content plugins
{
    //    QDir plugDir = QDir(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.config/Xound/plugins");
    QDir plugDir = QDir(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/Programmation/nebulae/applications/xound/");

    foreach(QString file, plugDir.entryList(QStringList("*.so")))
    {
        QPluginLoader loader(plugDir.absoluteFilePath(file));
        if(QObject *pluginObject = loader.instance())
            m_contentPlugList.append(qobject_cast<ContentPlugin *>(pluginObject));
        else qWarning(tr("Plugin %1 can't be loaded: %2").arg(file).arg(loader.errorString()).toStdString().c_str());
    }
    plugDir.setPath("/usr/lib/Xound/plugins");
    foreach(QString file, plugDir.entryList(QStringList("*.so")))
    {
        QPluginLoader loader(plugDir.absoluteFilePath(file));
        if(QObject *pluginObject = loader.instance())
            m_contentPlugList.append(qobject_cast<ContentPlugin *>(pluginObject));
        else qWarning(tr("Plugin %1 can't be loaded: %2").arg(file).arg(loader.errorString()).toStdString().c_str());
    }
}

double Core::version()
{
    return 0.1;
}

QString Core::currentURL()
{
    return media->currentSource().fileName();
}

Music *Core::currentMusic()
{
    return m_currentMusic;
}

int Core::volume()
{
    return audioOutput->volume();
}

void Core::setVolume(int newV)
{
    audioOutput->setVolume((qreal)newV / 100);
    emit(volumeChanged());
}

void Core::play()
{
    media->play();
}

void Core::play(QString url)
{
    media->setCurrentSource(url);
    m_currentMusic = 0;
    emit(currentMusicChanged());
    media->play();
}

void Core::play(Music *m)
{
    media->setCurrentSource(m->URL());
    m_currentMusic = m;
    emit(currentMusicChanged());
    media->play();
}

void Core::pause()
{
    media->pause();
}

bool Core::isPlaying()
{
    if(media->state() == Phonon::PlayingState)
        return true;
    else
        return false;
}

bool Core::isMuted()
{
    return audioOutput->isMuted();
}

void Core::setMuted(bool b)
{
    audioOutput->setMuted(b);
}

void Core::previous()
{
    if(m_playlist != 0)
       m_playlist->previous();
}

void Core::next()
{
    if(m_playlist != 0)
        m_playlist->next();
}

void Core::random()
{
    m_playlist->randomMusic();
}

int Core::length()
{
    return media->totalTime();
}

int Core::position()
{
    return media->currentTime();
}

void Core::setPosition(int p)
{
    return media->seek(p);
}

void Core::hasVideoChanged(bool b)
{
    setVideoAppearance(b);
}

bool Core::hasVideo()
{
    return media->hasVideo();
}

QMap<QString, QString> Core::metaData()
{
    return media->metaData();
}

void Core::setPlaylist(Playlist *p)
{
    m_playlist = p;
    connect(m_playlist, SIGNAL(currentIndexChanged()), this, SLOT(currentIndexChanged()));
}

Playlist *Core::currentPlaylist()
{
    return m_playlist;
}

QList<Playlist *> Core::playlists()
{
    return m_playlistList;
}

void Core::removePlaylist(Playlist *p)
{
    m_playlistList.removeOne(p);
}

void Core::addPlaylist(Playlist *p)
{
    m_playlistList.append(p);
}

void Core::setMainColor(QColor c)
{
    m_settings->setValue("mainColor", c);
    emit mainColorChanged();
}

QColor Core::mainColor()
{
    return m_settings->value("mainColor", QColor(210, 210, 210)).value<QColor>();
}

void Core::setSplitterGeometry(QByteArray ba)
{
    m_settings->setValue("splitterGeometry", ba);
}

QByteArray Core::splitterGeometry()
{
    return m_settings->value("splitterGeometry").toByteArray();
}

uint Core::note(QString URL)
{
    return m_settings->value("Note/" + URL).toUInt();
}

bool Core::isFirstLaunch()
{
    if(m_settings->value("FirstLaunch", true).toBool() == true)
    {
        m_settings->setValue("FirstLaunch", false);
        return true;
    }
    return false;
}

void Core::setNote(QString URL, uint newNote)
{
    m_settings->setValue("Note/" + URL, newNote);
}

QRect Core::geometry()
{
    return m_settings->value("geometry", QRect(qApp->desktop()->width()/2 - 300, qApp->desktop()->width()/2 - 195, 600, 400)).toRect();
}

void Core::setGeometry(QRect r)
{
    m_settings->setValue("geometry", r);
}

Artist *Core::artist(QString name)
{
    Artist *art = m_artistHash->value(name);
    if(art == 0) //If the artist doesn't already exists
    {
        art = new Artist();
        art->setName(name);
        m_artistHash->insert(name, art);
    }
    return art;
}

QList<Artist *> Core::artists()
{
    return m_artistHash->values();
}

Music *Core::addMedia(QString URL)
{
    TagLib::FileRef f(URL.toLocal8Bit());
    Artist *art;
    Album *alb;
    if(f.tag() != 0)
    {
        if(!f.tag()->artist().isNull())       // If the artist is known
            art = artist(f.tag()->artist().toCString());
        else
            art = artist(tr("Unknown Artist"));

        if(!f.tag()->album().isNull())       // If his album is known
            alb = art->album(f.tag()->album().toCString());
        else
            alb = art->album(tr("Unknown Album"));
    }
    else
    {
        art = artist(tr("Unknown Artist"));
        alb = art->album(tr("Unknown Album"));
    }
    Music *m = new Music(URL, alb);
    m->setTitle(QString(f.tag()->title().toCString()));
    m->setGenre(QString(f.tag()->genre().toCString()));
    m->setDescription(QString(f.tag()->comment().toCString()));
    m->setDate(f.tag()->year());
    m->setTrackNumber(f.tag()->track());
    m_tagsWatcher->addPath(URL);
    return m;
}

void Core::setVideoWidget(Phonon::VideoWidget *v)
{
    Phonon::createPath(media, v);
}

void Core::tick(qint64 t)
{
    emit posChanged(t);
}

void Core::finished()
{
//    emit finished();
}

void Core::changeTags()
{
    emit tagsChanged();
}

void Core::changeLength()
{
    emit lengthChanged();
}

void Core::stateChanged(Phonon::State state,Phonon::State)
{
    if(state == Phonon::PlayingState)
        emit isPlayingChanged();
    else if(state == Phonon::StoppedState || state == Phonon::PausedState)
        emit isPlayingChanged();
}

void Core::saveMusicLibrary()
{
    QDomDocument *doc = new QDomDocument();
    foreach(Artist *art, artists())
    {
        QDomElement artistElem = doc->createElement("artist");
        artistElem.setAttribute("name", art->name());
        foreach(Album *alb, art->albums())
        {
            QDomElement albumElem = doc->createElement("album");
            albumElem.setAttribute("name", alb->name());
            foreach(Music *music, alb->musics())
            {
                QDomElement musicElem = doc->createElement("music");
                musicElem.setAttribute("title", music->title());
                musicElem.setAttribute("genre", music->genre());
                musicElem.setAttribute("description", music->description());
                musicElem.setAttribute("URL", music->URL());
                musicElem.setAttribute("date", music->date());
                musicElem.setAttribute("trackNumber", music->trackNumber());
                albumElem.appendChild(musicElem);
            }
            artistElem.appendChild(albumElem);
        }
        doc->appendChild(artistElem);
    }

    QFile file(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.config/Xound/MusicLibrary.xml");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream << "<library>\n" << doc->toString() << "</library>";
    file.close();
    delete doc;
}

void Core::saveVideoLibrary()
{
    QFile file(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.config/Xound/VideoLibrary");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    foreach(QString url, m_videoURLList)
        stream << url << "\n";
    file.close();
}

void Core::savePlaylists()
{
    QDomDocument *doc = new QDomDocument();
    foreach(Playlist *p, m_playlistList)
    {
        QDomElement playlistElem = doc->createElement("playlist");
        playlistElem.setAttribute("name", p->name());
        foreach(Music *music, p->musicList())
        {
            QDomElement musicElem = doc->createElement("music");
            musicElem.setAttribute("artist", music->artist()->name());
            musicElem.setAttribute("album", music->album()->name());
            musicElem.setAttribute("title", music->title());
            playlistElem.appendChild(musicElem);
        }
        doc->appendChild(playlistElem);
    }

    QFile file(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.config/Xound/Playlists.xml");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream << "<playlists>\n" << doc->toString() << "</playlists>";
    file.close();
    delete doc;

}

void Core::addVideo(QString u)
{
    m_videoURLList << u;
    emit videoAdded(u);
}

void Core::removeVideo(QString u)
{
    m_videoURLList.removeOne(u);
    emit videoRemoved(u);
}

QList<ContentPlugin *> Core::contentPlugins()
{
    return m_contentPlugList;
}

QList<QString> Core::videos()
{
    return m_videoURLList;
}

void Core::setVideoAppearance(bool showVideo)
{
    m_videoAppearance = showVideo;
    emit videoAppearanceChanged(m_videoAppearance);
}

bool Core::hasVideoAppearance()
{
    return m_videoAppearance;
}

void Core::currentIndexChanged()
{
    play(m_playlist->currentMusic());
}

void Core::updateFile(QString url)
{
    addMedia(url);
}
