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

#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include <QtGui/QFont>
#include <QtGui/QLineEdit>

// this is juste a QLineEdit reimplementation to provide a function whent the line edit //
// is focused in and out, and then change the text //

class SearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SearchLineEdit(QWidget *parent = 0) : QLineEdit(parent) {
        setText(tr("Find..."));
        QFont f;
        f.setItalic(true);
        setFont(f);

        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

protected:
    void focusInEvent(QFocusEvent *ev) {
        if(text() == tr("Find...")) {
            setText("");
            QFont f;
            f.setItalic(false);
            setFont(f);
        }
        QWidget::focusInEvent(ev);
    }
    void focusOutEvent(QFocusEvent *ev) {
        if(text() == "") {
            setText(tr("Find..."));
            QFont f;
            f.setItalic(true);
            setFont(f);
        }
        QWidget::focusOutEvent(ev);
    }
};

#endif // SEARCHLINEEDIT_H
