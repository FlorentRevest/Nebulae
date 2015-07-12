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

#include "include/musiclibrary.h"

MusicLibrary::MusicLibrary(QWidget *parent) : QWidget(parent)
{
    m_c = Core::instance();
    m_aw = 0;
    m_itemHash = new QHash<QListWidgetItem *, Artist *>();
    m_searchLine = new QLineEdit(this);
    m_searchLine->setPlaceholderText(tr("Search"));
    m_searchLine->setToolTip(tr("Search in this playlist"));
    connect(m_searchLine, SIGNAL(textChanged(QString)), this, SLOT(search(QString)));

    m_addButton = new QPushButton(QIcon(":/Add.png"), "", this);
    m_addButton->setGeometry(width() - 64, 0, 32, 32);
    m_addButton->setIconSize(QSize(31, 31));
    m_addButton->setFlat(true);
    m_addButton->setToolTip(tr("Add a file to the library"));
    connect(m_addButton, SIGNAL(clicked()), this, SLOT(add()));

    m_addFolderButton = new QPushButton(QIcon(":/Add_Folder.png"), "", this);
    m_addFolderButton->setGeometry(width() - 32, 0, 32, 32);
    m_addFolderButton->setIconSize(QSize(31, 31));
    m_addFolderButton->setFlat(true);
    m_addFolderButton->setToolTip(tr("Add all the musics of a folder to the library"));
    connect(m_addFolderButton, SIGNAL(clicked()), this, SLOT(addFolder()));

    m_infoLabel = new QLabel(this);
    m_infoLabel->setText("<center><img src=\":/Music.png\" /><b><font size=\"+1\">Xound - Your new music player</font></b></center>"
                         "<br /><br />"
                         "Your musics library is currently empty:<br />"
                         "    - You can add some musics by clicking on the <img src=\":/Add.png\" width=32 height=32 />  button above");
    if(m_c->artists().count() > 0)
        m_infoLabel->hide();

    m_artistList = new QListWidget(this);
    m_artistList->setSortingEnabled(true);
    m_artistList->setGridSize(QSize(128, 146));
    m_artistList->setIconSize(QSize(126, 126));
    m_artistList->setStyleSheet("QListWidget{background-color: transparent; border:none}");
    m_artistList->setViewMode(QListView::IconMode);
    m_artistList->setResizeMode(QListView::Adjust);
    m_artistList->setMovement(QListWidget::Static);
    connect(m_artistList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(showArtistWidget(QListWidgetItem*)));

    foreach(Artist *art, m_c->artists())
    {
        QListWidgetItem *i = new QListWidgetItem(QIcon(art->photo()), art->name(), m_artistList);
        m_artistList->addItem(i);
        m_itemHash->insert(i, art);
    }
}

void MusicLibrary::showArtistWidget(QListWidgetItem *i)
{
    m_aw = new ArtistWidget(m_itemHash->value(i), this);
    m_aw->resize(size());
    m_aw->show();
    connect(m_aw, SIGNAL(closed()), this, SLOT(artistWidgetClosed()));
}

void MusicLibrary::artistWidgetClosed()
{
    m_aw = 0;
}

void MusicLibrary::resizeEvent(QResizeEvent *)
{
    m_searchLine->move(0, 2);
    m_addButton->setGeometry(width() - 64, 0, 32, 32);
    m_addFolderButton->setGeometry(width() - 32, 0, 32, 32);
    m_artistList->setGeometry(0, 32, width(), height() - 32);
    m_infoLabel->setGeometry(0, 32, width(), height() - 32);
    if(m_aw != 0)
        m_aw->resize(size());
}

void MusicLibrary::add()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Add musics"), QDesktopServices::storageLocation(QDesktopServices::MusicLocation), tr("Music Files (*.mp3 *.ogg *.m4v *.mp4)"));
    foreach(QString file, files)
        m_c->addMedia(file);
}

void MusicLibrary::addFolder()
{
    QDirIterator it(QFileDialog::getExistingDirectory(this), QStringList() << "*.avi" << "*.flv" << "*.m4v" << "*.mp4", QDir::NoFilter, QDirIterator::Subdirectories);
    while(it.hasNext())
        m_c->addMedia(it.next());
}

void MusicLibrary::search(QString)
{

}
