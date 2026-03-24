/* ------------------------------------------------------------------------ *\
 * src/PasswordPropsWidget.hpp
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

#ifndef __GENPASSGUI_PASSWORDPROPSWIDGET_HPP__
#define __GENPASSGUI_PASSWORDPROPSWIDGET_HPP__

#include <QObject>  // for Q_PROPERTY, Q_OBJECT, Q_SIGNALS
#include <QString>  // for QString
#include <QWidget>  // for QWidget
#include <memory>   // for unique_ptr
#include <string>   // for basic_string, string

class AlgorithmProps;
namespace genpass {
class Genpass;
class Password;
}  // namespace genpass

namespace Ui {
  class PasswordPropsWidget;
}

class PasswordPropsWidget : public QWidget {
  Q_OBJECT
  Q_PROPERTY(genpass::Password *password
    READ getPassword
    WRITE setPassword
    NOTIFY passwordChanged
  )
  Q_PROPERTY(bool editMode
    READ isEditing
    WRITE setEditing
    NOTIFY editModeChanged
  )

public:
  PasswordPropsWidget(genpass::Genpass& genpass);
  virtual ~PasswordPropsWidget();

  genpass::Password *getPassword() const { return currentPw; }
  void setPassword(genpass::Password *password);

  bool isEditing() const { return editMode; }
  void setEditing(bool);
  void commitEdits();
  void deletePassword();

Q_SIGNALS:
  void editModeChanged(bool);
  void passwordChanged(genpass::Password *newPw);
  void propertyEdited(); // user has touched a property in the gui
  void editsCommitted(); // edits from gui written to the backend

private:
  genpass::Genpass& genpass;
  genpass::Password *currentPw = nullptr;
  bool editMode = false;
  bool editPending = false;
  std::unique_ptr<AlgorithmProps> algorithmProps;

  const std::unique_ptr<Ui::PasswordPropsWidget> ui;

  void refreshPassword();
  void changeAlgorithm(const std::string& name);
  void changeAlgorithmQ(const QString& name) {
    changeAlgorithm(name.toStdString());
  }
};

#endif // __GENPASSGUI_PASSWORDPROPSWIDGET_HPP__
