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

#ifndef PROGRAMDEFAULTSEDITOR_H
#define PROGRAMDEFAULTSEDITOR_H

#include <QtGui/QWidget>

class QLabel;
class QCheckBox;
class QTreeWidget;
class DesktopEntry;
class ProgramDefaultsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ProgramDefaultsEditor(DesktopEntry *program, QWidget *parent = 0);

private slots:
    // the slots are called when one of the bottom buttons are pressed
    void m_save_button_pressed();
    void m_cancel_button_pressed();
    void m_select_all_toggled(bool);

signals:
    void finished();

private:
    // decorators, explications, etc.
    QLabel *m_title_label, *m_subtitle_label, *m_icon_label, *m_name_label;

    // the main display
    QCheckBox *m_select_all;
    QTreeWidget *m_list;
};

#endif // PROGRAMDEFAULTSEDITOR_H
