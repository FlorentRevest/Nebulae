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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QDirIterator>
#include <QMenu>
#include <QDesktopServices>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QSizeGrip>

#include "include/core.h"
#include "include/musiccontrol.h"
#include "include/workzone.h"
#include "video.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private:
    Core *m_c;
    MusicControl *m_mc;
    QSystemTrayIcon *m_icon;
    QMenu *m_menu;
    WorkZone *m_wz;
    QSizeGrip *m_sizeGrip;
    int percent;
    Video *m_video;

private slots:
    void systrayActivities(QSystemTrayIcon::ActivationReason);
    void changeIconPercent(int);
    void changeIconState();
    void quit();
    void videoAppearanceChanged(bool);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
