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

#include "include/playlistwidget.h"

PlaylistWidget::PlaylistWidget(Playlist *playl, QWidget *parent) : QWidget(parent) // This widget allows the user to do some actions on the playlist
{
    m_playlist = playl;

    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setSortingEnabled(true);
    m_tableWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
    m_tableWidget->setDragEnabled(true);
    m_tableWidget->viewport()->setAcceptDrops(true);
    m_tableWidget->setDropIndicatorShown(true);
    m_tableWidget->setDragDropMode(QAbstractItemView::InternalMove);
    m_tableWidget->setShowGrid(false);
    m_tableWidget->verticalHeader()->hide();
    m_tableWidget->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    m_tableWidget->setColumnCount(5);
    m_tableWidget->setToolTip(tr("Lists of all the files of the current playlist"));
    connect(m_tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(changeCurrentMusic(QTableWidgetItem*)));
    connect(m_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(changeCurrentRow()));

    m_searchLine = new QLineEdit(this);
    m_searchLine->setPlaceholderText(tr("Search"));
    m_searchLine->setToolTip(tr("Search in this playlist"));
    connect(m_searchLine, SIGNAL(textChanged(QString)), this, SLOT(search(QString)));

    m_removeButton = new QPushButton(QIcon(":/Remove.png"), "", this);
    m_removeButton->setGeometry(width() - 32, 0, 32, 32);
    m_removeButton->setIconSize(QSize(31, 31));
    m_removeButton->setFlat(true);
    m_removeButton->setToolTip(tr("Remove the selected music of this playlist"));
    connect(m_removeButton, SIGNAL(clicked()), this, SLOT(remove()));

    connect(m_playlist, SIGNAL(currentIndexChanged()), this, SLOT(changeCurrentIndex()));
    connect(m_playlist, SIGNAL(musicAdded(Music *)), this, SLOT(addMusic(Music *)));
    connect(m_playlist, SIGNAL(musicRemoved(int)), this, SLOT(removeRow(int)));
    connect(m_playlist, SIGNAL(refresh()), this, SLOT(refresh()));

    refresh();
    m_tableWidget->setCurrentCell(0, 0);
}

void PlaylistWidget::resizeEvent(QResizeEvent *) // When the playlist widget is resized, moves all the widgets
{
    m_tableWidget->setGeometry(0, 32, width(), height() - 32);
    m_tableWidget->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    m_tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Title") << tr("Artist") << tr("Album") << tr("Style") << tr("Note"));
    m_removeButton->setGeometry(width() - 32, 0, 32, 32);
    m_searchLine->move(0, 2);
}

void PlaylistWidget::changeCurrentIndex() // When the playlist emits the currentIndexChanged, we change the position in the widget
{
    if(m_searchLine->text() == QString::null) // Only if there isn't any filter
        m_tableWidget->setCurrentCell(m_playlist->currentIndex(), 0);
}

void PlaylistWidget::changeCurrentMusic(QTableWidgetItem *item)
{
    if(m_c->currentPlaylist() != m_playlist)
        m_c->setPlaylist(m_playlist);
    m_playlist->setCurrentIndex(item->row());
}

void PlaylistWidget::addMusic(Music *m) // This method adds a music to the table widget
{
    m_tableWidget->setRowCount(m_tableWidget->rowCount() + 1);
    m_tableWidget->horizontalHeader()->resizeSections(QHeaderView::Stretch);

    QTableWidgetItem *titleItem = new QTableWidgetItem(m->title());
    titleItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    if(m->title().isNull())
        titleItem->setText(QFileInfo(m->URL()).baseName());
    else
        titleItem->setText(m->title());

    QTableWidgetItem *artistItem = new QTableWidgetItem(m->artist()->name());
    artistItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem *albumItem = new QTableWidgetItem(m->album()->name());
    albumItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem *styleItem = new QTableWidgetItem(m->genre());
    styleItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem *noteItem = new QTableWidgetItem(); //We have to create a null item if we want to put a widget in
    noteItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    MusicNote *note = new MusicNote(m);
    note->resize(0, 0);

    m_tableWidget->setItem(m_tableWidget->rowCount() - 1, 0, titleItem);
    m_tableWidget->setItem(m_tableWidget->rowCount() - 1, 1, artistItem);
    m_tableWidget->setItem(m_tableWidget->rowCount() - 1, 2, albumItem);
    m_tableWidget->setItem(m_tableWidget->rowCount() - 1, 3, styleItem);
    m_tableWidget->setItem(m_tableWidget->rowCount() - 1, 4, noteItem);
    m_tableWidget->setCellWidget(m_tableWidget->rowCount() - 1, 4, note);
}

void PlaylistWidget::search(QString s)
{
    for(int i = 0 ; i < m_playlist->musicList().count() ; i ++)
    {
        Music *m = m_playlist->musicList().at(i);
        if(m->title().contains(s, Qt::CaseInsensitive) || m->artist()->name().contains(s, Qt::CaseInsensitive) || m->album()->name().contains(s, Qt::CaseInsensitive))
            m_tableWidget->showRow(i);
        else
            m_tableWidget->hideRow(i);
    }
}

void PlaylistWidget::refresh() // Recreate all the playlist
{
    m_tableWidget->clear();
    m_tableWidget->setRowCount(0);
    m_tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Title") << tr("Artist") << tr("Album") << tr("Style") << tr("Note"));
    foreach(Music *m, m_playlist->musicList())
        addMusic(m);
}

void PlaylistWidget::changeCurrentRow() // Select a row
{
    m_tableWidget->item(m_tableWidget->currentRow(), 0)->setSelected(true);
    m_tableWidget->item(m_tableWidget->currentRow(), 1)->setSelected(true);
    m_tableWidget->item(m_tableWidget->currentRow(), 2)->setSelected(true);
    m_tableWidget->item(m_tableWidget->currentRow(), 3)->setSelected(true);
    m_tableWidget->item(m_tableWidget->currentRow(), 4)->setSelected(true);
}

void PlaylistWidget::remove() // Simply remove the selected URL
{
    m_playlist->removeMusicAt(m_tableWidget->currentRow());
}

void PlaylistWidget::removeRow(int row) // If a music is deleted, delete a row in the playlist widget
{
    m_tableWidget->removeRow(row);
}

Playlist *PlaylistWidget::playlist()
{
   return m_playlist;
}
