# ------------------------------------------------------------------------ *\
# src/CMakeLists.txt
# This file is part of GenPass-GUI.
#
# Copyright (C) 2025-2026 David Bears <dbear4q@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
# ------------------------------------------------------------------------ */

add_executable(genpass-gui
  AlgorithmHandler.cpp
  AlgorithmHandler.hpp
  Config.cpp
  Config.hpp
  GenpassWindow.cpp
  GenpassWindow.hpp
  GenpassWindow.ui
  main.cpp
  PasswordListModel.cpp
  PasswordListModel.hpp
  PasswordPropsWidget.cpp
  PasswordPropsWidget.hpp
  PasswordPropsWidget.ui
  V2AlgorithmHandler.cpp
  V2AlgorithmHandler.hpp
)

target_link_libraries(genpass-gui
  PRIVATE
  genpass::genpass
  Qt6::Widgets
  cfgpath::cfgpath
)
