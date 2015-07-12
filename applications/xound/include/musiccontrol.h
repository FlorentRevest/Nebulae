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

#ifndef MUSICCONTROL_H
#define MUSICCONTROL_H

#include <QWidget>
#include <QSlider>
#include <QPainter>
#include <QTime>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QMouseEvent>

#include "include/core.h"
#include "include/cover.h"

class MusicControl : public QWidget
{
Q_OBJECT
public:
    MusicControl(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

private:
    QSlider *m_seekSlider, *m_volumeSlider;
    QPushButton *m_randButton, *m_repeatButton, *m_previousButton, *m_playButton,  *m_nextButton, *m_volumeIcon, *m_fullScreenButton;
    QLabel *m_musicTagsLabel, *m_timeLabel;
    Core *m_c;
    QPoint m_Diff;
    Cover *m_cover;

private slots:
    void moveWidgets();
    void play();
    void previous();
    void next();
    void mute();
    void updateTime();
    void showTags();
    void musicChanged();
    void isPlayingChanged();
};

#endif // MUSICCONTROL_H
