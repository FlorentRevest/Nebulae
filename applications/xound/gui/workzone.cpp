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

#include "include/workzone.h"

WorkZone::WorkZone(QWidget *parent) : QSplitter(parent)
{
    m_c = Core::instance();
    QWidget *menuWidget = new QWidget();
    QGridLayout *layout = new QGridLayout(menuWidget);
    layout->setSpacing(3);
    layout->setMargin(0);

    m_treeMenu = new QTreeWidget();
    m_treeMenu->setHeaderHidden(true);
    m_treeMenu->setAnimated(true);
    m_treeMenu->setStyleSheet("QTreeView{background-color: transparent; border:none;}"
                              "QTreeView::branch:has-children:!has-siblings:closed, QTreeView::branch:closed:has-children:has-siblings {"
                              "        border-image: none;"
                              "        image: url(:/Branch_Closed.png);}"
                              "QTreeView::branch:open:has-children:!has-siblings, QTreeView::branch:open:has-children:has-siblings {"
                              "        border-image: none;"
                              "        image: url(:/Branch_Open.png);}");

    m_widgetHash = new QHash<QTreeWidgetItem *, QWidget *>();

    QTreeWidgetItem *library = new QTreeWidgetItem(m_treeMenu);
    library->setText(0, tr("Library"));
    library->setExpanded(true);
    library->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
    m_treeMenu->addTopLevelItem(library);
    connect(m_treeMenu, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemIsClicked(QTreeWidgetItem*,int)));

    musicLibraryItem = new QTreeWidgetItem(library);
    musicLibraryItem->setText(0, tr("Musics"));
    musicLibraryItem->setIcon(0, QIcon(":/Music.png"));
    m_treeMenu->setCurrentIndex(m_treeMenu->indexAt(QPoint(0, 1)));
    m_widget = m_ml =  new MusicLibrary();
    m_widgetHash->insert(musicLibraryItem, m_widget);

    videoLibraryItem = new QTreeWidgetItem(library);
    videoLibraryItem->setText(0, tr("Videos"));
    videoLibraryItem->setIcon(0, QIcon(":/Video.png"));
    m_vl =  new VideoLibrary();
    m_widgetHash->insert(videoLibraryItem, m_vl);

    playlist = new QTreeWidgetItem(m_treeMenu);
    playlist->setText(0, tr("Playlist"));
    playlist->setExpanded(true);
    playlist->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
    m_treeMenu->addTopLevelItem(playlist);

    addWidget(menuWidget);
    addWidget(m_widget);

    mainWidgetSP = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mainWidgetSP.setHorizontalStretch(1);
    m_widget->setSizePolicy(mainWidgetSP);
    foreach(Playlist *pl, m_c->playlists())
    {
        QTreeWidgetItem *playlistItem = new QTreeWidgetItem();
        m_widgetHash->insert(playlistItem, new PlaylistWidget(pl));
        playlistItem->setText(0, pl->name());
        playlistItem->setIcon(0, QIcon(":/Playlist_Icon.png"));
        playlistItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);

        playlist->addChild(playlistItem);
    }

    foreach(ContentPlugin *cp, m_c->contentPlugins())
    {
        QTreeWidgetItem *plugItem = new QTreeWidgetItem();
        m_widgetHash->insert(plugItem, cp);
        plugItem->setText(0, cp->name());

        if(!cp->icon().isNull())
            plugItem->setIcon(0, cp->icon());

        m_treeMenu->addTopLevelItem(plugItem);
    }

    layout->addWidget(m_treeMenu, 1, 0, 1, 6);

    m_addButton = new QPushButton(QIcon(":/Add.png"), "", this);
    m_addButton->setFlat(true);
    m_addButton->setMaximumSize(32, 32);
    m_addButton->setIconSize(QSize(32, 32));
    m_addButton->setToolTip(tr("Add a playlist"));
    connect(m_addButton, SIGNAL(clicked()), this, SLOT(add()));
    layout->addWidget(m_addButton, 0, 1);

    m_openButton = new QPushButton(QIcon(":/Open_Playlist.png"), "", this);
    m_openButton->setFlat(true);
    m_openButton->setMaximumSize(32, 32);
    m_openButton->setToolTip(tr("Import a playlist from a file"));
    m_openButton->setIconSize(QSize(32, 32));
    connect(m_openButton, SIGNAL(clicked()), this, SLOT(open()));
    layout->addWidget(m_openButton, 0, 2);

    m_saveButton = new QPushButton(QIcon(":/Save_Playlist.png"), "", this);
    m_saveButton->setFlat(true);
    m_saveButton->setMaximumSize(32, 32);
    m_saveButton->setToolTip(tr("Save a playlist in a file"));
    m_saveButton->setIconSize(QSize(32, 32));
    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(save()));
    layout->addWidget(m_saveButton, 0, 3);

    m_removeButton = new QPushButton(QIcon(":/Remove.png"), "", this);
    m_removeButton->setFlat(true);
    m_removeButton->setMaximumSize(32, 32);
    m_removeButton->setToolTip(tr("Remove the selected playlist"));
    m_removeButton->setIconSize(QSize(32, 32));
    connect(m_removeButton, SIGNAL(clicked()), this, SLOT(remove()));
    layout->addWidget(m_removeButton, 0, 4);

    restoreState(m_c->splitterGeometry());
}

void WorkZone::itemIsClicked(QTreeWidgetItem* item, int) // Shows a plugin or a playlist
{
    if(m_widgetHash->contains(item))
    {
        QList<int> s = sizes();
        m_widget->setParent(0);
        m_widget->hide();

        m_widget = m_widgetHash->value(item);
        addWidget(m_widget);
        m_widget->setSizePolicy(mainWidgetSP);
        m_widget->show();
        setSizes(s);
    }
}

void WorkZone::add() // Adds a playlist
{
    QString name = QInputDialog::getText(0, tr("Add a playlist"), tr("What name do you want to set to this new playlist?"));
    if(name != QString::Null())
    {
        Playlist *playl = new Playlist(name);

        QTreeWidgetItem *playlItem = new QTreeWidgetItem();
        playlItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        playlItem->setText(0, name);
        playlItem->setIcon(0, QIcon(":/Playlist_Icon.png"));
        playlist->addChild(playlItem);

        m_c->addPlaylist(playl);
        m_widgetHash->insert(playlItem, new PlaylistWidget(playl));
    }
}

void WorkZone::remove() // Removes a playlist
{
    PlaylistWidget *pw = qobject_cast<PlaylistWidget *>(m_widget);
    if(pw != 0)
    {
        QList<int> s = sizes();

        m_c->removePlaylist(pw->playlist());
        playlist->removeChild(m_widgetHash->key(pw));
        delete pw;
        m_treeMenu->itemAt(QPoint(0, 1))->setSelected(true);

        m_widget = m_ml;
        addWidget(m_widget);
        m_widget->setSizePolicy(mainWidgetSP);
        m_widget->show();
        setSizes(s);
    }
}

void WorkZone::save() // Asks the user where he wants to save the playlist
{
    m_c->currentPlaylist()->save(QFileDialog::getSaveFileName(0, tr("Save the playlist"), QDesktopServices::storageLocation(QDesktopServices::HomeLocation)));
}

void WorkZone::open() // Creates a new playlist, and loads a playlist file
{
    QString URL = QFileDialog::getOpenFileName(0, tr("Add a playlist"), QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
    if(URL != QString::Null())
    {
        Playlist *playl = new Playlist(tr("Untitled Playlist"));
        playl->load(URL);

        QTreeWidgetItem *playlItem = new QTreeWidgetItem();
        playlItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        playlItem->setText(0, playl->name());
        playlItem->setIcon(0, QIcon(":/Playlist_Icon.png"));
        playlist->addChild(playlItem);

        m_c->addPlaylist(playl);
        m_widgetHash->insert(playlItem, new PlaylistWidget(playl));
    }
}
