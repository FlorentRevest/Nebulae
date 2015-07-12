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

#include "include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    m_c = Core::instance();
    m_c->loadPlugins();
    connect(m_c, SIGNAL(isPlayingChanged()), this, SLOT(changeIconState()));
    connect(m_c, SIGNAL(posChanged(int)), this, SLOT(changeIconPercent(int)));
    connect(m_c, SIGNAL(mainColorChanged()), this, SLOT(repaint()));
    setWindowTitle(QString("Xound - What's a life without music?"));
    setWindowIcon(QIcon(QString(":/Icon.png")));
    setMinimumSize(600, 400);
    setMouseTracking(true);
    setGeometry(m_c->geometry());

    m_mc = new MusicControl(this);
    connect(m_c, SIGNAL(videoAppearanceChanged(bool)), this, SLOT(videoAppearanceChanged(bool))); // It has to be called after the creation of m_mc
    m_wz = new WorkZone(this);

    m_video = new Video(this);
    m_video->hide();
    m_c->setVideoWidget(m_video);

    m_sizeGrip = new QSizeGrip(this);
    m_sizeGrip->setGeometry(width() - 32, height() - 32, 32, 32);
    m_sizeGrip->hide();

    m_menu = new QMenu("Xound", this);
    m_menu->setTitle("Xound");
    m_menu->addAction(QIcon(":/Systray_Player_Play.png"), tr("Play/Pause"), m_mc, SLOT(play()));
    m_menu->addAction(QIcon(":/Systray_Maximize.png"), tr("Show"), this, SLOT(show()));
    m_menu->addAction(QIcon(":/Systray_Close.png"), tr("Quit"), this, SLOT(quit()));
    m_menu->setIcon(QIcon(QString(":/Icon.png")));

    m_icon = new QSystemTrayIcon(QIcon(":/Icon.png"), this);
    m_icon->setVisible(true);
    m_icon->setContextMenu(m_menu);
    connect(m_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(systrayActivities(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::videoAppearanceChanged(bool showVideo)
{
    if(showVideo)
    {
        m_sizeGrip->show();
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
        m_video->setGeometry(0, m_mc->height(), width(), height() - m_mc->height());
        m_wz->hide();
        m_video->show();
        m_sizeGrip->setGeometry(width() - 15, height() - 15, 15, 15);
    }
    else
    {
        m_sizeGrip->show();
        setWindowFlags(Qt::Window);
        m_wz->setGeometry(0, m_mc->height(), width(), height() - m_mc->height());
        m_wz->show();
    }
    show();
}

void MainWindow::paintEvent(QPaintEvent *) // Draws the background of the window
{
    QPainter p(this);
    p.setBrush(QBrush(m_c->mainColor()));
    p.setPen(m_c->mainColor());
    p.drawRect(QRect(0, 0, width(), height()));
}

void MainWindow::resizeEvent(QResizeEvent *) // When the window is resized, resizes the widgets
{
    m_mc->resize(width(), m_mc->height());
    if(m_c->hasVideoAppearance())
    {
        m_video->setGeometry(0, m_mc->height(), width(), height() - m_mc->height());
        m_video->show();
        m_sizeGrip->setGeometry(width() - 15, height() - 15, 15, 15);
    }
    else
        m_wz->setGeometry(0, m_mc->height(), width(), height() - m_mc->height());
}

void MainWindow::quit()
{
    m_c->setGeometry(geometry());
    m_c->setSplitterGeometry(m_wz->saveState());
    m_c->saveVideoLibrary();
    m_c->saveMusicLibrary();
    m_c->savePlaylists();
    qApp->quit();
}

void MainWindow::closeEvent(QCloseEvent *e) // Called when the user closes the window, we just hide the window (the Xound's icon is in the systray !)
{
    hide();
    e->ignore();
}

void MainWindow::systrayActivities(QSystemTrayIcon::ActivationReason reason) // If the user click on the systray icon, we show the window
{
    if(reason != QSystemTrayIcon::Context)
        setVisible(!isVisible());
}

void MainWindow::changeIconPercent(int actualPos) // Just a killer feature! Shows the advencement of the current media on the systray icon
{
    if(percent != actualPos/(m_c->length()/100)) //Don't refresh if it's the same percentage
    {
        percent = actualPos/(m_c->length()/100);

        if(percent == 100)
            m_icon->setIcon(QIcon(":/Systray_Icon_Play100.png"));
        else if(percent >= 90)
            m_icon->setIcon(QIcon(":/Systray_Icon_Play90.png"));
        else if(percent >= 80)
            m_icon->setIcon(QIcon(":/Systray_Icon_Play80.png"));
        else if(percent >= 70)
            m_icon->setIcon(QIcon(":/Systray_Icon_Play70.png"));
        else if(percent >= 60)
            m_icon->setIcon(QIcon(":/Systray_Icon_Play60.png"));
        else if(percent >= 50)
            m_icon->setIcon(QIcon(":/Systray_Icon_Play50.png"));
        else if(percent >= 40)
            m_icon->setIcon(QIcon(":/Systray_Icon_Play40.png"));
        else if(percent >= 30)
            m_icon->setIcon(QIcon(":/Systray_Icon_Play30.png"));
        else if(percent >= 20)
            m_icon->setIcon(QIcon(":/Systray_Icon_Play20.png"));
        else if(percent >= 10)
            m_icon->setIcon(QIcon(":/Systray_Icon_Play10.png"));
        else
            m_icon->setIcon(QIcon(":/Systray_Icon_Play.png"));
    }
}

void MainWindow::changeIconState() // Show the state of the current media on the systray icon
{
    if(m_c->isPlaying())
        m_icon->setIcon(QIcon(":/Systray_Icon_Play.png"));
    else
        m_icon->setIcon(QIcon(":/Icon.png"));
}
