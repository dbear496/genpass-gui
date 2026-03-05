/* ---------------------------------------------------------------------- *\
 * src/AlgorithmHandler.hpp
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

#ifndef __GENPASSGUI_ALGORITHMHANDLER_HPP__
#define __GENPASSGUI_ALGORITHMHANDLER_HPP__

#include <string>
#include <genpass/Genpass.hpp>
#include <stdexcept>
#include <map>
#include <QWidget>

class AlgorithmProps;
class PasswordPropsWidget;

class AlgorithmHandler {
public:
  AlgorithmHandler(const std::string& algorithm);
  virtual ~AlgorithmHandler();

  static const AlgorithmHandler *getAlgorithm(const std::string& algorithm);

  const std::string& algorithmName() const { return algorithm; }

  virtual std::unique_ptr<AlgorithmProps>
    createProps(PasswordPropsWidget&) const = 0;

  virtual std::unique_ptr<genpass::Password> createPassword() const = 0;

private:
  const std::string algorithm;

  static std::map<std::string, const AlgorithmHandler *> algorithms;
};

class AlgorithmProps {
public:
  const AlgorithmHandler& getHandler() const { return handler; }
  const std::string& algorithmName() const { return handler.algorithmName(); }

  virtual std::vector<std::pair<std::string, QWidget *>> getWidgets() = 0;
  virtual void setPassword(const genpass::Password *pw) = 0;
  virtual void setEditing(bool editing) = 0;
  virtual void commitEdits(genpass::Password& pw) = 0;

protected:
  AlgorithmProps(const AlgorithmHandler& handler) : handler(handler) { }

  const AlgorithmHandler& handler;
};

#endif // __GENPASSGUI_ALGORITHMHANDLER_HPP__
