/* ---------------------------------------------------------------------- *\
 * src/V2AlgorithmHandler.hpp
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

#ifndef __GENPASSGUI_V2ALGORITHMHANDLER_HPP__
#define __GENPASSGUI_V2ALGORITHMHANDLER_HPP__

#include <QLineEdit>             // for QLineEdit
#include <QSpinBox>              // for QSpinBox
#include <string>                // for string

#include "AlgorithmHandler.hpp"  // for AlgorithmHandler, AlgorithmProps

class PasswordPropsWidget;

class V2AlgorithmHandler: public AlgorithmHandler {
public:
  V2AlgorithmHandler();
  virtual ~V2AlgorithmHandler() { }

  virtual std::unique_ptr<AlgorithmProps>
    createProps(PasswordPropsWidget& parent) const override;

  virtual std::unique_ptr<genpass::Password> createPassword() const override;

protected:
  V2AlgorithmHandler(const std::string& algorithm);
};

class V2AlgorithmProps : public AlgorithmProps {
public:
  virtual std::vector<std::pair<std::string, QWidget *>> getWidgets() override;
  virtual void setPassword(const genpass::Password *pw) override;
  virtual void setEditing(bool editing) override;
  virtual void commitEdits(genpass::Password& pw) override;

protected:
  V2AlgorithmProps(const V2AlgorithmHandler& handler,
    PasswordPropsWidget& parent);

private:
  QSpinBox lengthWidget;
  QLineEdit postfixWidget;
  QLineEdit bannedCharsWidget;
  QLineEdit fillWidget;

  const PasswordPropsWidget& parent;

  friend class V2AlgorithmHandler;
};

#endif // __GENPASSGUI_V2ALGORITHMHANDLER_HPP__
