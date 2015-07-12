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

#ifndef WALLPAPERCHOOSER_H
#define WALLPAPERCHOOSER_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>
#include <QPainter>
#include <QSettings>
#include <QFileDialog>
#include <QColorDialog>
#include <QThread>

enum Style { // This struct represents a type of display for the wallpaper
    Zoomed,
    Centered,
    Resized,
    Enlarged
};

class WallpaperChooser : public QWidget
{
Q_OBJECT
public:
    WallpaperChooser(QWidget *parent = 0);

public slots:
    void addWall();
    void removeWall();
    void wallSelected(QListWidgetItem *);
    void colorSelected();
    void styleSelected(int);
    void emitHide();

signals:
    void changeWallpaper(QString);
    void changeColor(QColor);
    void changeStyle(Style);
    void hideWallChooser();

private:
    QColor mainColor;
    QGridLayout *layout;
    QLabel *infol;
    QPushButton *addb, *removeb, *okb, *colorb;
    QComboBox *stylecb;
    QListWidget *wallpaperList;
    QPixmap *m_cache;
    QSettings *m_settings;
    QStringList m_URLList;
    QHash<QListWidgetItem *, QString> *m_itemHash;

protected:
    void paintEvent(QPaintEvent *);
};


#endif // WALLPAPERCHOOSER_H
