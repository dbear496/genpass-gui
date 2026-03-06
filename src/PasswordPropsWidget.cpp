/* ---------------------------------------------------------------------- *\
 * src/PasswordPropsWidget.cpp
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

#include "PasswordPropsWidget.hpp"

#include <genpass/Genpass.hpp>       // for Genpass, operator==
#include <genpass/Password.hpp>      // for Password
#include <QApplication>              // for QApplication, qApp
#include <QChar>                     // for QChar
#include <QComboBox>                 // for QComboBox
#include <QFormLayout>               // for QFormLayout
#include <QLayoutItem>               // for QLayoutItem
#include <QLineEdit>                 // for QLineEdit
#include <QList>                     // for QList
#include <QPlainTextEdit>            // for QPlainTextEdit
#include <QSpinBox>                  // for QSpinBox
#include <QStringList>               // for QStringList
#include <QStyleHints>               // for QStyleHints
#include <stdexcept>                 // for runtime_error
#include <utility>                   // for pair
#include <vector>                    // for vector

#include "AlgorithmHandler.hpp"      // for AlgorithmProps, AlgorithmHandler
#include "ui_PasswordPropsWidget.h"  // for PasswordPropsWidget

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
  QObject::connect(
    ui->selectAlgo, &QComboBox::currentTextChanged,
    this, &PasswordPropsWidget::changeAlgorithmQ
  );

  const QChar pwMaskChar = qApp->styleHints()->passwordMaskCharacter();
  ui->password->setPlaceholderText(QString(pwMaskChar).repeated(16));
}

PasswordPropsWidget::~PasswordPropsWidget() { }

void
PasswordPropsWidget::setPassword(genpass::Password *pw) {
  if(pw == currentPw) return;
  if(editMode) commitEdits(); // TODO: handle invalid state
  currentPw = pw;

  ui->id->setText(currentPw ? currentPw->id.c_str() : "");
  ui->id->setEnabled(currentPw || editMode);

  ui->spinSerial->setValue(currentPw ? currentPw->serial : 0);

  std::string algorithm = currentPw ? currentPw->algorithmName() : "";
  ui->selectAlgo->setCurrentText(algorithm.c_str());

  ui->password->setText("");

  ui->note->setPlainText(currentPw ? currentPw->note.c_str() : "");
  ui->note->setEnabled(currentPw || editMode);

  passwordChanged(currentPw);
}

void
PasswordPropsWidget::setEditing(bool edit) {
  if(edit == editMode) return;
  if(editMode) commitEdits(); // TODO: handle invalid state
  editMode = edit;

  ui->id->setReadOnly(!editMode);
  ui->spinSerial->setEnabled(editMode);
  ui->selectAlgo->setEnabled(editMode);
  ui->note->setReadOnly(!editMode);

  algorithmProps->setEditing(editMode);

  editModeChanged(editMode);
}

void
PasswordPropsWidget::commitEdits() {
  std::string id = ui->id->text().toStdString();
  std::string algo = ui->selectAlgo->currentText().toStdString();

  if(!currentPw || algo != currentPw->algorithmName()) {
    if(currentPw) genpass.removePassword(currentPw->id);
    try { currentPw = &genpass.newPassword(algo, id); }
    catch (const std::runtime_error&) { throw; } // TODO
  }

  std::string oldId = currentPw->id;
  currentPw->id = id;
  if(id != oldId) {
    genpass.updateId(oldId);
  }

  currentPw->serial = ui->spinSerial->value();
  currentPw->note = ui->note->toPlainText().toStdString();

  algorithmProps->commitEdits(*currentPw);

  editsCommitted();
}

void
PasswordPropsWidget::changeAlgorithm(const std::string& name) {
  const AlgorithmHandler *handler = AlgorithmHandler::getAlgorithm(name);

  QFormLayout * const layout = static_cast<QFormLayout *>(this->layout());

  // remove widgets for old algorithm
  if(algorithmProps)
  for(const auto& w : algorithmProps->getWidgets()) {
    auto row = layout->takeRow(w.second);
    delete row.fieldItem;
    if(row.labelItem)
      delete row.labelItem->widget();
    delete row.labelItem;
  }

  if(!handler) {
    algorithmProps = nullptr;
    return;
  }
  algorithmProps = handler->createProps(*this);

  // add widgets for new algorithm
  int row;
  layout->getWidgetPosition(ui->selectAlgo, &row, nullptr);
  for(const auto& w : algorithmProps->getWidgets()) {
    layout->insertRow(++row, (w.first + ":").c_str(), w.second);
  }
}
