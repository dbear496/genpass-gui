/* ---------------------------------------------------------------------- *\
 * src/GenpassWindow.hpp
 * This file is part of GenPass-GUI.
 *
 * Copyright (C) 2026      David Bears <dbear4q@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
\* ---------------------------------------------------------------------- */

#ifndef __GENPASSGUI_GENPASSWINDOW_HPP__
#define __GENPASSGUI_GENPASSWINDOW_HPP__

#include <genpass/Genpass.hpp>
#include <QItemSelection>
#include <QMainWindow>

#include "PasswordListModel.hpp"
#include "PasswordPropsWidget.hpp"

class PasswordPropsWidget;
namespace Ui {
  class GenpassWindow;
}

class GenpassWindow : public QMainWindow {
  Q_OBJECT

public:
  GenpassWindow(genpass::Genpass& genpass);
  virtual ~GenpassWindow();

  void updatePasswordSelection(
    const QItemSelection &newSelection,
    const QItemSelection &oldSelection
  );

private:
  genpass::Genpass& genpass;

  PasswordListModel pwListModel;
  PasswordPropsWidget *pwProps;

  const std::unique_ptr<Ui::GenpassWindow> ui;
};

#endif // __GENPASSGUI_GENPASSWINDOW_HPP__
