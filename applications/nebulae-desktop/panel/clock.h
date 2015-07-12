/*Copyright (C) 2011 Revest Florent <florent.revest666@gmail.com>
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

#ifndef CLOCK_H
#define CLOCK_H

#include <QLabel>
#include <QFont>
#include <QMenu>
#include <QSettings>
#include <QTimer>
#include <QDateTime>
#include <QFontDialog>
#include <QColorDialog>

class Clock : public QLabel
{
Q_OBJECT
public:
    Clock(QWidget *parent = 0);

public slots:
    void showMenu();
    void changeFont();
    void changeFontColor();

private:
    QTimer *m_timer;
    QSettings *settings;
    QFont m_font;
    QColor m_color;

public slots:
    void updateTime();
};

#endif // CLOCK_H
