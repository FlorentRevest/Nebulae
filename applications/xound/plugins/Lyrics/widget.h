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

#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>
#include <QtNetwork>
#include <fileref.h>
#include <tag.h>
#include "../../include/contentplugin.h"

class LyricsPlugin : public ContentPlugin
{
    Q_OBJECT
    Q_INTERFACES(ContentPlugin)

public:
    LyricsPlugin();
    virtual QString name() const;
    virtual QIcon icon() const;
    virtual void setMusicPath(QString);

public slots:
    void showLyrics(QNetworkReply *reply);

private:
    QNetworkReply *m_requestReply;
    QString m_title, m_artist;
    QTextEdit *textBrowser;
    TagLib::FileRef *f;
    QNetworkAccessManager *m_http;
};

#endif // WIDGET_H
