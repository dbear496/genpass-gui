# ---------------------------------------------------------------------- *\
# mk/cmake/cfgpath-config.cmake
# This file is part of GenPass.
#
# Copyright (C) 2026       David Bears <dbear4q@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
# ---------------------------------------------------------------------- */

find_path(${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIR NAMES cfgpath.h
  PATHS ${PROJECT_SOURCE_DIR}/libs/cfgpath
)
mark_as_advanced(${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args( cfgpath
  REQUIRED_VARS cfgpath_INCLUDE_DIR
)

if(${CMAKE_FIND_PACKAGE_NAME}_FOUND)
  set(${CMAKE_FIND_PACKAGE_NAME}_LIBRARIES )
  set(${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIRS
    ${${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIR})
  set(${CMAKE_FIND_PACKAGE_NAME}_DEFINITIONS )

  if(NOT TARGET cfgpath::cgfpath)
    add_library(cfgpath::cfgpath INTERFACE IMPORTED)
    target_include_directories(cfgpath::cfgpath INTERFACE
      ${${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIRS}
    )
    set_target_properties(cfgpath::cfgpath PROPERTIES LINKER_LANGUAGE C)
  endif()
endif()
