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

#ifndef LANGPAGE_H
#define LANGPAGE_H

#include <QtCore/QHash>

#include <QtGui/QWidget>

class TZComboBox;
class PictureFlow;
class InfoManager;
class QLabel;
class QComboBox;
class QTranslator;
class LangPage : public QWidget
{
    Q_OBJECT
public:
    LangPage(InfoManager *inf, QWidget *parent = 0);

    // when everything is done, this function is called to store the configuration
    void writeConfig();

signals:

private slots:
    // called when the user siwtch the flag
    void m_index_changed(int);

    // called when the user change one of the QComboBox
    void m_lang_changed(QString);
    void m_timezone_changed(QString, QString);

private:
    // list of all the buttons and their respective country
    QStringList m_countries;

    // main display
    PictureFlow *flow;

    // labels and buttons used to display and change the informations
    QLabel *m_country_label, *m_lang_label, *m_keyboard_label, *m_timezone_label;
    QComboBox *m_lang_box, *m_keyboard_box;
    TZComboBox *m_timezone_box;
    QWidget *m_bottom_widget;

    // class that store all options
    InfoManager *m_inf_mgr;

    // used to change the langage of the program during execution
    QTranslator *m_translator;

    // options
    QString m_locale, m_locale_name, m_kblayout, m_kblayout_name, m_timezone, m_timezone_name;

    // cache for the association locale <=> langage name e.g. <en_GB, English (GB)>
    QHash<QString, QString> m_locales;

    // used to remember the index of the items in the combo box
    QStringList m_locales_indexes;
    QStringList m_timezone_indexes;

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual bool event(QEvent *);
};

#endif // LANGPAGE_H
