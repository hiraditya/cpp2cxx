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

#include "RlParser.h"
#include "ReplacementList.h"
#include "ExceptionHandler.h"
#include "DemacroficationScheme.h"
#include "debug.h"

#include <algorithm>
#include <sstream>

RlParser::RlParser(DemacroficationScheme const& demacrofication_scheme,
                   std::ostream& log_file)
:pDemacroficationScheme(&demacrofication_scheme),
logFile(log_file)
{

}

void RlParser::Parse(ReplacementList& rl)
{
  //resetting the categories
  //rl_ccat = RlCCat::open;
  rl_ccat = RlCCat::closed;
  rl_dcat = RlDCat::independent;
  rl_idlist.clear();
  funArgId.clear();
  rl_str_formatted.clear();
  rl_ttype.Reset();
  auto replacement_list_temp = rl.rl_tokens;
  //bool rl_modified = false;
  //auto  rl_term_tok_id = boost::wave::token_id(replacement_list_temp.back());
  //auto  rl_term_tok_val =  replacement_list_temp.back().get_value();
  //std::vector<token_type>::iterator beg = rl.rl_tokens.begin();
  //std::vector<token_type>::iterator
  if(replacement_list_temp.back() == boost::wave::T_CPPCOMMENT){
    //std::cout<<"Last token is a cpp comment";
    replacement_list_temp.back().set_token_id(boost::wave::T_NEWLINE);
    replacement_list_temp.back().set_value("\n");
    //rl_modified = true;
    //replacement_list_temp.push_back(token_type(boost::wave::T_NEWLINE));
  }
  //end = rl.rl_tokens.end();
  std::vector<token_type>::iterator beg = replacement_list_temp.begin();
  end = replacement_list_temp.end();

  try {
    funArgId = rl.funArgId;
    switch(rl.rl_tokens.size()) {
      case 0://some problem while parsing the replacement_list
        throw ExceptionHandler("  -note: error while parsing the replacement list");
      case 1://only one token means that is newline
        rl.rl_ccat = RlCCat::closed;
        rl.rl_dcat = RlDCat::independent;
        break;
      default://more than one token required for parsing the replacement list
        Parser(beg,end);
      //need to set these values by the parser
      //change these rvalues to pointers to save space and time
        rl.rl_ccat = rl_ccat;
        rl.rl_dcat = rl_dcat;
        rl.rl_ttype = rl_ttype;
        rl.rl_idlist = rl_idlist;
        rl.rl_str_formatted = rl_str_formatted;
        break;
    }
  } catch(ExceptionHandler& e) {
    //set the reject type and the demacrofier will take care of it
    rl_ccat = RlCCat::open;
    rl_ttype.reject_type = true;
    logFile<<e.GetMessage()<<"\n";
  }
  /*if the replacement text was modified?
  if(rl_modified){
    replacement_list_temp.back().set_token_id(rl_term_tok_id);
    replacement_list_temp.back().set_value(rl_term_tok_val);
  }*/
}

void RlParser::Parser(std::vector<token_type>::iterator beg,
                   std::vector<token_type>::iterator term)
{
  //boost::wave::token_id id;
  it = beg;
  end = term;

DEBUG_RLPARSER(
  std::vector<token_type>::iterator temp_it;
  temp_it = beg;
  std::cout<<"\nInside RL Parser\n";
  while(temp_it!=end) {
    std::cout<<temp_it->get_value();
    temp_it++;
  }
  std::cout<<"\nDone printing RL Parser\n";
  //it = beg;
);
  ExpressionStatement();
  //do not change the order, first CPP comments should be checked
  //see the Match function for why...
/*  if(Match(boost::wave::T_CPPCOMMENT) || Match(boost::wave::T_NEWLINE)) {
DEBUG_RLPARSER(
    std::cout<<"PARSER: RlCCat::closed\n";
);
    rl_ccat = RlCCat::closed;
  }
  */
  //std::cout<<"Out of RL Parser\n";
  //if no error then complete
  //std::cout<<"expression matched successfully\n";
}

bool RlParser::Match(boost::wave::token_id id)
{
  boost::wave::token_id next_id;
  std::stringstream id_value;
  next_id = *it;
  //while ((next_id = *it) == boost::wave::T_SPACE)
  //  it++;
  id_value << it->get_value();
  if(id == next_id) {

DEBUG_RLPARSER(
    std::cout<<"\t\tSymbol Matched: "<<id_value.str();
);

    if(id == boost::wave::T_NEWLINE || id == boost::wave::T_CPPCOMMENT) {
      return true;
    }
    else {
      rl_str_formatted += it->get_value().c_str();
      FillFormattedRL(*it);
      ++it;//increment to get to the next token
    }

    //condStmt.push_back(*it);
    id_value.str(std::string());

    /// @todo all the space is already consumed so no need of this,
    /// but check it first
    while (*it == boost::wave::T_SPACE || *it == boost::wave::T_CCOMMENT) {
      rl_str_formatted += it->get_value().c_str();
      ++it;
    }
    id_value << it->get_value();
    //std::cout<<"\nNext_id: "<<id_value.str();
    return true;
  }
  else {
    //it--;
    //std::cout<<"\nNOMATCH::Expected: "<<id_value.str();
    //if there is mismatch the replacement_list is assumed to be open
    /// @todo decide open/close carefully
    //if(id != boost::wave::T_CPPCOMMENT || id != boost::wave::T_SEMICOLON) {
    if(id != boost::wave::T_SEMICOLON) {
      rl_ccat = RlCCat::open;
      /// @brief also writes a log when there is an unmatching semicolon in a macro.
      logFile<<"  - note: PARSER_ERROR: RlCCat::open \'"<<id_value.str()<<"\'\n";
DEBUG_RLPARSER(
      std::cout<<"\nPARSER: RlCCat::open \'"<< id_value.str()<<"\'\n";
);
    }
    //throw "Invalid expression";
    return false;
  }
}

void RlParser::ExpressionStatement()
{
    /// expression-statement:
    ///      expression_opt ;
    using namespace boost::wave;
    Assignment();
    boost::wave::token_id id = boost::wave::token_id(*it);

  DEBUG_RLPARSER(
    std::stringstream id_value;
    id_value << it->get_value();
    std::cout << "\nin ExpressionStatement: "<<id_value.str();
  );
    while(id == T_SEMICOLON) {
      Match(id);
      Assignment();
      id = *it;
      rl_ttype.statement_type = true;
  }
}

void RlParser::Assignment()
{
  using namespace boost::wave;
  Expression();
  //assignment-- function like only
  std::stringstream id_value;
  boost::wave::token_id id = boost::wave::token_id(*it);
  id_value << it->get_value();
DEBUG_RLPARSER(
  std::cout << "\nin Assignment: "<<id_value.str();
);
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
      rl_ttype.assignment_type = true;
      break;
    default:
      break;
	}
}

void RlParser::Expression()
{
  using namespace boost::wave;
  //bool expr_valid = false;
  Expression1();
  std::stringstream id_value;
  boost::wave::token_id id = boost::wave::token_id(*it);
  id_value << it->get_value();
DEBUG_RLPARSER(
  std::cout << "\nin Expression: "<<id_value.str();
);
  //although the comma has lower priority than the assignment but
  //it has been kept here to facilitate simple parsing
	while(id == T_AND || id == T_XOR || id == T_OR || id == T_ANDAND ||
		id == T_OROR || id == T_OROR_ALT) {
		Match(id);
		Expression1();
			id = *it;
      rl_ttype.binary_operator_type = true;
  }
}

void RlParser::Expression1()
{
  using namespace boost::wave;
  Expression2();
  std::stringstream id_value;
  boost::wave::token_id id = boost::wave::token_id(*it);
  id_value << it->get_value();
  DEBUG_RLPARSER(std::cout<<"\nin Expression1: "<<id_value.str(););
  while(id == T_EQUAL || id == T_NOTEQUAL || id == T_NOTEQUAL_ALT ||
        id == T_LESS || id == T_LESSEQUAL || id == T_GREATER ||
        id == T_GREATEREQUAL) {
    Match(id);
    Expression2();
    id = *it;
    rl_ttype.relational_operator_type = true;
  }
}

void RlParser::Expression2()
{
  using namespace boost::wave;
  Expression3();
  std::stringstream id_value;
  boost::wave::token_id id = boost::wave::token_id(*it);
  id_value << it->get_value();
  DEBUG_RLPARSER(std::cout<<"\nin Expression2: "<<id_value.str(););
  while(id == T_SHIFTLEFT || id == T_SHIFTRIGHT) {
    Match(id);
    Expression3();
    id = *it;
    rl_ttype.binary_operator_type = true;
  }
}

void RlParser::Expression3()
{
  using namespace boost::wave;
  Expression4();
  std::stringstream id_value;
  boost::wave::token_id id = boost::wave::token_id(*it);
  id_value << it->get_value();
  DEBUG_RLPARSER(std::cout<<"\nin Expression3: "<<id_value.str(););
  while(id == T_PLUS || id == T_MINUS) {
    Match(id);
    Expression4();
    id = *it;
    rl_ttype.binary_operator_type = true;
  }
}

void RlParser::Expression4()
{
  using namespace boost::wave;
  Expression5();
  std::stringstream id_value;
  boost::wave::token_id id = boost::wave::token_id(*it);
  id_value << it->get_value();
  DEBUG_RLPARSER(std::cout<<"\nin Expression4: "<<id_value.str(););
  while(id == T_STAR || id == T_DIVIDE || id == T_PERCENT) {
    Match(id);
    Expression5();
    id = *it;
    rl_ttype.binary_operator_type = true;
  }
}

void RlParser::Expression5()
{
  using namespace boost::wave;
  Expression6();
  std::stringstream id_value;
  boost::wave::token_id id = boost::wave::token_id(*it);
  DEBUG_RLPARSER(std::cout<<"\nin Expression5: "<<id_value.str(););
  while(id == T_DOTSTAR || id == T_ARROWSTAR) {
    Match(id);
    Expression6();
    id = *it;
    rl_ttype.binary_operator_type = true;
  }
}

void RlParser::Expression6()
{
  using namespace boost::wave;
  Expression7();
  std::stringstream id_value;
  boost::wave::token_id id = boost::wave::token_id(*it);
  DEBUG_RLPARSER(std::cout<<"\nin Expression6: "<<id_value.str(););
  while(id == T_STAR || id == T_DIVIDE || id == T_PERCENT) {
    Match(id);
    Expression7();
    id = *it;
    rl_ttype.binary_operator_type = true;
  }
}

void RlParser::Expression7()
{
  using namespace boost::wave;
  Expression8();
  std::stringstream id_value;
  boost::wave::token_id id = boost::wave::token_id(*it);
  DEBUG_RLPARSER(std::cout<<"\nin Expression7: "<<id_value.str(););
  if(id == T_POUND_POUND || id == T_POUND_POUND_ALT ||
    id == T_POUND_POUND_TRIGRAPH) {
    rl_ccat = RlCCat::open;
    throw ExceptionHandler(*it, "'##' operator");
  }
}

void RlParser::Expression8()
{
  using namespace boost::wave;
  int brace_count = 0;
  std::stringstream id_value;
  boost::wave::token_id id = boost::wave::token_id(*it);
  id_value<<it->get_value();
  DEBUG_RLPARSER(std::cout<<"\nin Expression8: "<<id_value.str()<<"\n";);

  switch(id) {
    case T_POUND:
    case T_POUND_ALT:
    case T_POUND_TRIGRAPH:
      rl_ccat = RlCCat::open;
      throw ExceptionHandler("  - note: '#' operator");
   //prefix operators
    case T_MINUS:
    case T_PLUS:
    case T_NOT:
    case T_NOT_ALT:
    case T_COMPL:
    case T_MINUSMINUS:
    case T_PLUSPLUS:
    // then new operator then new[], delete, and delete [] operators
      Match(id);
      Expression();
      rl_ttype.unary_operator_type = true;
      break;
    case T_IDENTIFIER:
    /// @todo wrong classification for function call(with args) inside
    /// a function like macro
      rl_idlist.insert(*it);
      DEBUG_RLPARSER(std::cout<<"\nIn Expression8 Expected: "<<id_value.str(););

      //identifier might be defined somewhere
      //if parenthesis follows => function
      //if square brackets => subscript
      //post increment operator
      rl_dcat = RlDCat::dependent;
      std::for_each(funArgId.begin(),funArgId.end(),
        [this](token_type tok) {
          if(*this->it == tok)
            this->rl_dcat = RlDCat::independent;
        });
      Match(T_IDENTIFIER);
      rl_ttype.reject_type = IsRejectPredefinedMacro(id_value.str());
      //this is the id of the next token since Match
      //fetches the next token after a Match
      id = boost::wave::token_id(*it);
      if(id == T_LEFTBRACKET) {
        Match(T_LEFTBRACKET);
        Expression();
        DEBUG_RLPARSER(id_value.str(std::string());
                       id_value<<it->get_value();
                       std::cout<<"\nMatching right bracket: "<<id_value.str(););

        Match(T_RIGHTBRACKET);
      }
      else if(id == T_PLUSPLUS || id == T_MINUSMINUS) {
        //postfix operator okay
        Match(id);
      }
      else if(id == T_LEFTPAREN) { //function --kindof simplification
        Match(id);
        Assignment();
        DEBUG_RLPARSER(id_value.str(std::string());
                       id_value<<it->get_value();
                       std::cout<<"\nMatching right paren of function: "<<id_value.str(););
        Match(T_RIGHTPAREN);
        //look if it is followed by parenthesis
        //so that function
      }
      rl_ttype.identifier_type = true;
      break;
    case T_OCTALINT:
    case T_DECIMALINT:
    case T_HEXAINT:
    case T_INTLIT:
      DEBUG_RLPARSER(std::cout<<"\nExpecting Num literal_type: "<<id_value.str()<<"\n";);
      Match(id);
      rl_ttype.literal_type = true;
      break;
    case T_LONGINTLIT:
    case T_FLOATLIT:
    case T_FIXEDPOINTLIT:  // IDL specific
      Match(id);
      //to bypass <lit>e<+/-><lit><id>
      if(Match(T_IDENTIFIER)) {} // Empty
      rl_ttype.literal_type = true;
      break;
    case T_CHARLIT:
    case T_STRINGLIT:
      Match(id);
      rl_ttype.literal_type = true;
      break;
    case T_LEFTPAREN:
      DEBUG_RLPARSER(std::cout<<"\nMatch Left Paren";);
      Match(T_LEFTPAREN);
      Expression();
      DEBUG_RLPARSER(std::cout<<"\nMatch Right Paren";);
      Match(T_RIGHTPAREN);
      rl_ttype.paren_type = true;
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
    case T_SHORT:
    case T_SIGNED:
    ///@todo left for now  to be looked into later
    ///how to handle macros like #define _size_type_ unsigned int
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
      rl_ttype.keyword_type = true;
      Match(id);
      Expression();
      break;
    case T_RETURN:
    case T_GOTO:
      /// @brief macro body with return definitely will be used for code generation.
      //rl_ttype.reject_type = true;
      rl_ttype.keyword_type = true;
      id_value.str(std::string());
      id_value<<"  - note: found "<<it->get_value()<<", not demacrofying\n";
      throw ExceptionHandler(*it, id_value.str());
      //Match(id);
      //Expression();
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
      DEBUG_RLPARSER2(std::cout<<"\nfound left brace: "<<id_value.str()<<"\n";);
      ++brace_count;
      Match(id);
      //continue overlooking symbols until a right brace is found
      //assumption: the macro is a multi-statement C++ code
      //there are no macros which has only open or only close brace
      do {
        id = *it;
        //std::cout<<"Inside do-while loop curr symbol:"<<it->get_value();
        if(id == T_RIGHTBRACE) {//for nested braces
          DEBUG_RLPARSER2(std::cout<<"\nfound right brace: \n";);
          --brace_count;
        }
        if(id == T_LEFTBRACE) {
          DEBUG_RLPARSER2(std::cout<<"\nfound left brace: \n";);
          ++brace_count;
        }
        if(id == boost::wave::T_NEWLINE)
          break;
        Match(id);
      } while(brace_count!=0);
      if(brace_count != 0) rl_ccat = RlCCat::open;
      rl_ttype.braces_type = true;
      //set the category
      break;
    //if there is a right brace without a left brace that
    //meand it is code-snippet a.k.a. partial
    case T_RIGHTBRACE:
    case T_RIGHTBRACE_ALT:
    case T_RIGHTBRACE_TRIGRAPH:
      rl_ccat = RlCCat::open;
      rl_ttype.braces_type = true;
      break;
      //skip for now

//     case T_POUND_POUND:
//     case T_POUND_POUND_ALT:
//     case T_POUND_POUND_TRIGRAPH:
//       throw ExceptionHandler("##operator");
    case T_ELLIPSIS:
      Match(id);
      Match(T_IDENTIFIER);
      rl_ttype.unknown_type = true;
      break;
    case T_SEMICOLON:
      //std::cout<<"\nlooking for semi colon: \n";
      Match(id);
      // assuming a syntactically corrent macro,
      // semicolon marks end of a statement
      rl_ttype.statement_type = true;
      std::cout<<"\nfound semi colon: \n";
      break;
    case T_DOT:
    case T_ARROW:
    // :: has the highest priority
    case T_COLON_COLON:
    case T_COLON:
    case T_DOTSTAR:
    case T_ARROWSTAR:
      Match(id);
      rl_ttype.special_type = true;
      break;
    case T_QUESTION_MARK:
      Match(T_QUESTION_MARK);
      Expression();
      Match(T_COLON);
      Expression();
      rl_ttype.ternary_operator_type = true;
      break;

    //set rl_tcat::assignment_type;
    //skips
    case T_SPACE:
    case T_SPACE2:
    case T_CONTLINE:
      Match(id);
      break;
    case T_CCOMMENT://eliminate before analyzing
    case T_CPPCOMMENT:
DEBUG_RLPARSER2(
      std::cout<<"\ncomments: "<<id_value.str()<<"\n";
);
      break;
    //may be error
    case T_ANY:
    case T_ANY_TRIGRAPH:
    case T_UNKNOWN:
    case T_FIRST_TOKEN:
    case T_PP_NUMBER:
      rl_ttype.unknown_type = true;
      Match(id);
      break;
    //the new line should come after everything
    case T_NEWLINE:
    case T_GENERATEDNEWLINE:
      /**   **************************************************************
        Before the ExpressionStatement function, the matched T_NEWLINE
        threw error. The only place where a newline comes in the
        replacement text is due to T_CPPCOMMENT. where I insert a T_NEWLINE (see Parser.cpp)
        So \em{that} T_NEWLINE should come at the end and not here. With the introduction
        of ExpressionStatement() it would match the T_NEWLINE \em{here}...
        So now I am not throwing any error from this place.
            **************************************************************
      id_value.str(std::string());
      //ErrorMsg Format: <FileName>:<LineNo>:<ColNo>:<ErrorMessage>
      id_value <<"found T_NEWLINE, failed parsing. "
               <<"expecting literal/identifier/expression "
               <<"in the replacement list";
      throw ExceptionHandler(*it, id_value.str());
      */
      Match(id);
      break;
    case T_EOF:
    case T_EOI:
    default:
     id_value.str(std::string());
     //ErrorMsg Format: <FileName>:<LineNo>:<ColNo>:<ErrorMessage>
     id_value <<"found token: '"<<it->get_value()<<"', "
              <<"expecting literal/identifier/expression "
              <<"in the replacement list";
     throw ExceptionHandler(*it, id_value.str());
  }
}

bool RlParser::IsRejectPredefinedMacro(std::string str) const
{
  //basically all the following macros are categorised as reject types
  if(pDemacroficationScheme->macrosPreventingDemacrofication.find(str)
      != pDemacroficationScheme->macrosPreventingDemacrofication.end()){
    logFile<<"  - note: found the macro "<<str<<", not demacrofying\n";
DEBUG_RLPARSER(
      std::cout<<"\nFound reject type: "<<str;
);
    return true;
  }
  else {
    return false;
  }
}

void RlParser::FillFormattedRL(token_type tok)
{
  using namespace boost::wave;
  token_id id = tok;
  //rl_str_formatted += tok.get_value().c_str();
  //rl_str_formatted += " ";
  switch(id) {
    case T_SEMICOLON:
      rl_str_formatted += "\n";
      break;
    case T_LEFTBRACE:
      rl_str_formatted += "\n";
      break;
    case T_RIGHTBRACE:
      rl_str_formatted += "\n";
      break;
    default:
      break;
  }
//  std::cout<<"FORMATTED REPLACEMENT TEXT:\n";
//  std::cout<<rl_str_formatted<<"\n";
}

/*
 * might be useful for analyzing how the parsing is done
 * capture the output from all the functions like "indside expression 1" etc
void RlParser::print_state_transition()
{
  //std::cout<<replacement_list_str<<"\n";
  //std::for_each(state_transition.begin(),state_transition.end(),[](int v){//std::cout<<v<<" ";});
  //std::cout<<"\n";
}
*/
