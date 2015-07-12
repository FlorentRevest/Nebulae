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

#include "include/musicnote.h"

MusicNote::MusicNote(Music * music) : QWidget() // It's the little widget at the right of the playlist!
{
    URL = music->URL();
    note = Core::instance()->note(URL);
    isHover = false;
    setMouseTracking(true);
    setToolTip(tr("Note of this music"));
}

void MusicNote::enterEvent(QEvent *) // Just informs the widget that the mouse is currently hover this widget
{
    isHover = true;
}

void MusicNote::leaveEvent(QEvent *) // Repaints the widget, with the new note
{
    isHover = false;
    repaint();
}

void MusicNote::mousePressEvent(QMouseEvent *) // Saves the new note
{
    if(note == 1 && newNote == 1)
        note = 0;
    else
        note = newNote;
    Core::instance()->setNote(URL, note);
}

void MusicNote::mouseMoveEvent(QMouseEvent *e) // Calculates the new note
{
    newNote = e->x() / 32 + 1;
    repaint();
}

void MusicNote::paintEvent(QPaintEvent *) // This virtual protected method draws the stars
{
    QPainter painter(this);

    if(isHover)
    {
        if(newNote >= 1)
            painter.drawPixmap(0, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Normal));
        else
            painter.drawPixmap(0, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Disabled));
        if(newNote >= 2)
            painter.drawPixmap(height(), 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Normal));
        else
            painter.drawPixmap(height(), 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Disabled));
        if(newNote >= 3)
            painter.drawPixmap(height() * 2, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Normal));
        else
            painter.drawPixmap(height() * 2, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Disabled));
        if(newNote >= 4)
            painter.drawPixmap(height() * 3, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Normal));
        else
            painter.drawPixmap(height() * 3, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Disabled));
        if(newNote >= 5)
            painter.drawPixmap(height() * 4, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Normal));
        else
            painter.drawPixmap(height() * 4, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Disabled));
    }
    else
    {
        if(note >= 1)
            painter.drawPixmap(0, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Normal));
        else
            painter.drawPixmap(0, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Disabled));
        if(note >= 2)
            painter.drawPixmap(height(), 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Normal));
        else
            painter.drawPixmap(height(), 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Disabled));
        if(note >= 3)
            painter.drawPixmap(height() * 2, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Normal));
        else
            painter.drawPixmap(height() * 2, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Disabled));
        if(note >= 4)
            painter.drawPixmap(height() * 3, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Normal));
        else
            painter.drawPixmap(height() * 3, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Disabled));
        if(note == 5)
            painter.drawPixmap(height() * 4, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Normal));
        else
            painter.drawPixmap(height() * 4, 0, height(), height(), QIcon::fromTheme("star").pixmap(height(), height(), QIcon::Disabled));
    }
}
