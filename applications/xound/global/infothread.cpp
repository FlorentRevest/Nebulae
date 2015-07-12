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

#include "include/infothread.h"
#include "include/core.h"

InfoThread::InfoThread() {}

void InfoThread::run()
{
    artists = Core::instance()->artists();
    i = -1;
    if(artists.count() != 0)
        getNextPhoto();

    exec();
}

void InfoThread::getNextPhoto()
{
    i++;
    while(i < artists.count())
    {
        Artist *art = artists[i];
        if(!QFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.cache/Xound/" + art->name() + "/photo.jpg").exists()
           && art->name() != tr("Unknown Artist"))
        {
            m_art = art;
            QUrl URL = QUrl(QString("http://ws.audioscrobbler.com/2.0/?method=artist.getimages") +
                          "&artist=" + m_art->name().replace(' ', "%20") +
                          "&api_key=c835c474036ea8b05414e337fc589a46");
            QNetworkAccessManager *m = new QNetworkAccessManager;
            QNetworkReply *r = m->get(QNetworkRequest(URL));
            connect(r, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(photoError(QNetworkReply::NetworkError)));
            connect(r, SIGNAL(finished()), this, SLOT(photoXmlDownloaded()));
            break;
        }
        i++;
    }
}

void InfoThread::photoXmlDownloaded()
{
    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
    QDomDocument doc;
    doc.setContent(r->readAll());
    r->deleteLater();
    QDomElement docElem = doc.documentElement();
    if(docElem.tagName() == "lfm")
    {
        if(docElem.attribute("status") == "ok")
        {
            if(docElem.firstChild().isElement())
            {
                QDomElement elt = docElem.firstChild().toElement();
                if(elt.tagName() == "images")
                {
                    for(QDomNode n = elt.firstChild(); !n.isNull(); n = n.nextSibling())
                    {
                        QDomElement t = n.toElement();
                        if(t.tagName() == "image")
                        {
                            for(QDomNode m = t.firstChild(); !m.isNull(); m = m.nextSibling())
                            {
                                if(m.toElement().tagName() == "sizes")
                                {
                                    for(QDomNode p = m.firstChild(); !p.isNull(); p = p.nextSibling())
                                    {
                                        if(p.toElement().tagName() == "size" && p.toElement().attribute("name") == "original" && p.toElement().attribute("width").toInt() > 126 && p.toElement().attribute("height").toInt() > 126)
                                        {
                                            QUrl url = QUrl(p.toElement().text());
                                            QString fileName = QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.cache/Xound/" + m_art->name() + "/photo.jpg";

                                            if(!QFileInfo(fileName).absoluteDir().exists())
                                                QDir().mkpath(QFileInfo(fileName).absoluteDir().path());
                                            if(QFile(fileName).exists())
                                                QFile::remove(fileName);

                                            if(url.toString() != "")
                                            {
                                                QNetworkAccessManager *m = new QNetworkAccessManager;
                                                QNetworkReply *r = m->get(QNetworkRequest(QUrl(QUrl::toPercentEncoding(url.toString(), "!$&'()*+,;=:@/"))));
                                                connect(r, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(photoError(QNetworkReply::NetworkError)));
                                                connect(r, SIGNAL(finished()), this, SLOT(photoDownloaded()));
                                                return;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    getNextPhoto();
}

void InfoThread::photoDownloaded()
{
    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
    QPixmap pix = QPixmap();
    pix.loadFromData(r->readAll());
    pix = pix.scaled(126, 126, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation).copy(0, 0, 126, 126);
    pix.save(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.cache/Xound/" + m_art->name() + "/photo.jpg");
    emit artistPhotoReady(m_art);
    r->deleteLater();
    getNextPhoto();
}

void InfoThread::photoError(QNetworkReply::NetworkError)
{
    getNextPhoto();
}
