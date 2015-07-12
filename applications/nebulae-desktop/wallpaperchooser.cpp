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

#include "wallpaperchooser.h"

class WallsThread : public QThread // This thread load the wallpapers in parallel to the load of
                                   // the Desktop (it accelerates the loading of the Desktop)
{
public:
    void run(QListWidget *wallList, QStringList URLList)
    {
        foreach(QString url, URLList) // It populates the listWidget
            wallList->addItem(new QListWidgetItem(QIcon(url), ""));
    }
};

WallpaperChooser::WallpaperChooser(QWidget *parent) : QWidget(parent) // This widget is shown on the bottom of the screen,
                                                                      // it allows to select a wallpaper and some other parameters
{
    mainColor = QColor(Qt::white); // Used to build the gradient with a cool color, TODO: make it changeable
    m_settings = new QSettings("Desktop", "Desktop");
    m_URLList = m_settings->value("WallpaperList").toStringList();
    m_settings->value("BackgroundColor", Qt::black).value<QColor>();

    layout = new QGridLayout(this);

    wallpaperList = new QListWidget();
    wallpaperList->setStyleSheet("QListWidget{background-color: transparent; border: none;}");
    wallpaperList->setViewMode(QListView::IconMode);
    wallpaperList->setIconSize(QSize(100, 100));
    wallpaperList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    wallpaperList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    wallpaperList->setFlow(QListWidget::TopToBottom);
    wallpaperList->setMovement(QListWidget::Static);
    wallpaperList->setResizeMode(QListWidget::Adjust);
    connect(wallpaperList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(wallSelected(QListWidgetItem*)));
    layout->addWidget(wallpaperList, 0, 0, 2, 4);
    WallsThread *thread = new WallsThread();
    thread->run(wallpaperList, m_URLList);

    addb = new QPushButton(QIcon::fromTheme("add"), tr("Add"));
    connect(addb, SIGNAL(clicked()), SLOT(addWall()));
    layout->addWidget(addb, 0, 4);
    removeb = new QPushButton(QIcon::fromTheme("remove"), tr("Remove"));
    connect(removeb, SIGNAL(clicked()), SLOT(removeWall()));
    layout->addWidget(removeb, 1, 4);

    colorb = new QPushButton(tr("Background color"));
    connect(colorb, SIGNAL(clicked()), SLOT(colorSelected()));
    layout->addWidget(colorb, 2, 0);

    stylecb = new QComboBox();
    stylecb->insertItems(0, QStringList() << tr("Zoomed") << tr("Centered") << tr("Resized") << tr("Enlarged"));
    stylecb->setCurrentIndex(m_settings->value("Style", Zoomed).toInt());
    connect(stylecb, SIGNAL(currentIndexChanged(int)), this, SLOT(styleSelected(int)));
    layout->addWidget(stylecb, 2, 1);

    layout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding), 2, 2);
    okb = new QPushButton(QIcon::fromTheme("dialog-apply"), tr("Apply"));
    connect(okb, SIGNAL(clicked()), this, SLOT(emitHide()));
    layout->addWidget(okb, 2, 4);
}

void WallpaperChooser::wallSelected(QListWidgetItem *i) // When the user click on an image, inform the desktop to show this wall
{
    emit changeWallpaper(m_URLList.at(wallpaperList->row(i)));
}

void WallpaperChooser::styleSelected(int i) // When the user change the style of the image, inform the desktop to update the wall
{
    emit changeStyle(Style(i)); //  The "i" index is automaticaly converted to the right Style struct
}

void WallpaperChooser::colorSelected() // Called by a click on colorb
{
    QColor color = QColorDialog::getColor();
    QPalette pal;
    pal.setColor(QPalette::Text, color);
    colorb->setPalette(pal);
    emit changeColor(color);
}

void WallpaperChooser::addWall() // Add a wallpaper to the list
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Add Wallpaper(s)"), "", tr("Images (*.png *.jpg)"));
    foreach(QString file, fileNames)
    {
        m_URLList.append(file);
        m_settings->setValue("WallpaperList", m_URLList);
        wallpaperList->addItem(new QListWidgetItem(QIcon(file), ""));
    }
}

void WallpaperChooser::removeWall() // Removes of the list the selected wallpaper
{
    int n = wallpaperList->currentRow();
    delete wallpaperList->takeItem(n);
    m_URLList.removeAt(n);
    m_settings->setValue("WallpaperList", m_URLList);
}

void WallpaperChooser::paintEvent(QPaintEvent *) // This methods draws the background of the widget
{
    QPainter painter(this);

    QLinearGradient grad1(0, 0, 0, height());

    QColor color = mainColor.lighter(130);
    color.setAlpha(50);
    grad1.setColorAt(0, color);

    color = mainColor.darker(130);
    color.setAlpha(120);
    grad1.setColorAt(0.5, color);

    color = mainColor.lighter(130);
    color.setAlpha(50);
    grad1.setColorAt(1, color);

    painter.fillRect(0, 0, width(), height(), grad1);
}

void WallpaperChooser::emitHide() // When the user click on "Apply", we inform the desktop to hide this widget and show shutdownButton and searchBar
{
    emit hideWallChooser();
}
