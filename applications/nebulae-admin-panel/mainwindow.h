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

#include <QtCore/QHash>
#include <QtGui/QMainWindow>

#include "nebulae-desktopentry.h"

class QLabel;
class QLineEdit;
class QListWidget;
class QListWidgetItem;

class Category {

public:
    Category(QString title, QStringList fdesktop_cat, QIcon icon) : m_title(title), m_fdesktop_cat(fdesktop_cat), m_icon(icon) {}

    void addEntry(DesktopEntry entry) { m_entries << entry; }
    void clearEntries()          { m_entries.clear();  }
    QString title() { return m_title; }
    QIcon   icon()  { return m_icon;  }
    QStringList fdesktop_cat() { return m_fdesktop_cat; }
    QList<DesktopEntry> entries()      { return m_entries;      }

private:
    QString m_title;            // localized title of the category
    QStringList m_fdesktop_cat; // the freedesktop-defined categories corresponding
    QIcon m_icon;               // an icon to display in the side bar
    QList<DesktopEntry> m_entries;      // .desktop files
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void m_cat_clicked();                      // called on a click on a category icon, on the side bar
    void m_item_clicked(QListWidgetItem*);     // called on a click on an entry in the main display
    void m_home_clicked();                     // called on a click on the home action
    void m_search(QString);                    // called when the search bar text changes

private:
    // widgets //

    // view
    QListWidget *m_display;

    // toolbars
    QLabel *m_title_label;
    QLineEdit *m_search_bar;
    QAction *m_home_act;

    // internal //

    // categories
    QList<Category *> m_cats;
    Category *m_virtual_search_cat; // virtual vat that contains search results

    // exec values of the entries
    QHash<QListWidgetItem*, QString> m_exec_values;

    // indicate of we are currently at the home page
    bool m_at_home;

    void m_display_category(Category*);
    void m_init_display();
    void m_populate_toolbars();
    void m_populate_categories();
};

#endif // MAINWINDOW_H
