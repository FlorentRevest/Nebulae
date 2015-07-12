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

#ifndef VIDEO_H
#define VIDEO_H

#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QSpacerItem>
#include <QSlider>
#include <QLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QToolButton>
#include <QLabel>

#include <phonon/videowidget.h>
#include <phonon/mediaobject.h>

#include "playlist.h"
#include "musiccontrol.h"

class Video : public Phonon::VideoWidget
{
Q_OBJECT
public:
    Video(QWidget *parent = 0);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);

public slots:
    void timeout();
    void fullScreen();
    void showConfWindow();
    void showMenu(const QPoint &);
    void setAspectValue(int);
    void setScaleValue(int);
    void setSaturationValue(int);
    void setContrastValue(int);
    void setHueValue(int);
    void setBrightnessValue(int);

private:
    QMenu *menu;
    QTimer *m_timer;
    QDialog *settingsWindow;
    QDialogButtonBox *buttonBox;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2, *verticalLayout, *vboxLayout;
    QHBoxLayout *hboxLayout2, *hboxLayout, *hboxLayout1, *horizontalLayout;
    QSpacerItem *spacerItem1, *spacerItem;
    QLabel *label_11, *label_10, *label_9, *label_8, *label_7, *label_6, *label_5, *label_4, *label_3, *label_2, *label,
           *crossFadeLabel3, *crossFadeLabel2, *crossFadeLabel1, *crossFadeLabel;
    QSlider *brightnessSlider, *saturationSlider, *hueSlider, *contrastSlider, *crossFadeSlider;
    QComboBox *aspectCombo, *scalemodeCombo, *deviceCombo;
    QGroupBox *groupBox, *groupBox_2;
    QComboBox *audioEffectsCombo;
    QToolButton *effectButton;
    MusicControl *m_mc;
    bool m_fullShown;
    QWidget *m_parent;
    QRect m_geometry;
};

#endif // VIDEO_H
