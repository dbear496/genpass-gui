/* ---------------------------------------------------------------------- *\
 * src/PasswordPropsWidget.cpp
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

#include "PasswordPropsWidget.hpp"
#include "ui_PasswordPropsWidget.h"

#include <genpass/Genpass.hpp>
#include <genpass/Password.hpp>
#include <QStyleHints>

#include "GenpassWindow.hpp"

PasswordPropsWidget::PasswordPropsWidget(genpass::Genpass& genpass) :
  QWidget(), genpass(genpass), ui(new Ui::PasswordPropsWidget())
{
  ui->setupUi(this);

  QStringList algos;
  for(auto it = genpass.algorithms_begin();
    it != genpass.algorithms_end(); ++it
  ) {
    algos.push_back(it->c_str());
  }
  algos.sort();
  ui->selectAlgo->addItems(algos);

  const QChar pwMaskChar = qApp->styleHints()->passwordMaskCharacter();
  ui->password->setPlaceholderText(QString(pwMaskChar).repeated(16));
}

PasswordPropsWidget::~PasswordPropsWidget() { }

void
PasswordPropsWidget::setPassword(genpass::Password *pw) {
  if(pw == currentPw) return;
  currentPw = pw;
  if(!currentPw) setEditing(false);

  ui->id->setText(currentPw ? currentPw->id.c_str() : "");
  ui->id->setEnabled(currentPw);
  ui->id->setReadOnly(!editMode);

  ui->spinSerial->setValue(currentPw ? currentPw->serial : 0);
  ui->spinSerial->setEnabled(editMode);

  std::string algorithm = currentPw ? currentPw->algorithmName() : "";
  ui->selectAlgo->setCurrentText(algorithm.c_str());
  ui->selectAlgo->setEnabled(editMode);

  ui->password->setText("");

  ui->note->setPlainText(currentPw ? currentPw->note.c_str() : "");
  ui->note->setEnabled(currentPw);
  ui->note->setReadOnly(!editMode);

  passwordChanged(currentPw);
}

void
PasswordPropsWidget::setEditing(bool edit) {
  if(edit == editMode) return;
  editMode = edit;

  editModeChanged(editMode);
}
