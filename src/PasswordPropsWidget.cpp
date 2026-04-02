/* ------------------------------------------------------------------------ *\
 * src/PasswordPropsWidget.cpp
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

#include "PasswordPropsWidget.hpp"

#include <genpass/Genpass.hpp>       // for Genpass, operator==
#include <genpass/Password.hpp>      // for Password
#include <QApplication>              // for QApplication, qApp
#include <QChar>                     // for QChar
#include <QClipboard>                // for QClipboard
#include <QComboBox>                 // for QComboBox
#include <QFormLayout>               // for QFormLayout
#include <QLayoutItem>               // for QLayoutItem
#include <QLineEdit>                 // for QLineEdit
#include <QList>                     // for QList
#include <QPlainTextEdit>            // for QPlainTextEdit
#include <QPushButton>               // for QPushButton
#include <QSignalBlocker>            // for QSignalBlocker
#include <QSpinBox>                  // for QSpinBox
#include <QStringList>               // for QStringList
#include <QStyleHints>               // for QStyleHints
#include <QVariant>                  // for QVariant, operator==
#include <stdexcept>                 // for runtime_error
#include <utility>                   // for pair
#include <vector>                    // for vector

#include "AlgorithmHandler.hpp"      // for AlgorithmProps, AlgorithmHandler
#include "GenpassWindow.hpp"         // for GenpassWindow
#include "ui_PasswordPropsWidget.h"  // for PasswordPropsWidget

PasswordPropsWidget::PasswordPropsWidget(genpass::Genpass& genpass,
  GenpassWindow *parent
) :
  QWidget(parent), parent(parent), genpass(genpass),
  ui(new Ui::PasswordPropsWidget())
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
  ui->selectAlgo->insertItem(0, "", "UNKNOWN");
  ui->selectAlgo->setCurrentIndex(0);
  QObject::connect(
    ui->selectAlgo, &QComboBox::currentTextChanged,
    this, &PasswordPropsWidget::changeAlgorithmQ
  );

  const QChar pwMaskChar = qApp->styleHints()->passwordMaskCharacter();
  ui->password->setPlaceholderText(QString(pwMaskChar).repeated(16));

  QObject::connect(
    this, &PasswordPropsWidget::propertyEdited,
    this, [this](){ editPending = true; }
  );
  QObject::connect(
    ui->id, &QLineEdit::editingFinished,
    this, &PasswordPropsWidget::propertyEdited
  );
  QObject::connect(
    ui->spinSerial, &QSpinBox::editingFinished,
    this, &PasswordPropsWidget::propertyEdited
  );
  QObject::connect(
    ui->selectAlgo, &QComboBox::activated,
    this, &PasswordPropsWidget::propertyEdited
  );
  QObject::connect(
    ui->note, &QPlainTextEdit::textChanged,
    this, &PasswordPropsWidget::propertyEdited
  );

  QObject::connect(
    parent, &GenpassWindow::passwordGenerated,
    ui->password, [this](genpass::Password& pw, const std::string& pwStr) {
      if(&pw == currentPw)
        ui->password->setText(pwStr.c_str());
      else
        ui->password->clear();
    }
  );
  QObject::connect(
    ui->buttonShowPw, &QPushButton::clicked,
    this, [this](bool checked) {
      if(checked)
        checked = this->parent->genPassword(*currentPw);
      ui->buttonShowPw->setChecked(checked);
      ui->password->setEchoMode(
        checked ? QLineEdit::Normal : QLineEdit::Password);
    }
  );
  QObject::connect(
    ui->buttonCopyPw, &QPushButton::clicked,
    this, [this]() {
      if(const std::string *pwStr = this->parent->genPassword(*currentPw))
        QApplication::clipboard()->setText(pwStr->c_str());
    }
  );
}

PasswordPropsWidget::~PasswordPropsWidget() {
  changeAlgorithm("");
}

void
PasswordPropsWidget::setPassword(genpass::Password *pw) {
  if(pw == currentPw) return;
  currentPw = pw;
  refreshPassword();
  passwordChanged(currentPw);
}

void
PasswordPropsWidget::refreshPassword() {
  QSignalBlocker signalBlocker(this); // block propertyEdited signals

  ui->id->setText(currentPw ? currentPw->id.c_str() : "");
  ui->id->setEnabled(currentPw || editMode);

  ui->spinSerial->setValue(currentPw ? currentPw->serial : 0);
  ui->spinSerial->setEnabled(currentPw || editMode);

  std::string algorithm = currentPw ? currentPw->algorithmName() : "";
  ui->selectAlgo->setCurrentText(algorithm.c_str());
  if(ui->selectAlgo->currentText() == algorithm.c_str()) {
    if(ui->selectAlgo->currentIndex() != 0 &&
      ui->selectAlgo->itemData(0) == "UNKNOWN"
    )
      ui->selectAlgo->removeItem(0);
  }
  else {
    if(ui->selectAlgo->count() && ui->selectAlgo->itemData(0) == "UNKNOWN")
      ui->selectAlgo->setItemText(0, algorithm.c_str());
    else
      ui->selectAlgo->insertItem(0, algorithm.c_str(), "UNKNOWN");
    ui->selectAlgo->setCurrentIndex(0);
  }
  ui->selectAlgo->setEnabled(editMode);

  // TODO: don't overwrite valid pw
  ui->password->setText("");
  ui->password->setEchoMode(QLineEdit::Password);
  ui->buttonShowPw->setChecked(false);

  ui->buttonShowPw->setEnabled(currentPw);
  ui->buttonCopyPw->setEnabled(currentPw);

  ui->note->setPlainText(currentPw ? currentPw->note.c_str() : "");
  ui->note->setEnabled(currentPw || editMode);

  changeAlgorithm(algorithm);
  if(algorithmProps)
    algorithmProps->setPassword(currentPw);

  editPending = false;
}

void
PasswordPropsWidget::setEditing(bool edit) {
  if(edit == editMode) return;
  editMode = edit;

  if(editPending)
    refreshPassword();

  ui->id->setReadOnly(!editMode);
  ui->spinSerial->setReadOnly(!editMode);
  ui->selectAlgo->setEnabled(editMode);
  ui->note->setReadOnly(!editMode);

  if(algorithmProps)
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

  if(id != currentPw->id) {
    std::string oldId = currentPw->id;
    currentPw->id = id;
    genpass.updateId(oldId);
  }

  currentPw->serial = ui->spinSerial->value();
  currentPw->note = ui->note->toPlainText().toStdString();

  algorithmProps->commitEdits(*currentPw);

  editPending = false;

  editsCommitted();
}

void
PasswordPropsWidget::deletePassword() {
  if(!currentPw) throw std::runtime_error("no current password to delete");
  genpass::Password *remove = currentPw;
  setEditing(false);
  setPassword(nullptr);
  genpass.removePassword(remove->id);
}

void
PasswordPropsWidget::changeAlgorithm(const std::string& name) {
  if(algorithmProps && name == algorithmProps->algorithmName()) return;

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

  // get new algorithmProps
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
