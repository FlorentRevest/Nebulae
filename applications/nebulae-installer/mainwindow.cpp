/*
 * This file is part of Nebulae.
 * Copyright (C) 2011 - Leo Testard <leo.testard@gmail.com>
 *
 * Nebulae is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nebulae is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nebulae. If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "langpage.h"
#include "partitionpage.h"
#include "infomanager.h"

#include <QtGui/QFont>
#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>
#include <QtGui/QPaintEvent>

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // the class InfoManager will store all the options
    m_info_mgr = new InfoManager(this);

    // initialize display widgets
    m_welcome = new QLabel(tr("<strong>Welcome to Nebulae!</strong>"), this);
    m_welcome->setBackgroundRole(QPalette::Dark);
    m_welcome->setAlignment(Qt::AlignCenter);
    QFont f;
    f.setPointSize(32);
    m_welcome->setFont(f);
    m_lang_page = new LangPage(m_info_mgr, this);
    m_partition_page = new PartitionPage(m_info_mgr, this);

    // show the first page
    m_page = m_lang_page;
    m_partition_page->hide();

    // the bottom buttons area
    m_buttons_area = new QWidget(this);
    m_buttons_layout = new QHBoxLayout(m_buttons_area);
    m_next = new QPushButton(tr("Next step"), this);
    m_quit = new QPushButton(tr("Quit"), this);
    m_back = new QPushButton(tr("Previous "), this);
    m_install = new QPushButton(tr("Start install"), this);
    m_buttons_layout->addWidget(m_quit, 0, Qt::AlignLeft);
    m_buttons_layout->addWidget(m_next, 0, Qt::AlignRight);
    m_buttons_area->setLayout(m_buttons_layout);
    m_back->hide();
    m_install->hide();

    connect(m_quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(m_back, SIGNAL(clicked()), this, SLOT(m_disp_lang()));
    connect(m_next, SIGNAL(clicked()), this, SLOT(m_disp_partition()));
    connect(m_install, SIGNAL(clicked()), this, SLOT(m_start_install()));

    //showFullScreen();
    resize(1280,800);
    //resize(800,600);
}

MainWindow::~MainWindow()
{

}


void MainWindow::paintEvent(QPaintEvent*)
{
    // repaints the entire window with a gradient

    if(m_page == m_lang_page)
    {
        // paint the full window in black, like the coverflow
        QPainter painter(this);
        painter.fillRect(QRect(QPoint(0,0),size()), Qt::black);
    }
    else
    {
        // use a circular gradient of grays
        QRadialGradient gradient(width()/2, height(), width()/2, width()/2, height() + 100);
        gradient.setColorAt(0, QColor(200, 200, 200));
        gradient.setColorAt(1, QColor(90, 90, 90));

        // apply it to the full window
        QPainter painter(this);
        painter.fillRect(QRect(QPoint(0, 0), size()), gradient);
    }
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    if(!(m_page == m_lang_page))
    {
        // resize the widgets
        m_page->resize(width(), height() - 50);
        m_buttons_area->resize(width(), 50);

        // move the widgets
        m_page->move(0,0);
        m_buttons_area->move(0, height() - 50);
    }
    else
    {
        // the lang page uses a supplementary label
        m_welcome->resize(width(), 50);
        m_welcome->move(0,0);

        // resize the widgets
        m_page->resize(width(), height() - 100);
        m_buttons_area->resize(width(), 50);

        // move the widgets
        m_page->move(0, 50);
        m_buttons_area->move(0, height()-50);
    }
}

// displays back the lang page
void MainWindow::m_disp_lang()
{
    // displays the lang page
    m_page = m_lang_page;
    m_partition_page->hide();
    m_welcome->show();
    m_lang_page->show();

    // update the buttons
    m_quit->show();
    m_next->show();
    m_back->hide();
    m_install->hide();
    m_buttons_layout->removeWidget(m_back);
    m_buttons_layout->removeWidget(m_install);
    m_buttons_layout->addWidget(m_quit, 0, Qt::AlignLeft);
    m_buttons_layout->addWidget(m_next, 0, Qt::AlignRight);

    m_buttons_area->repaint();
}

// write the config of the first page and displays the next one
void MainWindow::m_disp_partition()
{
    // write conf
    m_lang_page->writeConfig();
    m_info_mgr->displayDebugInfo();

    // display the new page
    m_page = m_partition_page;
    m_lang_page->hide();
    m_welcome->hide();
    m_partition_page->show();

    // update the bottom area
    m_quit->hide();
    m_next->hide();
    m_back->show();
    m_install->show();
    m_buttons_layout->removeWidget(m_quit);
    m_buttons_layout->removeWidget(m_next);
    m_buttons_layout->addWidget(m_back, 0, Qt::AlignLeft);
    m_buttons_layout->addWidget(m_install, 0, Qt::AlignRight);

    m_buttons_area->repaint();

    // resize the widgets
    m_page->resize(width(), height() - 50);
    m_buttons_area->resize(width(), 50);

    // move the widgets
    m_page->move(0,0);
    m_buttons_area->move(0, height() - 50);
}

void MainWindow::m_start_install()
{

}
