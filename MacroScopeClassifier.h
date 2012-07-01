#ifndef MACROSCOPECLASSIFIER_H
#define MACROSCOPECLASSIFIER_H
// (c) Copyright
// ALL RIGHTS RESERVED
/**
*  @file MacroScopeClassifier.h
*  @brief defines various classification criteria for macros
*  @version 1.0
*  @author Aditya Kumar
*  @details
*  compiles with g++-4.5 or higher,
*  for compiling pass -std=c++0x to the compiler
*/

#include<ostream>
/**
 * @enum PPOperation
 */
enum class PPOperation
{
  define,
  undef,
  conditional,
  includes,
  pragma,
  warning,
  line,
  error
};

inline std::ostream& operator<<(std::ostream& os, PPOperation const& oper)
{
  switch(oper){
  case PPOperation::define:
      os<<"define";
      break;
  case PPOperation::undef:
      os<<"undef";
      break;
  case PPOperation::conditional:
      os<<"conditional";
      break;
  case PPOperation::includes:
      os<<"includes";
      break;
  case PPOperation::pragma:
      os<<"pragma";
      break;
  case PPOperation::warning:
      os<<"warning";
      break;
  case PPOperation::line:
      os<<"line";
      break;
  case PPOperation::error:
      os<<"error";
      break;
  default:
      os<<"unknown";
      break;
  }
  return os;
}

/**
 * @enum MacroCategory
 */
enum class MacroCategory
{
  none,
  null_define,//#define X i.e. without replacement_list
  object_like,
  function_like,
  variadic
};

inline std::ostream& operator<<(std::ostream& os, MacroCategory const& m_cat )
{
  switch(m_cat){
    case MacroCategory::none:
      os<<"none";
      break;
    case MacroCategory::null_define:
      os<<"null_define";
      break;
    case MacroCategory::object_like:
      os<<"object_like";
      break;
    case MacroCategory::function_like:
      os<<"function_like";
      break;
    case MacroCategory::variadic:
      os<<"variadic";
      break;
  }
  return os;
}

/**
 * @enum  MacroScopeCategory
 */
struct MacroScopeCategory
{
  MacroScopeCategory()
    :predefined(false),
      local(true),
      inside_function(false),
      inside_class(false)
  { }
  bool predefined;
  bool local;
  bool inside_function;
  bool inside_class;
};
inline std::ostream& operator<<(std::ostream& os, MacroScopeCategory const& m_cat )
{
  if(m_cat.predefined)
    os<<"predefined\t";
  if(m_cat.local)
    os<<"local\t";
  if(m_cat.inside_function)
    os<<"inside_function\t";
  if(m_cat.inside_class)
    os<<"inside_class\t";
  else
    os<<"Not classified\t";
  return os;
}

/*
inline std::ostream& operator<<(std::ostream& os, MacroScopeCategory const& m_cat )
{
  switch(m_cat){
    case MacroScopeCategory::predefined:
      os<<"predefine";
      break;
    case MacroScopeCategory::local:
      os<<"local";
      break;
    case MacroScopeCategory::inside_function:
      os<<"inside_function";
      break;
    case MacroScopeCategory::inside_class:
      os<<"inside_class";
      break;
  }
  return os;
}*/

/**
 * @enum CondCategory
 */
enum class CondCategory
{
  config,
  local
};
inline std::ostream& operator<<(std::ostream& os, CondCategory const& c_cat )
{
  switch(c_cat){
    case CondCategory::config:
      os<<"config";
      break;
    case CondCategory::local:
      os<<"local";
      break;
  }
  return os;
}

#endif // MACROSCOPECLASSIFIER_H
