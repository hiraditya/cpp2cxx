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


#include "CondParser.h"
#include "debug.h"

#include <algorithm>
#include <sstream>
#include <fstream>

CondParser::CondParser(std::string file_global_macros)
{
  const unsigned int line_width = 2048;
  //putting line_width instead of 256 gives an error why??
  char fc[line_width],sc[line_width];
  std::ifstream gMacros(file_global_macros);
  if(!gMacros.is_open())
    return;
  gMacros.seekg(0,std::ios::beg);

  //to ignore the newline at the end of the file
  gMacros.getline(fc,line_width);
  gMacros.getline(sc,line_width);
  while(gMacros.good()) {
    macroList.insert(std::make_pair(fc,sc));
    gMacros.getline(fc,line_width);
    gMacros.getline(sc,line_width);
  }
  gMacros.close();
}

void CondParser::Parser(Node& tree_node, token_iterator t_it)
{
  pNode = &tree_node;
  it = t_it;
  boost::wave::token_id id;
  //it = beg;
  //end = term;
  //put the condition statement into node
  (pNode->condStmt).push_back(*it);
  while ((id = *(++it)) == boost::wave::T_SPACE) {
    (pNode->condStmt).push_back(*it);
  }

  DEBUG_RLPARSER(dbgs() << "\nInside Cond Parser\n";);

  Assignment();
  if(Match(boost::wave::T_NEWLINE)) {
    DEBUG_RLPARSER(dbgs() << "PARSER::cond\n";);
  }
}

bool CondParser::Match(boost::wave::token_id id)
{
  boost::wave::token_id next_id;
  std::stringstream id_value;
  next_id = *it;
  //while ((next_id = *it) == boost::wave::T_SPACE)
  //  it++;
  id_value << (*it).get_value();
  if(id == next_id) {
    (pNode->condStmt).push_back(*it);
    if(id == boost::wave::T_NEWLINE)
      return true;
    //std::cout<<"\t\tSymbol Matched: "<<id_value.str();
    id_value.str(std::string());
    while (*(++it) == boost::wave::T_SPACE) {
      (pNode->condStmt).push_back(*it);
    }
    id_value<<(*it).get_value();
    //std::cout<<"\nNext_id: "<<id_value.str();
  }
  else {
    //std::cout<<"\nNOMATCH::Expected: "<<id_value.str();
    //throw "Invalid expression";
    return false;
  }
return true;
}

void CondParser::Assignment()
{
  using namespace boost::wave;
  Expression();
  //assignment-- function like only
  std::stringstream id_value;
  token_id id = token_id(*it);
  id_value << (*it).get_value();
  DEBUG_CONDITIONALS(dbgs()  << "\nin Assignment: "<<id_value.str(););

  switch(id) {
    //comma has the lowest priority
    case T_COMMA:
      Match(id);
      Assignment();
      break;
    case T_ASSIGN://=
    case T_ANDASSIGN://&=
    case T_ANDASSIGN_ALT:// and_eq
    case T_DIVIDEASSIGN:// /=
    case T_MINUSASSIGN:// -=
    case T_STARASSIGN:// *=
    case T_PERCENTASSIGN:
    case T_PLUSASSIGN:
    case T_ORASSIGN:
    case T_ORASSIGN_ALT:// or_eq
    case T_ORASSIGN_TRIGRAPH:
    case T_XOR_TRIGRAPH:
    case T_XORASSIGN:
    case T_XORASSIGN_ALT:
    case T_XORASSIGN_TRIGRAPH:
    case T_SHIFTLEFTASSIGN: // <<=
    case T_SHIFTRIGHTASSIGN:// >>=
      Match(id);
      Expression();
      break;
    default:
      break;
  }
}

void CondParser::Expression()
{
  using namespace boost::wave;
  //bool expr_valid = false;
  Expression1();
  std::stringstream id_value;
  token_id id = token_id(*it);
  id_value << (*it).get_value();
  DEBUG_CONDITIONALS(dbgs()  << "\nin Expression: "<<id_value.str(););

  //although the comma has lower priority than the assignment but
  //it has been kept here to facilitate simple parsing
  while(id == T_AND || id == T_XOR || id == T_OR || id == T_ANDAND ||
    id == T_OROR || id == T_OROR_ALT) {
    Match(id);
    Expression1();
      id = *it;
  }
}

void CondParser::Expression1()
{
  using namespace boost::wave;
  Expression2();
  std::stringstream id_value;
  token_id id = token_id(*it);
  id_value << (*it).get_value();
  DEBUG_CONDITIONALS(dbgs() <<"\nin Expression1: "<<id_value.str(););

  while(id == T_EQUAL || id == T_NOTEQUAL || id == T_NOTEQUAL_ALT ||
      id == T_LESS || id == T_LESSEQUAL || id == T_GREATER ||
      id == T_GREATEREQUAL) {
    Match(id);
    Expression2();
    id = *it;
  }
}

void CondParser::Expression2()
{
  using namespace boost::wave;
  Expression3();
  std::stringstream id_value;
  token_id id = token_id(*it);
  id_value << (*it).get_value();
  DEBUG_CONDITIONALS(dbgs() <<"\nin Expression2: "<<id_value.str(););
  while(id == T_SHIFTLEFT || id == T_SHIFTRIGHT) {
    Match(id);
    Expression3();
    id = *it;
  }
}

void CondParser::Expression3()
{
  using namespace boost::wave;
  Expression4();
  std::stringstream id_value;
  token_id id = token_id(*it);
  id_value << (*it).get_value();
  DEBUG_CONDITIONALS(dbgs() << "\nin Expression3: " << id_value.str(););

  while(id == T_PLUS || id == T_MINUS) {
    Match(id);
    Expression4();
    id = *it;
  }
}

void CondParser::Expression4()
{
  using namespace boost::wave;
  Expression5();
  std::stringstream id_value;
  token_id id = token_id(*it);
  id_value << (*it).get_value();
  DEBUG_CONDITIONALS(dbgs() <<"\nin Expression4: "<<id_value.str(););
  while(id == T_STAR || id == T_DIVIDE || id == T_PERCENT) {
    Match(id);
    Expression5();
    id = *it;
  }
}

void CondParser::Expression5()
{
  using namespace boost::wave;
  Expression6();
  std::stringstream id_value;
  token_id id = token_id(*it);
  DEBUG_CONDITIONALS(dbgs() <<"\nin Expression5: "<<id_value.str(););
  while(id == T_DOTSTAR || id == T_ARROWSTAR) {
    Match(id);
    Expression6();
    id = *it;
  }
}

void CondParser::Expression6()
{
  using namespace boost::wave;
  Expression7();
  std::stringstream id_value;
  token_id id = token_id(*it);
DEBUG_CONDITIONALS(
  dbgs() <<"\nin Expression6: "<<id_value.str();
);
  while(id == T_STAR || id == T_DIVIDE || id == T_PERCENT) {
    Match(id);
    Expression7();
    id = *it;
  }
}

void CondParser::Expression7()
{
  using namespace boost::wave;
  Expression8();
  std::stringstream id_value;
  token_id id = token_id(*it);
  id_value<<(*it).get_value();
  DEBUG_CONDITIONALS(dbgs() <<"\nin Expression7: "<<id_value.str(););
  while(id == T_MINUS ||id == T_PLUS || id == T_NOT || id == T_NOT_ALT ||
    id == T_COMPL || id == T_MINUSMINUS ||id == T_PLUSPLUS) {
    // new then new[] delete then delete []
    Match(id);
    Expression8();
    id = *it;
  }
}
// TODO look for the post increment operator
void CondParser::Expression8()
{
  using namespace boost::wave;
  std::stringstream id_value;
  token_id id = token_id(*it);
  id_value<<(*it).get_value();
  DEBUG_CONDITIONALS(dbgs() <<"\nin Expression8: "<<id_value.str()<<"\n";);

  switch(id) {
    case T_IDENTIFIER:
        DEBUG_CONDITIONALS(dbgs() <<"\t Expected: "<<id_value.str(););
        Match(T_IDENTIFIER);
//      if(id_value.str().compare("defined")==0) {
        id = token_id(*it);
        id_value.str(std::string());
        DEBUG_CONDITIONALS(id_value << (*it).get_value();
                           dbgs() << "\tdefined found: expecting: "
                                  << id_value.str() << "\n";
                           id_value.str(std::string()););

        if(id == T_LEFTPAREN) {
          DEBUG_CONDITIONALS(dbgs() << "\tmatch left paren";);
          Match(T_LEFTPAREN);
          //std::cout<<"\ndefined with identifier within parens";
          //capture the identifier(if any) for lookup
          id_value << (*it).get_value();
          id = token_id(*it);
          Match(id);
          Match(T_RIGHTPAREN);
        }
        else if(id == T_IDENTIFIER) {
          //capture the identifier(if any) for lookup
          id_value << (*it).get_value();
          id = token_id(*it);
          Match(T_IDENTIFIER);
        }
        //although global as well as the macro without an #if block
        //are placed in the root node, gConditions contain only
        //the predefined macros. The definition of config macro can be changed
        //easily by modifying the gConditions.h file
        if(PPCheckIdentifier(id_value.str())) {
          //even if one of the identifiers is global
          //the CondCategory changes to config
          pNode->condCat = CondCategory::config;
          DEBUG_CONDITIONALS(dbgs() << "\nconfig_condition: "
                                    << id_value.str(););
        }//else pNode->condCat = CondCategory::local;//by default
//      }//if id_value.str() = defined
      break;
    case T_OCTALINT:
    case T_DECIMALINT:
    case T_HEXAINT:
    case T_INTLIT:
      DEBUG_CONDITIONALS(dbgs() << "\nExpecting Num literal_type: "
                                << id_value.str() << "\n";);
      Match(id);
      break;
    case T_LONGINTLIT:
    case T_FLOATLIT:
    case T_FIXEDPOINTLIT:  // IDL specific
      Match(id);
      //to bypass <lit>e<+/-><lit><id>
      if(Match(T_IDENTIFIER)) {} // Empty
      break;
    case T_CHARLIT:
    case T_STRINGLIT:
      Match(id);
      break;
    case T_LEFTPAREN:
      DEBUG_CONDITIONALS(dbgs() <<"\nMatch Left Paren";);
      Match(T_LEFTPAREN);
      Expression();
      DEBUG_CONDITIONALS(dbgs() << "\nMatch Right Paren";);
      Match(T_RIGHTPAREN);
      break;

//reject keyword_type
    case T_ASM:
    case T_AUTO:
    case T_BOOL:
    case T_FALSE:
    case T_TRUE:
    case T_BREAK:
    case T_CASE:
    case T_CATCH:
    case T_CHAR:
    case T_CLASS:
    case T_CONST:
    case T_CONSTCAST:
    case T_CONTINUE:
    case T_DEFAULT:
    case T_DO:
    case T_DOUBLE:
    case T_ELSE:
    case T_ENUM:
    case T_EXPLICIT:
    case T_EXPORT:
    case T_EXTERN:
    case T_FLOAT:
    case T_FOR:
    case T_FRIEND:
    case T_GOTO:
    case T_IF:
    case T_INLINE:
    case T_INT:
    case T_LONG:
    case T_MUTABLE:
    case T_NAMESPACE:
    case T_OPERATOR:
    case T_PRIVATE:
    case T_PROTECTED:
    case T_PUBLIC:
    case T_REGISTER:
    case T_RETURN:
    case T_SHORT:
    case T_SIGNED:
    //TODO left for now  to be looked into later
    case T_NEW:
    case T_DELETE:
    case T_DYNAMICCAST:
    case T_REINTERPRETCAST:
    case T_STATICCAST:
    case T_SIZEOF:
      // left paren then expr then right paren
    case T_STATIC:
    case T_STRUCT:
    case T_SWITCH:
    case T_TEMPLATE:
    case T_THIS:
    case T_THROW:
    case T_TRY:
    case T_TYPEDEF:
    case T_TYPEID:
    case T_TYPENAME:
    case T_UNION:
    case T_UNSIGNED:
    case T_USING:
    case T_VIRTUAL:
    case T_VOID:
    case T_VOLATILE:
    case T_WCHART:
    case T_WHILE:
    case T_PP_DEFINE:
    case T_PP_IF:
    case T_PP_IFDEF:
    case T_PP_IFNDEF:
    case T_PP_ELSE:
    case T_PP_ELIF:
    case T_PP_ENDIF:
    case T_PP_ERROR:
    case T_PP_LINE:
    case T_PP_PRAGMA:
    case T_PP_UNDEF:
    case T_PP_WARNING:
    case T_PP_INCLUDE:
    case T_PP_QHEADER:
    case T_PP_HHEADER:
    case T_PP_INCLUDE_NEXT:
    case T_PP_QHEADER_NEXT:
    case T_PP_HHEADER_NEXT:
      Match(id);
      Expression();
      break;
//     //array dereference
//     case T_LEFTBRACKET:
//     case T_LEFTBRACKET_ALT:
//     case T_LEFTBRACKET_TRIGRAPH:
//       Match(id);
//       Expression();
//       Match(T_RIGHTBRACKET);
    //case T_RIGHTBRACKET:
    //case T_RIGHTBRACKET_ALT:
    //case T_RIGHTBRACKET_TRIGRAPH:

    //function like only
    case T_LEFTBRACE:
    case T_LEFTBRACE_ALT:
    case T_LEFTBRACE_TRIGRAPH:
      //look for multiple statements within the braces
      //assume that statements are correct within the block
      Match(id);
      Expression();
      Match(T_RIGHTBRACE);
      //set the category
      break;
    //case T_RIGHTBRACE:
    //case T_RIGHTBRACE_ALT:
    //case T_RIGHTBRACE_TRIGRAPH:
      //skip for now
    case T_POUND_POUND:
    case T_POUND_POUND_ALT:
    case T_POUND_POUND_TRIGRAPH:
    case T_POUND:
    case T_POUND_ALT:
    case T_POUND_TRIGRAPH:
    case T_ELLIPSIS:
      Match(id);
      Match(T_IDENTIFIER);
      break;
    case T_SEMICOLON:
    case T_DOT:
    case T_ARROW:
    // :: has the highest priority
    case T_COLON_COLON:
    case T_COLON:
    case T_DOTSTAR:
    case T_ARROWSTAR:
      Match(id);
      break;
    case T_QUESTION_MARK:
      Match(T_QUESTION_MARK);
      Expression();
      Match(T_COLON);
      Expression();
      break;

    //set rl_tcat::assignment_type;
    //skips
    case T_SPACE:
    case T_SPACE2:
    case T_NEWLINE:
    case T_GENERATEDNEWLINE:
    case T_EOF:
    case T_EOI:
    case T_CONTLINE:
      Match(id);
      break;
    case T_CCOMMENT://eliminate before analyzing
    case T_CPPCOMMENT:
      DEBUG_CONDITIONALS(dbgs() << "\ncomments: " << id_value.str() << "\n";);
      break;
    //may be error
    case T_ANY:
    case T_ANY_TRIGRAPH:
    case T_UNKNOWN:
    case T_FIRST_TOKEN:
    case T_PP_NUMBER:
      Match(id);
      break;
    default:
//      std::cout<<"token: "<<(*it).get_value()
//               <<(*it).get_position().get_line()<<"\n";
//      throw "Expecting lit or id or expr in Rlist";
      break;
  }
}


bool CondParser::PPCheckIdentifier(std::string id_str)
{
  DEBUG_CONDITIONALS(dbgs() << "\nlooking for the identifier: "
                            << id_str << "\n");
  if(macroList.find(id_str) != macroList.end())
    return true;
  else
    return false;
}

token_iterator CondParser::GetTokenPosition()
{
  return it;
}
