/* ---------------------------------------------------------------------- *\
 * src/PasswordListModel.hpp
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

#include <QAbstractListModel>
#include <genpass/Genpass.hpp>

class PasswordListModel : public QAbstractListModel {
public:
  PasswordListModel(genpass::Genpass& genpass);
  virtual ~PasswordListModel();

  virtual int rowCount(const QModelIndex& parent) const override;
  virtual int columnCount(const QModelIndex& parent) const override;
  virtual QVariant data(const QModelIndex& index, int role) const override;
  const std::string& get(const QModelIndex& index) const;
  virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

  virtual QVariant headerData(int section, Qt::Orientation orientation,
    int role) const override;

  void refresh();

  void idAdded(const std::string& id);
  void idRemoved(const std::string& id);

private:
  genpass::Genpass& genpass;
  std::vector<std::string> ids;

  bool indexValid(const QModelIndex& index) const;
};
