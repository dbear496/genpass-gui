/* ---------------------------------------------------------------------- *\
 * src/AlgorithmHandler.cpp
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

#include "V2AlgorithmHandler.hpp"

#include <genpass/Password.hpp>

V2AlgorithmHandler::V2AlgorithmHandler() :
  V2AlgorithmHandler(genpass::PasswordV2::algName)
{ }

V2AlgorithmHandler::V2AlgorithmHandler(const std::string& algorithm) :
  AlgorithmHandler(algorithm)
{ }

std::unique_ptr<AlgorithmProps>
V2AlgorithmHandler::createProps(PasswordPropsWidget& parent) const {
  return std::unique_ptr<V2AlgorithmProps>(new V2AlgorithmProps(*this, parent));
}

std::unique_ptr<genpass::Password>
V2AlgorithmHandler::createPassword() const {
  return std::make_unique<genpass::PasswordV2>();
}

V2AlgorithmProps::V2AlgorithmProps(
  const V2AlgorithmHandler& handler,
  PasswordPropsWidget& parent
) :
  AlgorithmProps(handler), parent(parent)
{
  const int arbitraryMaximumLength = 99;

  lengthWidget.setEnabled(false);
  lengthWidget.setRange(0, arbitraryMaximumLength);
  QObject::connect(
    &lengthWidget, &QSpinBox::editingFinished,
    &parent, &PasswordPropsWidget::propertyEdited
  );

  postfixWidget.setReadOnly(true);
  postfixWidget.setMaxLength(arbitraryMaximumLength);
  QObject::connect(
    &postfixWidget, &QLineEdit::editingFinished,
    &parent, &PasswordPropsWidget::propertyEdited
  );

  bannedCharsWidget.setReadOnly(true);
  bannedCharsWidget.setValidator(new QRegularExpressionValidator(
    QRegularExpression("((.)(?!.*\\2))*"))); // unique chars
  QObject::connect(
    &bannedCharsWidget, &QLineEdit::editingFinished,
    &parent, &PasswordPropsWidget::propertyEdited
  );

  fillWidget.setReadOnly(true);
  fillWidget.setMaxLength(1);
  fillWidget.setValidator(new QRegularExpressionValidator(
    QRegularExpression("."))); // single char
  QObject::connect(
    &fillWidget, &QLineEdit::editingFinished,
    &parent, &PasswordPropsWidget::propertyEdited
  );
}

std::vector<std::pair<std::string, QWidget *>>
V2AlgorithmProps::getWidgets() {
  return {
    {"length", &lengthWidget},
    {"postfix", &postfixWidget},
    {"banned", &bannedCharsWidget},
    {"fill", &fillWidget},
  };
}

void
V2AlgorithmProps::setPassword(const genpass::Password *password) {
  const genpass::PasswordV2 *pw =
    dynamic_cast<const genpass::PasswordV2 *>(password);
  const bool editMode = parent.isEditing();

  lengthWidget.setValue(pw ? pw->length : 48);

  postfixWidget.setText(pw ? pw->postfix.c_str() : "");
  postfixWidget.setEnabled(pw || editMode);

  QString bannedStr;
  if(pw) bannedStr.assign(pw->bannedChars.begin(), pw->bannedChars.end());
  bannedCharsWidget.setText(bannedStr);
  bannedCharsWidget.setEnabled(pw || editMode);

  fillWidget.setText(QString(pw ? pw->fill : '0'));
}

void
V2AlgorithmProps::setEditing(bool editing) {
  lengthWidget.setEnabled(editing);
  postfixWidget.setReadOnly(!editing);
  bannedCharsWidget.setReadOnly(!editing);
  fillWidget.setReadOnly(!editing);
}

void
V2AlgorithmProps::commitEdits(genpass::Password& pw) {
  genpass::PasswordV2& v2 = static_cast<genpass::PasswordV2&>(pw);
  v2.length = lengthWidget.value();
  v2.postfix = postfixWidget.text().toStdString();

  auto banned = bannedCharsWidget.text().toStdString();
  v2.bannedChars = std::set(banned.begin(), banned.end());

  v2.fill = fillWidget.text().front().toLatin1();
}
