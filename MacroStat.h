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
