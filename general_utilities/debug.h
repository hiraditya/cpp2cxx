#ifndef GENERAL_UTILITIES_DEBUG_H
#define GENERAL_UTILITIES_DEBUG_H

// This file sets up debugging infrastructure.
// This is mainly inspired from the LLVMs (www.llvm.org) method of debugging.

#ifndef NDEBUG
#define DEBUG(ARG) do { ARG; } while(0)
#else
#define DEBUG(ARG)
#endif

#ifdef  DEBUG_CLEANUP_CODE_PRINT
#define DEBUG_CLEANUP_CODE(ARG) DEBUG(ARG)
#else
#define DEBUG_CLEANUP_CODE(ARG)
#endif

#ifdef  DEBUG_CONDITIONALS_PRINT
#define DEBUG_CONDITIONALS(ARG) DEBUG(ARG)
#else
#define DEBUG_CONDITIONALS(ARG)
#endif

#ifdef  DEBUG_CONDITIONALS1_PRINT
#define DEBUG_CONDITIONALS1(ARG) DEBUG(ARG)
#else
#define DEBUG_CONDITIONALS1(ARG)
#endif

#ifdef  DEBUG_PARSER_PRINT
#define DEBUG_PARSER(ARG) DEBUG(ARG)
#else
#define DEBUG_PARSER(ARG)
#endif

#ifdef  CLANG_AST_DEBUG_PRINT
#define CLANG_AST_DEBUG(ARG) DEBUG(ARG)
#else
#define CLANG_AST_DEBUG(ARG)
#endif

#ifdef  DEBUG_RLPARSER_PRINT
#define DEBUG_RLPARSER(ARG) DEBUG(ARG)
#else
#define DEBUG_RLPARSER(ARG)
#endif

#ifdef  DEBUG_RLPARSER2_PRINT
#define DEBUG_RLPARSER2(ARG) DEBUG(ARG)
#else
#define DEBUG_RLPARSER2(ARG)
#endif

#ifdef  DEBUG_DEMACROFIER_PRINT
#define DEBUG_DEMACROFIER(ARG) DEBUG(ARG)
#else
#define DEBUG_DEMACROFIER(ARG)
#endif

#ifdef  DEBUG_SUGGESTION_PRINT
#define DEBUG_SUGGESTION(ARG) DEBUG(ARG)
#else
#define DEBUG_SUGGESTION(ARG)
#endif

#ifdef  DEBUG_TREE_PRINT
#define DEBUG_TREE(ARG) DEBUG(ARG)
#else
#define DEBUG_TREE(ARG)
#endif

#ifdef  DEBUG_MACRO_USE_CASE_PRINT
#define DEBUG_MACRO_USE_CASE(ARG) DEBUG(ARG)
#else
#define DEBUG_MACRO_USE_CASE(ARG)
#endif

#ifdef  ENABLE_WARNING_PRINT
#define ENABLE_WARNING(ARG) DEBUG(ARG)
#else
#define ENABLE_WARNING(ARG)
#endif

#ifdef  DEBUG_MACRO_USE_CASE_PRINT
#define DEBUG_MACRO_USE_CASE(ARG) DEBUG(ARG)
#else
#define DEBUG_MACRO_USE_CASE(ARG)
#endif

#ifdef  DEBUG_MACRO_CLASS_PRINT
#define DEBUG_MACRO_CLASS(ARG) DEBUG(ARG)
#else
#define DEBUG_MACRO_CLASS(ARG)
#endif

#ifdef  DEBUG_MACRO_DEPENDENCY_PRINT
#define DEBUG_MACRO_DEPENDENCY(ARG) DEBUG(ARG)
#else
#define DEBUG_MACRO_DEPENDENCY(ARG)
#endif

#ifdef  DEBUG_ASTCONSUMER_PRINT
#define DEBUG_ASTCONSUMER(ARG) DEBUG(ARG)
#else
#define DEBUG_ASTCONSUMER(ARG)
#endif

#include<iostream>
#include<algorithm>

// This is llvm style debug printer.
extern std::ostream *dbg_stream;
std::ostream& dbgs();
void set_dbg_stream(std::ostream &os);

template<typename T>
inline void PrintElements(std::ostream& os, const T& t)
{
  std::for_each(t.begin(), t.end(), [&os](typename T::value_type v){
      os << v << " ";
      });
  os << "\n";
}

template<typename T>
inline void PrintPointees(std::ostream& os, const T& t)
{
  std::for_each(t.begin(), t.end(), [&os](typename T::value_type v){
      os << *v << " ";
      });
  os << "\n";
}

#endif // GENERAL_UTILITIES_DEBUG_H
