/* ------------------------------------------------------------------------ *\
 * src/GenpassWindow.cpp
 * This file is part of GenPass-GUI.
 *
 * Copyright (C) 2026      David Bears <dbear4q@gmail.com>
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

#include "GenpassWindow.hpp"

// IWYU pragma: no_include <QtCore>
#include <genpass/Genpass.hpp>      // for Genpass
#include <genpass/Password.hpp>     // for Password
#include <QAbstractButton>          // for QAbstractButton
#include <QItemSelection>           // for QItemSelection
#include <QItemSelectionModel>      // for QItemSelectionModel
#include <QListView>                // for QListView
#include <QModelIndexList>          // for QModelIndexList, QTypeInfo<>::isR...
#include <QPushButton>              // for QPushButton
#include <QScrollArea>              // for QScrollArea
#include <QWidget>                  // for QWidget
#include <functional>               // for bind
#include <string>                   // for operator==, basic_string, string

#include "PasswordPropsWidget.hpp"  // for PasswordPropsWidget
#include "ui_GenpassWindow.h"       // for GenpassWindow

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
  QObject::connect(
    pwProps, &PasswordPropsWidget::passwordChanged,
    this, &GenpassWindow::updatePassword
  );
  QObject::connect(
    pwProps, &PasswordPropsWidget::editModeChanged,
    this, &GenpassWindow::updateEditing
  );
  QObject::connect(
    pwProps, &PasswordPropsWidget::editsCommitted,
    this, std::bind(&PasswordListModel::refresh, &pwListModel)
  );
  QObject::connect(
    pwProps, &PasswordPropsWidget::editsCommitted,
    ui->buttonApply, std::bind(&QWidget::setEnabled, ui->buttonApply, false)
  );
  QObject::connect(
    pwProps, &PasswordPropsWidget::propertyEdited,
    ui->buttonApply, std::bind(&QWidget::setEnabled, ui->buttonApply, true)
  );
  QObject::connect(
    ui->buttonApply, &QAbstractButton::clicked,
    pwProps, [this](){
      pwProps->commitEdits();
      pwProps->setEditing(false);
    }
  );
  QObject::connect(
    ui->buttonCancel, &QAbstractButton::clicked,
    pwProps, [this](){ pwProps->setEditing(false); }
  );
  QObject::connect(
    ui->buttonEditPw, &QAbstractButton::clicked,
    pwProps, &PasswordPropsWidget::setEditing
  );
  QObject::connect(
    ui->buttonAddPw, &QAbstractButton::clicked,
    pwProps, [this](bool checked){
      if(checked)
        pwProps->setPassword(nullptr);
      pwProps->setEditing(checked);
    }
  );
  QObject::connect(
    ui->buttonRemovePw, &QAbstractButton::clicked,
    pwProps, &PasswordPropsWidget::deletePassword
  );

  updateEditing(pwProps->isEditing());
  updatePassword(pwProps->getPassword());
}

GenpassWindow::~GenpassWindow() { }

void
GenpassWindow::updatePasswordSelection(
  const QItemSelection &newSelection
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

void
GenpassWindow::updatePassword(genpass::Password *newPw) {
  QItemSelectionModel *selector = ui->idList->selectionModel();
  const QModelIndexList oldSelection = selector->selectedIndexes();
  if(!newPw) {
    selector->clearSelection();
  }
  else if(oldSelection.empty() ||
    pwListModel.get(oldSelection[0]) != newPw->id
  ) {
    selector->select(pwListModel.find(newPw->id),
      QItemSelectionModel::ClearAndSelect);
  }

  ui->buttonEditPw->setEnabled(newPw);
  ui->buttonRemovePw->setEnabled(newPw);
}

void
GenpassWindow::updateEditing(bool editing) {
  ui->buttonApply->setVisible(editing);
  ui->buttonCancel->setVisible(editing);
  if(!editing)
    ui->buttonApply->setEnabled(false);

  ui->buttonEditPw->setChecked(editing && pwProps->getPassword());
  ui->buttonAddPw->setChecked(editing && !pwProps->getPassword());
}
