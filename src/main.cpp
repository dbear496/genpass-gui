/* ------------------------------------------------------------------------ *\
 * src/main.cpp
 * This file is part of GenPass-GUI.
 *
 * Copyright (C) 2025      David Bears <dbear4q@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
\* ------------------------------------------------------------------------ */

#include <genpass/Genpass.hpp>     // for Genpass
#include <QApplication>            // for QApplication
#include <string>                  // for basic_string

#include "GenpassWindow.hpp"       // for GenpassWindow
#include "V2AlgorithmHandler.hpp"  // for V2AlgorithmHandler

static void addTestPasswords(genpass::Genpass& genpass);

int
main(int argc, char **argv) {
  QApplication qapp(argc, argv);

  genpass::Genpass genpass;
  addTestPasswords(genpass);

  V2AlgorithmHandler v2AlgorithmHandler;

  GenpassWindow mainWindow(genpass);

  mainWindow.show();

  int qret = qapp.exec();

  return qret;
}

static void addTestPasswords(genpass::Genpass& genpass) {
  genpass.newPassword("genpass-2.0", "example.com");
  genpass.newPassword("genpass-2.0", "example-1.com");
  genpass.newPassword("genpass-2.0", "example-2.com");
}
