/* ------------------------------------------------------------------------ *\
 * src/GenpassWindow.hpp
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

#ifndef __GENPASSGUI_GENPASSWINDOW_HPP__
#define __GENPASSGUI_GENPASSWINDOW_HPP__

#include <genpass/Genpass.hpp> // IWYU pragma: keep
#include <QMainWindow>            // for QMainWindow
#include <QObject>                // for Q_OBJECT
#include <memory>                 // for unique_ptr
#include <QTimer>

#include "PasswordListModel.hpp"  // for PasswordListModel
#include "Config.hpp"

class PasswordPropsWidget;
class QItemSelection;
namespace genpass {
class Password;
}  // namespace genpass

namespace Ui {
  class GenpassWindow;
}

class GenpassWindow : public QMainWindow {
  Q_OBJECT

public:
  GenpassWindow(genpass::Genpass& genpass, Config& config);
  virtual ~GenpassWindow();

  const std::string *genPassword(genpass::Password& pw);
  const std::string *genPassword(const std::string& id) {
    return genPassword(genpass.getPassword(id));
  }

Q_SIGNALS:
  void passwordGenerated(genpass::Password& pw, const std::string& pwStr);

private:
  void updatePasswordSelection(const QItemSelection &newSelection);
  void updatePassword(genpass::Password *newPw);
  void updateEditing(bool editing);

  genpass::Genpass& genpass;

  Config& config;

  PasswordListModel pwListModel;
  PasswordPropsWidget *pwProps;

  struct {
    std::string id;
    std::string pwStr;

    void clear() { id.clear(); clearTimer.stop(); }
    QTimer clearTimer;
  } pwCache;


  const std::unique_ptr<Ui::GenpassWindow> ui;
};

#endif // __GENPASSGUI_GENPASSWINDOW_HPP__
