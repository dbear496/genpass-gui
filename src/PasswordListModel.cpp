/* ------------------------------------------------------------------------ *\
 * src/PasswordListModel.cpp
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

#include "PasswordListModel.hpp"

// IWYU pragma: no_include <QtCore>
#include <genpass/Genpass.hpp>   // for Genpass, operator==
#include <genpass/Password.hpp>  // for Password
#include <QFlags>                // for QFlags
#include <QModelIndex>           // for QModelIndex
#include <QModelIndexList>       // for QModelIndexList
#include <QString>               // for QString
#include <QVariant>              // for QVariant
#include <algorithm>             // for lower_bound, binary_search, sort
#include <compare>               // for operator<
#include <cstddef>               // for size_t

PasswordListModel::PasswordListModel(genpass::Genpass& genpass)
  : genpass(genpass)
{
  refresh();
}

PasswordListModel::~PasswordListModel() = default;

void
PasswordListModel::refresh() {
  std::vector<std::string> addList;
  for(auto it = genpass.passwords_begin();
    it != genpass.passwords_end(); ++it
  ) {
    if(!std::binary_search(ids.cbegin(), ids.cend(), it->id))
      addList.push_back(it->id);
  }
  std::sort(addList.begin(), addList.end());

  std::vector<std::string> newIds;
  newIds.reserve(genpass.passwordCount());
  QModelIndexList fromIndexes, toIndexes;
  auto idIt = ids.cbegin();
  auto addIt = addList.cbegin();
  while(idIt != ids.cend()) {
    if(!genpass.getPasswordPtr(*idIt)) {
      ++idIt;
      continue;
    }
    else if(addIt != addList.cend() && *addIt < *idIt) {
      newIds.push_back(*addIt);
      ++addIt;
    }
    else {
      std::size_t f = idIt - ids.cbegin();
      std::size_t t = newIds.size();
      if(f != t) {
        fromIndexes.push_back(index(f, 0));
        toIndexes.push_back(index(t, 0));
      }
      newIds.push_back(*idIt);
      ++idIt;
    }
  }
  for(; addIt != addList.cend(); ++addIt) {
    newIds.push_back(*addIt);
  }

  layoutAboutToBeChanged();
  ids = newIds;
  changePersistentIndexList(fromIndexes, toIndexes);
  layoutChanged();
}

void
PasswordListModel::idAdded(const std::string& id) {
  layoutAboutToBeChanged();

  auto pos = std::lower_bound(ids.cbegin(), ids.cend(), id);
  if(*pos == id) return;
  pos = ids.insert(pos, id);

  QModelIndexList fromIndexes, toIndexes;
  for(std::size_t i = pos - ids.begin(); i < ids.size(); ++i) {
    fromIndexes.push_back(index(i, 0));
    toIndexes.push_back(index(i + 1, 0));
  }
  changePersistentIndexList(fromIndexes, toIndexes);
  layoutChanged();
}

void
PasswordListModel::idRemoved(const std::string& id) {
  layoutAboutToBeChanged();

  auto pos = std::lower_bound(ids.cbegin(), ids.cend(), id);
  if(*pos != id) return;
  pos = ids.erase(pos);

  QModelIndexList fromIndexes, toIndexes;
  std::size_t i = pos - ids.begin();
  fromIndexes.push_back(index(i, 0));
  toIndexes.push_back(QModelIndex());
  for(++i; i < ids.size(); ++i) {
    fromIndexes.push_back(index(i, 0));
    toIndexes.push_back(index(i - 1, 0));
  }
  changePersistentIndexList(fromIndexes, toIndexes);
  layoutChanged();
}

int
PasswordListModel::rowCount(const QModelIndex& /*parent*/) const {
  return ids.size();
}

int
PasswordListModel::columnCount(const QModelIndex& /*parent*/) const {
  return 1;
}

bool
PasswordListModel::indexValid(const QModelIndex& index) const {
  return
    index.isValid() &&
    index.parent() == QModelIndex() &&
    index.column() == 0 &&
    index.row() < ids.size();
}

QVariant
PasswordListModel::data(const QModelIndex& index, int role) const {
  if(
    !indexValid(index) ||
    !(role == Qt::DisplayRole || role == Qt::ToolTipRole)
  )
    return QVariant();

  return QString(ids[index.row()].c_str());
}

const std::string&
PasswordListModel::get(const QModelIndex& index) const {
  return ids.at(index.row());
}

Qt::ItemFlags
PasswordListModel::flags(const QModelIndex& /*index*/) const {
  return
    Qt::ItemIsSelectable |
    Qt::ItemIsEnabled |
    Qt::ItemNeverHasChildren;
}

QVariant
PasswordListModel::headerData(
  int section, Qt::Orientation orientation, int role
) const {
  if(
    orientation == Qt::Horizontal &&
    section == 0
  ) {
    switch(role) {
    case Qt::DisplayRole:
      return "ID";
    case Qt::ToolTipRole:
      return "password ID";
    }
  }

  return QVariant();
}

QModelIndex
PasswordListModel::find(const std::string& id) const {
  auto pos = std::lower_bound(ids.cbegin(), ids.cend(), id);
  if(*pos == id) return index(pos - ids.cbegin(), 0);
  else return QModelIndex();
}
