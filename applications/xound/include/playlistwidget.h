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

#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QStringListModel>
#include <QHeaderView>
#include <QFileDialog>
#include <QInputDialog>
#include <QDesktopServices>

#include "core.h"
#include "playlist.h"
#include "musicnote.h"

class PlaylistWidget : public QWidget
{
Q_OBJECT
public:
    PlaylistWidget(Playlist *, QWidget *parent = 0);
    Playlist *playlist();

private:
    QTableWidget *m_tableWidget;
    QLineEdit *m_searchLine;
    QPushButton *m_removeButton;
    Playlist *m_playlist;
    Core *m_c;

protected:
    void resizeEvent(QResizeEvent *);

public slots:
    void remove();
    void removeRow(int);
    void changeCurrentMusic(QTableWidgetItem*);
    void changeCurrentIndex();
    void search(QString);
    void refresh();
    void changeCurrentRow();

private slots:
    void addMusic(Music *);
};

#endif // PLAYLISTWIDGET_H
