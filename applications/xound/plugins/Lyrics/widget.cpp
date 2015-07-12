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

#include "widget.h"

// Contains some code of the qmmp media player

LyricsPlugin::LyricsPlugin()
{
    m_name = QString(tr("Lyrics").toAscii());
    QGridLayout *layout = new QGridLayout();
    layout->setMargin(0);

    textBrowser = new QTextBrowser();
    textBrowser->viewport()->setStyleSheet("background-color:transparent;");
    textBrowser->setFrameShape(QFrame::NoFrame);
    layout->addWidget(textBrowser);

    setLayout(layout);
    m_http = new QNetworkAccessManager(this);
    connect(m_http, SIGNAL(finished(QNetworkReply *)), SLOT(showLyrics(QNetworkReply *)));
    f = new TagLib::FileRef();
}

QString LyricsPlugin::name() const
{
    return m_name;
}

QIcon LyricsPlugin::icon() const
{
    return QIcon(":/lyrics.png");
}

void LyricsPlugin::setMusicPath(QString currentMusicPath)
{
    f = new TagLib::FileRef(currentMusicPath.toAscii());

    if(!f->isNull() && f->tag())
    {
        TagLib::Tag *tag = f->tag();
        m_title = QString(tag->title().toCString());
        m_artist = QString(tag->artist().toCString());
        textBrowser->setHtml("<p align=\"center\">" + tr("Receiving") + "</p>");
        QNetworkRequest request(QUrl("http://www.lyricsplugin.com/winamp03/plugin/?artist=" + m_artist +"&title=" + m_title));
        request.setRawHeader("User-Agent", QString("Xound1.0").toAscii());
        m_requestReply = m_http->get(request);
    }
}

void LyricsPlugin::showLyrics(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        textBrowser->setHtml("<p align=\"center\">" + reply->errorString() + "</p>");
        reply->deleteLater();
        return;
    }
    QString content = QString::fromUtf8(reply->readAll().constData());

    if(m_requestReply == reply)
    {
        m_requestReply = 0;
        QRegExp js_params("javascript:getContent\\(\\\'(.*)\\\', \\\'(.*)\\\', \\\'(.*)\\\', \\\'(.*)\\\'");
        js_params.setMinimal (true);
        js_params.indexIn(content);

        QUrl url = QUrl::fromEncoded(QByteArray("http://www.lyricsplugin.com/winamp03/plugin/content.php?")
                   + "artist=" + js_params.cap(1).toAscii()
                   + "&title=" + js_params.cap(2).toAscii()
                   + "&time=" + js_params.cap(3).toAscii()
                   + "&check=" + js_params.cap(4).toAscii());

        QString referer = QString("http://www.lyricsplugin.com/winamp03/plugin/?")
                          + "artist=" + js_params.cap(1)
                          + "&title=" + js_params.cap(2);

        QNetworkRequest request;
        request.setUrl(url);
        request.setRawHeader("Referer", referer.toAscii());
        textBrowser->setText(tr("Receiving"));
        m_http->get(request);
        reply->deleteLater();
        return;
    }
    QRegExp artist_regexp("<div id=\\\"artist\\\">(.*)</div>");
    artist_regexp.setMinimal(true);
    QRegExp title_regexp("<div id=\\\"title\\\">([^<]*)</div>");
    QRegExp lyrics_regexp("<div id=\\\"lyrics\\\">([^<]*)</div>");
    artist_regexp.indexIn(content);
    title_regexp.indexIn(content);
    content.replace("<br />", "[br /]");
    lyrics_regexp.indexIn(content);

    QString text = "<h2>" +artist_regexp.cap(1) + " - " + title_regexp.cap(1) + "</h2>";
    QString lyrics = lyrics_regexp.cap(1);
    lyrics.replace("[br /]", "<br>");
    qDebug(lyrics.toAscii());
    if(!lyrics.trimmed().isEmpty())
        textBrowser->setHtml("<b><p align=\"center\">" + tr("Not found") + "</p></b>");
    else
    {
        text += lyrics;
        textBrowser->setHtml("<p align=\"center\">" + text + "</p>");
    }
    reply->deleteLater();
}

Q_EXPORT_PLUGIN2(LyricsPlugin, LyricsPlugin)
