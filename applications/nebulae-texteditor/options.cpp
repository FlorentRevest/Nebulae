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

void MainWindow::hideToolbar(bool hide)
{
    set->beginGroup("Toolbar");
    tBar->setHidden(hide);
    if(hide)
        set->setValue("Enabled", 0);
    else
        set->setValue("Enabled", 1);
    set->endGroup();
}

void MainWindow::fixToolbar(bool fix)
{
    set->beginGroup("Toolbar");
    tBar->setMovable(!fix);
    if(fix)
        set->setValue("Fixed", 1);
    else
        set->setValue("Fixed", 0);
    set->endGroup();
}
