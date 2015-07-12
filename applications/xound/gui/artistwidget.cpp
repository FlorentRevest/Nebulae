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

#include "include/artistwidget.h"

ArtistWidget::ArtistWidget(Artist *art, QWidget *parent) : QScrollArea(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_c = Core::instance();

    QWidget *w = new QWidget();

    QPushButton *closeButton = new QPushButton(tr("Return to artist list"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(returnToArtists()));

    m_albumLayout = new QGridLayout(w);
    m_albumLayout->setSpacing(15);
    m_albumLayout->addWidget(closeButton);

    m_albumHash = QHash<QListWidget *, Album *>();
    m_musicHash = QHash<QListWidgetItem *, Music *>();
    m_indexHash = QHash<Music *, uint>();

    m_playlist = new Playlist();
    connect(m_playlist, SIGNAL(currentIndexChanged()), this, SLOT(currentIndexChanged()));

    if(!art->albums().isEmpty())
    {
        QList<Album *> albsSorted = art->albums();
        for(int i = 1 ; i < albsSorted.count() ; i ++) // Sorts albums by dates
        {
            Album *key = albsSorted[i];
            int j = i-1;
            while(j >= 0 && albsSorted[j]->musics()[0]->date() > key->musics()[0]->date())
            {
                albsSorted[j + 1] = albsSorted[j];
                j --;
            }
            albsSorted[j + 1] = key;
        }

        int index = 0;
        foreach(Album *alb, albsSorted)
        {
            if(!alb->musics().isEmpty())
            {
                QString albStr;
                if(alb->musics()[0]->date() != 0)
                    albStr = "<h2>" + alb->name() + " - " + QString::number(alb->musics()[0]->date()) + "</h2>";
                else
                    albStr = "<h2>" + alb->name() + "</h2>";
                QLabel *l = new QLabel(albStr);
                QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
                shadowEffect->setColor(Qt::white);
                shadowEffect->setOffset(1, 2);
                l->setGraphicsEffect(shadowEffect);
                m_albumLayout->addWidget(l);

                QListWidget *lw = new QListWidget();
                connect(lw, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));
                connect(lw, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemDoubleClicked(QListWidgetItem*)));
                m_albumHash.insert(lw, alb);
                lw->setContextMenuPolicy(Qt::CustomContextMenu);
                lw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                lw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                lw->setStyleSheet("QListWidget{border: none; background-color: transparent;} QListWidget::item{height: 19}");

                QList<Music *> musicsSorted = QList<Music *>(alb->musics());
                for(int i = 1 ; i < musicsSorted.count() ; i ++) // Sorts albums by dates
                {
                    Music *key = musicsSorted[i];
                    int j = i-1;
                    while(j >= 0 && musicsSorted[j]->trackNumber() > key->trackNumber())
                    {
                        musicsSorted[j + 1] = musicsSorted[j];
                        j --;
                    }
                    musicsSorted[j + 1] = key;
                }

                foreach(Music *m, musicsSorted)
                {
                    m_musicHash.insert(new QListWidgetItem(QString::number(m->trackNumber()) + "- " + m->title(), lw), m);
                    m_playlist->addMusic(m);
                    m_indexHash.insert(m, index);
                    index++;
                }
                lw->setMinimumHeight(lw->count() * 19);
                lw->setMaximumHeight(lw->count() * 19);
                connect(lw, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu(QPoint)));
                m_albumLayout->addWidget(lw);
            }
        }
    }
    setWidget(w);
    m_currentItem = 0;
}

void ArtistWidget::resizeEvent(QResizeEvent *) // Resizes the widget when we are resized
{
    foreach(QListWidget *lw, m_albumHash.keys())
        lw->setMinimumWidth(widget()->height());
}

void ArtistWidget::currentIndexChanged()
{
    QListWidgetItem *i = m_musicHash.key(m_playlist->currentMusic());
    if(m_currentItem != 0 && i != m_currentItem)
        m_currentItem->setSelected(false);

    i->setSelected(true);
    m_currentItem = i;
}

void ArtistWidget::itemDoubleClicked(QListWidgetItem *i) // Plays the selected music
{
    m_c->setPlaylist(m_playlist);
    m_playlist->setCurrentIndex(m_indexHash.value(m_musicHash.value(i)));
}

void ArtistWidget::itemClicked(QListWidgetItem *i) // When a item is selected, unselects the old item
{
    if(m_currentItem != 0 && i != m_currentItem)
        m_currentItem->setSelected(false);

    m_currentItem = i;
}
void ArtistWidget::returnToArtists() // This is like a call to "close" but it emit the "closed" signal needed by MusicLibrary
{
    emit(closed());
    close();
}

void ArtistWidget::showMenu(QPoint) // This method creates and shows the menu
{
    QMenu *m_menu = new QMenu(this);
    QMenu *playlistsMenu = m_menu->addMenu(QIcon(":/Add.png"), tr("Add to the playlist..."));
    foreach(Playlist *p, m_c->playlists())
    {
        QAction *action = playlistsMenu->addAction(QIcon(":/Playlist_Icon.png"), p->name());
        m_playlistHash.insert(action, p);
        connect(action, SIGNAL(triggered()), this, SLOT(addToPlaylist()));
    }
    m_menu->addSeparator();

    QAction *removeAction = m_menu->addAction(QIcon(":/Remove.png"), tr("Remove from library"));
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeMusic(Music*)));

    m_menu->popup(QCursor::pos());
}

void ArtistWidget::addToPlaylist() // Add the selected music to the clicked playlist
{
    m_playlistHash.value(qobject_cast<QAction *>(QObject::sender()))->addMusic(m_musicHash.value(m_currentItem));
}
