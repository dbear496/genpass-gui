/* ------------------------------------------------------------------------ *\
 * src/Config.hpp
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

#ifndef __GENPASSGUI_CONFIG_HPP__
#define __GENPASSGUI_CONFIG_HPP__

#include <optional>
#include <stdexcept>
#include <filesystem>

class Config {
public:
  template<typename T>
  class Opt {
  public:
    Opt() : default_(std::nullopt) { }
    Opt(const T& default_) : default_(default_) { }

    std::optional<T> default_;
    std::optional<T> config = std::nullopt;
    std::optional<T> session = std::nullopt;

    const T& get() const {
      if(session) return session.value();
      else if(config) return config.value();
      else if(default_) return default_.value();
      else throw std::runtime_error("config option has no default");
    }

    bool isDefault() const { return !session && !config && default_; }
    bool isConfig() const { return !session && config; }
    bool isSession() const { return session; }
  };

  Config();
  ~Config();

  void init(int argc, char const * const *argv);

  Opt<std::filesystem::path> userDataDir;
};

#endif // __GENPASSGUI_CONFIG_HPP__
