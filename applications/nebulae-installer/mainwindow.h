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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class InfoManager;
class LangPage;
class PartitionPage;
class QLabel;
class QPushButton;
class QHBoxLayout;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void m_disp_lang();
    void m_disp_partition();
    void m_start_install();

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

private:
    // list of all the widgets of the installer
    QWidget *m_page, *m_buttons_area;
    QHBoxLayout *m_buttons_layout;
    LangPage *m_lang_page;
    PartitionPage *m_partition_page;
    InfoManager *m_info_mgr;
    QPushButton *m_quit, *m_next, *m_back, *m_install;
    QLabel *m_welcome;
};

#endif // MAINWINDOW_H
