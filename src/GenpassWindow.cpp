/* ---------------------------------------------------------------------- *\
 * src/GenpassWindow.cpp
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

#include "GenpassWindow.hpp"
#include "ui_GenpassWindow.h"

#include <genpass/Genpass.hpp>
#include <QStyleHints>

#include "PasswordPropsWidget.hpp"

GenpassWindow::GenpassWindow(genpass::Genpass& genpass) :
  QMainWindow(), genpass(genpass), pwListModel(genpass),
  ui(new Ui::GenpassWindow())
{
  ui->setupUi(this);

  ui->idList->setModel(&pwListModel);
  QObject::connect(
    ui->idList->selectionModel(), &QItemSelectionModel::selectionChanged,
    this, &GenpassWindow::updatePasswordSelection
  );

  pwProps = new PasswordPropsWidget(genpass);
  ui->passwordPropsArea->setWidget(pwProps);
}

GenpassWindow::~GenpassWindow() { }

void
GenpassWindow::updatePasswordSelection(
  const QItemSelection &newSelection,
  const QItemSelection &oldSelection
) {
  QModelIndexList selection = newSelection.indexes();
  genpass::Password *selectedPw;
  if(selection.empty())
    selectedPw = nullptr;
  else {
    const std::string& id = pwListModel.get(selection[0]);
    selectedPw = genpass.getPasswordPtr(id);
  }
  pwProps->setEditing(false);
  pwProps->setPassword(selectedPw);
}
