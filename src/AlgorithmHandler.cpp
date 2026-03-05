/* ---------------------------------------------------------------------- *\
 * src/AlgorithmHandler.cpp
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

#include "AlgorithmHandler.hpp"

decltype(AlgorithmHandler::algorithms) AlgorithmHandler::algorithms;

AlgorithmHandler::AlgorithmHandler(const std::string& algorithm) :
  algorithm(algorithm)
{
  if(!AlgorithmHandler::algorithms.insert({algorithm, this}).second)
    throw std::runtime_error("this algorithm handler already exists");
}

AlgorithmHandler::~AlgorithmHandler() {
  auto regIt = AlgorithmHandler::algorithms.find(algorithm);
  if(regIt == AlgorithmHandler::algorithms.end() || regIt->second != this)
    assert(!"deleting unregistered algorithm handler");
  AlgorithmHandler::algorithms.erase(regIt);
}

const AlgorithmHandler *
AlgorithmHandler::getAlgorithm(const std::string& algorithm) {
  auto algIt = AlgorithmHandler::algorithms.find(algorithm);
  if(algIt == AlgorithmHandler::algorithms.end()) return nullptr;
  else return algIt->second;
}
