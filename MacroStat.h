/**
cpp2cxx is an open source software distributed under terms of the
Apache2.0 licence.

Copyrights remain with the original copyright holders.
Use of this material is by permission and/or license.

Copyright [2012] Aditya Kumar, Andrew Sutton, Bjarne Stroustrup

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef MACROSTAT_H
#define MACROSTAT_H

#include "RlCategory.h"
#include <iostream>
#include <vector>
enum class MacroCategory;

/// use a smart pointer for resource management
struct MacroStat{
  MacroCategory m_cat;
  RlDCat rl_dcat;
  RlCCat rl_ccat;
  std::string id_string;
  std::string rep_list;
};

std::ostream& operator<<(std::ostream& os,MacroStat const& m_stat);
std::ostream& operator<<(std::ostream& os, const std::vector<MacroStat const*>& v);
#endif // MACROSTAT_H
