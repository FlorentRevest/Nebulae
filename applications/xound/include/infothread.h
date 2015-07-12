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

#ifndef INFOTHREAD_H
#define INFOTHREAD_H

#include <QThread>
#include <QPixmap>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QFile>
#include <QBuffer>
#include <QDesktopServices>

#include "include/artist.h"

class Core;

class InfoThread : public QThread
{
Q_OBJECT
public:
    InfoThread();
    void getNextPhoto();

protected:
    void run();

public slots:
    void photoXmlDownloaded();
    void photoDownloaded();
    void photoError(QNetworkReply::NetworkError);

signals:
    void artistPhotoReady(Artist *);

private:
    QList<Artist *> artists;
    int i;
    Artist *m_art;
};

#endif // INFOTHREAD_H
