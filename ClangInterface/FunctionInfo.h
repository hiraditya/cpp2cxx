#ifndef FUNCTION_INFO
#define FUNCTION_INFO


#include "MacroScopeClassifier.h"
#include "vector_utils.hpp"

#include <map>
#include <set>
#include <vector>

  struct ParsedDeclInfo{
    std::size_t start_line;
    std::size_t end_line;
  };

  struct CollectedMacroInfo{
    std::size_t defined_line;
    std::vector<std::size_t> invoked_lines;
    PPOperation op;
    MacroCategory m_cat;
    CondCategory c_cat;
    MacroScopeCategory s_cat;
  };
  typedef std::string MacroNameStr;
  typedef std::map<MacroNameStr, CollectedMacroInfo> ASTMacroStat_t;
  typedef std::multiset<std::size_t> InvocationStat_t;
  //typedef std::multimap<int,MacroName> InvocationStat_t;
#endif // FUNCTION_INFO
