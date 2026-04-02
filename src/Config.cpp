/* ------------------------------------------------------------------------ *\
 * src/Config.cpp
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

#include "Config.hpp"

#include <cfgpath.h>

Config::Config() {
  char path[MAX_PATH];
  get_user_data_folder(path, MAX_PATH, "genpass");
  if(*path)
    userDataDir.default_ = std::filesystem::path(path);
}

Config::~Config() { }

void
Config::init(int argc, char const * const *argv) { }
