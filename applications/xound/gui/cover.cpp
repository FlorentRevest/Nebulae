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

#include "include/cover.h"

// Contains some code of the Phinix media player

Cover::Cover(QWidget *parent) : QLabel(parent)
{
    setPixmap(QPixmap(":/Default_Cover.png"));
    connect(&m_http, SIGNAL(done(bool)), this, SLOT(xmlDownloadFinished(bool)));
    connect(&m_httpCover, SIGNAL(done(bool)), this, SLOT(showCover(bool)));
    buffer = new QBuffer(this);
    file = new QFile(this);
    setScaledContents(true);
}

void Cover::search(Album *a) // This method is called on each music change, it requests to lastfm to send us a xml file which contains
                             // some information about the album
{
    QString artist = a->artist()->name();
    QString album = a->name();

    m_http.abort();
    m_httpCover.abort();
    setPixmap(QPixmap(":/Default_Cover.png"));

    QUrl URL = QUrl();
    URL.setPath("/2.0/");
    URL.addQueryItem("api_key", "c835c474036ea8b05414e337fc589a46");
    URL.addQueryItem("method", "album.getinfo");

    URL.addQueryItem("artist", artist.replace(' ', "%20"));
    URL.addQueryItem("album", album.replace(' ', "%20"));

    m_http.setHost("ws.audioscrobbler.com");
    m_http.get(URL.toString(), buffer);
}

void Cover::xmlDownloadFinished(bool error) // When the xml file is downloaded we must parse
{
    if(!error)
    {
        buffer->close();
        QDomDocument doc;
        if(!buffer->open(QIODevice::ReadOnly))
            return;

        doc.setContent(buffer);
        buffer->close();
        QDomElement docElem = doc.documentElement();
        if(docElem.tagName() == "lfm")
        {
            if(docElem.attribute("status") == "ok")
            {
                if(docElem.firstChild().isElement())
                {
                    QDomElement elt = docElem.firstChild().toElement();
                    if(elt.tagName() == "album")
                    {
                        QUrl url = QUrl(parsedAlbumInfos(elt));
                        QFileInfo fileInfo(url.path());
                        QString fileName = QDir::tempPath() + QDir::separator() + fileInfo.fileName();

                        if(QFile::exists(fileName))
                            QFile::remove(fileName);

                        file->setFileName(fileName);
                        if(file->open(QIODevice::WriteOnly))
                        {
                            m_httpCover.setHost(url.host());

                            QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
                            if (path.isEmpty())
                                path = "/";

                            m_httpCover.get(path, file); // We download the cover in the temporary file directory
                            file->close();
                        }
                    }
                    else if(elt.tagName() == "track")
                    {
                        QUrl url = QUrl(parsedAlbumInfos(docElem.firstChild().firstChildElement("album")));
                        QFileInfo fileInfo(url.path());
                        QString fileName = QDir::tempPath() + QDir::separator() + fileInfo.fileName();

                        if (QFile::exists(fileName))
                            QFile::remove(fileName);

                        file->setFileName(fileName);
                        if (file->open(QIODevice::WriteOnly))
                        {
                            m_httpCover.setHost(url.host());

                            QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
                            if (path.isEmpty())
                                path = "/";

                            m_httpCover.get(path, file); // We download the cover in the temporary file directory
                            file->close();
                        }
                    }
                }
            }
        }
    }
    else
        setPixmap(QPixmap(":/Default_Cover.png"));
}

QString Cover::parsedAlbumInfos(QDomElement albumNode) // It returns the URL of the cover
{
    for(QDomNode n = albumNode.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement t = n.toElement();
        if((t.tagName() == "image") && t.hasAttribute("size"))
        {
            if(t.attribute("size") == "large")
                return(t.text());
        }
    }
    return("");
}

void Cover::showCover(bool error) //And we finaly show the cover :)
{
    if(!error)
    {
        file->close();
        if(file->exists())
        {
            if(!QPixmap(file->fileName()).isNull())
                setPixmap(QPixmap(file->fileName()));
            else
                setPixmap(QPixmap(":/Default_Cover.png"));
        }
        else
            setPixmap(QPixmap(":/Default_Cover.png"));
    }
    else
        setPixmap(QPixmap(":/Default_Cover.png"));
}
