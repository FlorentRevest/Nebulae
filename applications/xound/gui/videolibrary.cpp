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

#include "include/videolibrary.h"

VideoLibrary::VideoLibrary(QWidget *parent) : QWidget(parent)
{
    m_c = Core::instance();
    m_URLHash = new QHash<QListWidgetItem *, QString>();
    m_searchLine = new QLineEdit(this);
    m_searchLine->setPlaceholderText(tr("Search"));
    m_searchLine->setToolTip(tr("Search in this playlist"));
    connect(m_searchLine, SIGNAL(textChanged(QString)), this, SLOT(search(QString)));

    m_addButton = new QPushButton(QIcon(":/Add.png"), "", this);
    m_addButton->setGeometry(width() - 64, 0, 32, 32);
    m_addButton->setIconSize(QSize(31, 31));
    m_addButton->setFlat(true);
    m_addButton->setToolTip(tr("Add a video to the library"));
    connect(m_addButton, SIGNAL(clicked()), this, SLOT(add()));

    m_addFolderButton = new QPushButton(QIcon(":/Add_Folder.png"), "", this);
    m_addFolderButton->setGeometry(width() - 32, 0, 32, 32);
    m_addFolderButton->setIconSize(QSize(31, 31));
    m_addFolderButton->setFlat(true);
    m_addFolderButton->setToolTip(tr("Add all the videos of a folder to the library"));
    connect(m_addFolderButton, SIGNAL(clicked()), this, SLOT(addFolder()));

    m_videoList = new QListWidget(this);
    m_videoList->setMovement(QListWidget::Static);
    m_videoList->setSortingEnabled(true);
    m_videoList->setGridSize(QSize(128, 146));
    m_videoList->setIconSize(QSize(126, 126));
    m_videoList->setStyleSheet("QListWidget{background-color: transparent; border:none}");
    m_videoList->setViewMode(QListView::IconMode);
    m_videoList->setResizeMode(QListView::Adjust);
    connect(m_videoList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(play(QListWidgetItem*)));

    m_thumbnailer.setThumbnailSize(126);

    foreach(QString url, m_c->videos())
    {
        QListWidgetItem *i = new QListWidgetItem(thumbnail(url), QFileInfo(url).baseName(), m_videoList);
        m_videoList->addItem(i);
        m_URLHash->insert(i, url);
    }
}

QIcon VideoLibrary::thumbnail(QString u)
{
    try
    {
        std::vector<uint8_t> pixelBuffer;
        m_thumbnailer.generateThumbnail(std::string(u.toUtf8()), Png, pixelBuffer);
        QPixmap *pix = new QPixmap();
        if (!pix->loadFromData(&pixelBuffer.front(), pixelBuffer.size(), "PNG"))
            return QIcon();

        return QIcon(*pix);
    }
    catch (std::exception& e)
    {
        return QIcon();
    }

}

void VideoLibrary::resizeEvent(QResizeEvent *)
{
    m_searchLine->move(0, 2);
    m_addButton->setGeometry(width() - 64, 0, 32, 32);
    m_addFolderButton->setGeometry(width() - 32, 0, 32, 32);
    m_videoList->setGeometry(0, 32, width(), height() - 32);
}

void VideoLibrary::add()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Add videos"), QDesktopServices::storageLocation(QDesktopServices::MoviesLocation), tr("Video Files (*.avi *.flv *.m4v *.mp4)"));
    foreach(QString file, files)
        m_c->addVideo(file);
}

void VideoLibrary::addFolder()
{
    QDirIterator it(QFileDialog::getExistingDirectory(this), QStringList() << "*.avi" << "*.flv" << "*.m4v" << "*.mp4", QDir::NoFilter, QDirIterator::Subdirectories);
    while(it.hasNext())
        m_c->addVideo(it.next());
}

void VideoLibrary::search(QString s)
{
    foreach(QString url, m_URLHash->values())
    {
        if(QFileInfo(url).baseName().contains(s, Qt::CaseInsensitive))
            m_URLHash->key(url)->setHidden(false);
        else
            m_URLHash->key(url)->setHidden(true);
    }
}

void VideoLibrary::play(QListWidgetItem *i)
{
    m_c->play(m_URLHash->value(i));
}
