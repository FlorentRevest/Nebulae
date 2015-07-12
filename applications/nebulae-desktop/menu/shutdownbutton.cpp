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

#include "shutdownbutton.h"

ShutdownButton::ShutdownButton(QWidget *parent) : QToolButton(parent) // This button shows a menu which allows to shutdown or reboot or... the computer
{
    setIcon(QIcon::fromTheme("gnome-shutdown"));
    setText(tr("Shutdown"));
    setPopupMode(QToolButton::InstantPopup);
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    menu = new QMenu();
    menu->addAction(QIcon::fromTheme("gnome-shutdown"), tr("Shutdown"), this, SLOT(shutdown()));
    menu->addAction(QIcon::fromTheme("gnome-reboot"), tr("Reboot"), this, SLOT(reboot()));
    menu->addAction(QIcon::fromTheme("sleep"), tr("Sleep"), this, SLOT(sleep()));
    menu->addAction(QIcon::fromTheme("unconnect"), tr("Unconnect"), this, SLOT(unconnect()));
    setMenu(menu);
    setStyleSheet("QToolButton { border: 2px solid #333;"
    "    border-radius: 15px;"
    "    color: white;"
    "    padding: 0 8px;"
    "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(5, 5, 5, 116), stop:0.996094 rgba(89, 89, 89, 83));}");
}

void ShutdownButton::shutdown() // Stop the computer using the D-Bus interface of ConsoleKit
{
    QDBusInterface *consoleKit = new QDBusInterface("org.freedesktop.ConsoleKit", "/org/freedesktop/ConsoleKit/Manager", "org.freedesktop.ConsoleKit.Manager", QDBusConnection::systemBus());
    consoleKit->call("Stop");
}

void ShutdownButton::reboot() // Restart the computer using the D-Bus interface of ConsoleKit
{
    QDBusInterface *consoleKit = new QDBusInterface("org.freedesktop.ConsoleKit", "/org/freedesktop/ConsoleKit/Manager", "org.freedesktop.ConsoleKit.Manager", QDBusConnection::systemBus());
    consoleKit->call("Restart");
}

void ShutdownButton::sleep() // Enter the computer in sleep mode
{
}

void ShutdownButton::unconnect() // If the Desktop is the application of the gdm's session, if we quit the Desktop, we show GDM
{
    qApp->quit();
}
