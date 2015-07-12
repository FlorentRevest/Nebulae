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

#include "desktop.h"

Desktop::Desktop(QWidget *parent) : QWidget(parent) // This class is the top widget of the application, it manages the wallpaper and loads other class
{
    setAttribute(Qt::WA_X11NetWmWindowTypeDesktop, true);
    setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());
    settings = new QSettings("Desktop", "Desktop", this);
    m_s = Style(settings->value("Style", Zoomed).toInt());
    QPalette pal;
    pal.setColor(QPalette::Window, settings->value("BackgroundColor", QColor(Qt::black)).value<QColor>());
    setPalette(pal);

    label = new QLabel(this);
    label->resize(size());
    label->setAlignment(Qt::AlignCenter);
    wallURL = settings->value("Wallpaper").toString();
    showWall(wallURL);

    m_panel = new Panel(this);
    m_panel->setGeometry(0, 0, width(), 32);

    m_shutdownButton = new ShutdownButton(this);
    m_shutdownButton->setGeometry(30, height() - 50, 150, 30);

    m_searchBar = new SearchBar(this);
    m_searchBar->setGeometry(width() - 330, height() - 62, 300, 42);

    menu = new QMenu(this);
    menu->addAction(QIcon::fromTheme("desktop"), tr("Change the wallpaper"), this, SLOT(showWallChooser()));

    wallc = new WallpaperChooser(this);
    connect(wallc, SIGNAL(changeStyle(Style)), this, SLOT(changeStyle(Style)));
    connect(wallc, SIGNAL(changeColor(QColor)), this, SLOT(changeColor(QColor)));
    connect(wallc, SIGNAL(changeWallpaper(QString)), this, SLOT(showWall(QString)));
    connect(wallc, SIGNAL(hideWallChooser()), this, SLOT(hideWallChooser()));
    wallc->setGeometry(0, height() - 120, width(), 120);
    wallc->hide();

    m_menuContainer = new MenuContainer(this);
    m_menuContainer->hide();
    m_menuContainer->setGeometry(10, 138, width() - 20, height() - 200);

    CategorieButton *b = new CategorieButton(QPixmap("/usr/share/icons/Faenza/categories/48/applications-multimedia.png"), "Multimedia", this);
    b->move(1180, 38);
    connect(b, SIGNAL(hover()), m_menuContainer, SLOT(showItems()));
    connect(b, SIGNAL(leave()), m_menuContainer, SLOT(hide()));
    CategorieButton *b2 = new CategorieButton(QPixmap("/usr/share/icons/Faenza/categories/48/applications-internet.png"), "Internet", this);
    b2->move(1280, 38);

}

void Desktop::contextMenuEvent(QContextMenuEvent *event) // Called when the user does a right click on the desktop, it shows the menu
{
    menu->exec(event->globalPos());
}

void Desktop::showWall(QString u) // Updates the label with the corect style and URL
{
    QPixmap pix(u);
    switch(m_s)
    {
    case Zoomed:
        label->setScaledContents(false);
        if(pix.height() > pix.width())
            label->setPixmap(pix.scaledToHeight(height(), Qt::SmoothTransformation));
        if(pix.width() > pix.height())
            label->setPixmap(pix.scaledToWidth(width(), Qt::SmoothTransformation));
        break;

    case Centered:
        label->setScaledContents(false);
        label->setPixmap(pix);
        break;

    case Resized:
        if(pix.height() > pix.width())
            label->setPixmap(pix.scaledToWidth(width(), Qt::SmoothTransformation));
        if(pix.width() > pix.height())
            label->setPixmap(pix.scaledToHeight(height(), Qt::SmoothTransformation));
        label->setScaledContents(false);
        break;

    case Enlarged:
        label->setScaledContents(true);
        label->setPixmap(pix);
        break;
    }
    wallURL = u;
    settings->setValue("Wallpaper", wallURL);
}

void Desktop::showWallChooser() // If you choose "Change the wallpaper", it hides the shutdownButton and the searchBar and shows the WallpaperChooser
{
    wallc->show();
    m_shutdownButton->hide();
    m_searchBar->hide();
}

void Desktop::hideWallChooser() // When you click on "Apply", it hides the WallpaperChooser and shows the shutdownButton and the searchBar
{
    m_shutdownButton->show();
    m_searchBar->show();
    wallc->hide();
}

void Desktop::changeStyle(Style s) // Saves the style and updates the wallpaper
{
    m_s = s;
    settings->setValue("Style", int(m_s));
    showWall(wallURL); // Updates the wallpaper
}

void Desktop::changeColor(QColor c) // Saves the background's color and updates the wallpaper
{
    QPalette pal;
    pal.setColor(QPalette::Window, c);
    setPalette(pal);
    settings->setValue("BackgroundColor", c);
}
