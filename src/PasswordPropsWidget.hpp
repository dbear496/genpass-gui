/* ---------------------------------------------------------------------- *\
 * src/PasswordPropsWidget.hpp
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

#ifndef __GENPASSGUI_PASSWORDPROPSWIDGET_HPP__
#define __GENPASSGUI_PASSWORDPROPSWIDGET_HPP__

#include <QWidget>
#include <genpass/Password.hpp>

namespace genpass { class Genpass; }
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
  ~PasswordPropsWidget();

  genpass::Password *getPassword() const { return currentPw; }
  void setPassword(genpass::Password *password);

  bool isEditing() const { return editMode; }
  void setEditing(bool);

signals:
  void editModeChanged(bool);
  void passwordChanged(genpass::Password *);
  void passwordEdited();

private:
  genpass::Genpass& genpass;
  genpass::Password *currentPw = nullptr;
  bool editMode = false;
  const std::unique_ptr<Ui::PasswordPropsWidget> ui;
};

#endif // __GENPASSGUI_PASSWORDPROPSWIDGET_HPP__
