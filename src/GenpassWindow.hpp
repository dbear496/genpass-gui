/* ---------------------------------------------------------------------- *\
 * src/GenpassWindow.hpp
 * This file is part of GenPass-GUI.
 *
 * Copyright (C) 2026      David Bears <dbear4q@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
\* ---------------------------------------------------------------------- */

#include <QMainWindow>
#include <genpass/Genpass.hpp>

#include "PasswordListModel.hpp"

namespace Ui {
  class GenpassWindow;
}

class GenpassWindow : public QMainWindow {
public:
  GenpassWindow(genpass::Genpass& genpass);
  virtual ~GenpassWindow();

  void updateCurrentPw();

private:
  genpass::Genpass& genpass;
  genpass::Password *currentPw = nullptr;
  PasswordListModel pwListModel;

  bool editMode = false;

  const std::unique_ptr<Ui::GenpassWindow> ui;
};
