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

#include "Macro.h"
#include "MacroStat.h"
#include "RlParser.h"
#include "ExceptionHandler.h"
#include "vector_utils.hpp"
#include "debug.h"

#include <cctype> //for islower

PPMacro::PPMacro(std::ostream& log_file)
:identifier(boost::wave::T_UNKNOWN),
 m_cat(MacroCategory::none),
 logFile(log_file),
 use_case_set(false)
{ m_stat = new MacroStat; }

PPMacro::~PPMacro()
{ /*delete m_stat;*/ }

void PPMacro::set_identifier(token_type const& tok)
{
  identifier = tok;
//    std::cout<<"identifier: "<<(*it).get_value()<<std::endl;
}

void PPMacro::put_tokens(std::vector<token_type> const& vec_tokens)
{
  //macro_tokens will have atleast one element
  macro_tokens = vec_tokens;
  DEBUG_MACRO_CLASS(
  std::cout<<"Tokens for Macro: "<<macro_tokens.begin()->get_value()<<"\n\t";
  std::for_each(macro_tokens.begin(),macro_tokens.end(),
                [](token_type tok) {
                  std::cout<<tok.get_value();
  });
  std::cout<<"\n";);
}

void PPMacro::set_identifier_parameters(token_type const& tok,
                                        unsigned int parameter_count)
{
  //token mytoken(it);
  //std::cout<<"identifier_parameter iD: "<<tok.get_name()<<std::endl;
  identifier_parameters.push_back(std::make_pair(tok,parameter_count));
}

void PPMacro::set_identifier_str(std::string str)
{
  identifier_str = str;
//    std::cout<<"iden_string: "<<identifier_str<<std::endl;
}

void PPMacro::set_replacement_list(token_type tok)
{
  rep_list.set_replacement_list(tok);
  //ReplacementList.push_back(*it);
//    std::cout<<"ReplacementList: "<<(*it).get_value()<<std::endl;
}

void PPMacro::set_replacement_list_str(std::string str, RlParser & rl_parser)
{
  rep_list.set_replacement_list_str(str, identifier_parameters);
//    std::cout<<"repl_string: "<<replacement_list_str<<std::endl;
  logFile<<"  - log: parsing replacement list: '"
         <<str << "' of macro: "
         <<identifier.get_value()<<"\n";
  set_replacement_list_category(rl_parser);
}


void PPMacro::set_operation(PPOperation op)
{
  operation = op;
//    std::cout<<"operations: "<<(*it).get_value()<<std::endl;
}


void PPMacro::set_macro_category(MacroCategory cat)
{
  m_cat = cat;
//    std::cout<<"MacroCategory = "<<m_cat<<std::endl;
}


void PPMacro::set_replacement_list_category(RlParser & rl_parser)
{
  rep_list.set_replacement_list_category(rl_parser);
}


void PPMacro::set_conditional_category(CondCategory ccat)
{
  condCat = ccat;
}


void PPMacro::set_macro_scope_category(MacroScopeCategory scat)
{
  m_scat = scat;
}

void PPMacro::set_use_case(std::pair<token_iterator, token_iterator>& token_iter_range)
{
  if(!use_case_set)
    use_case = token_iter_range;
}

//capturing only the first macro invocation args
/// @brief if there is a mismatch in the number of arguments
/// then demacrofication is not done for that macro
void PPMacro::set_use_case_string(std::vector<std::string>& vec_string)
{
  if(vec_string.size() != identifier_parameters.size()){
    logFile<<"  - log: definition and invocation does not have equal number of args,";
    logFile<<" perhaps multiple definition of same macro.\n";
    logFile<<"  - log: Not demacrofying: "<<identifier_str<<"\n";
    rep_list.get_replacement_list_token_type().reject_type = true;
  }
/*  assert((vec_string.size() == identifier_parameters.size())
         && "definition and invocation does not have equal number of args");
         */
  if(!use_case_set){
    invoArgs = vec_string;
    using namespace general_utilities;
    DEBUG_MACRO_USE_CASE(std::cout<<"Use case args for macro: "
                                  << identifier_str << ":" <<invoArgs<<"\n";);
    use_case_set = true;
  }
}

void PPMacro::AnalyzeIdentifier() const
{
  bool hasProblems = false;
  std::string warning_msg;
  std::stringstream strm;
  //warning_msg = "Analyzing...\t";
  warning_msg = "  - line number: ";
  strm << identifier.get_position().get_line();
  warning_msg += strm.str();
  warning_msg += "\t: ";
  warning_msg += identifier_str;
  warning_msg += "\n";
  if(HasLowerCase()) {
    warning_msg += "  - warning: lower case letters:\n";
    hasProblems = true;
  }
  if(HasLeadingUnderscore()) {
    warning_msg += "  - warning: leading underscore(s):\n";
    hasProblems = true;
  }
  if(hasProblems) {
    logFile<<warning_msg;
    throw ExceptionHandler(warning_msg);
  }
/*
  else
    throw ExceptionHandler(warning_msg + "No Problems!\n");
  */
}

bool PPMacro::HasLowerCase() const
{
  unsigned int i = 0;
  while(i<identifier_str.length()) {
    if(islower(identifier_str[i]))
      return true;
    i++;
  }
  return false;
}


bool PPMacro::HasLeadingUnderscore() const
{
  return identifier_str.compare(0,1,"_") == 0;
}


token_type const PPMacro::get_identifier() const
{ return identifier; }


std::vector<token_type> const& PPMacro::get_tokens() const
{ return macro_tokens; }


int PPMacro::get_num_tokens() const
{ return macro_tokens.size(); }


std::string const& PPMacro::get_identifier_str() const
{ return identifier_str; }


void PPMacro::dump() const
{ std::cout << identifier_str; }

std::string PPMacro::get_replacement_list_str() const
{ return rep_list.get_replacement_list_str(); }


std::string const& PPMacro::get_formatted_replacement_list_str() const
{ return rep_list.get_formatted_replacement_list_str(); }


std::string const& PPMacro::get_replacement_list_str_with_comments() const
{ return rep_list.get_replacement_list_str_with_comments(); }


std::vector<std::pair<token_type,unsigned int> > const&
PPMacro::get_identifier_parameters() const
{ return identifier_parameters; }


ReplacementList& PPMacro::get_replacement_list()
{ return rep_list; }


ReplacementList const& PPMacro::get_replacement_list() const
{ return rep_list; }


PPOperation PPMacro::get_operation() const
{ return operation; }


MacroCategory PPMacro::get_macro_category() const
{ return m_cat; }


RlDCat
PPMacro::get_replacement_list_dependency_category() const
{ return rep_list.get_replacement_list_dependency_category(); }


RlCCat
PPMacro::get_replacement_list_closure_category() const
{ return rep_list.get_replacement_list_closure_category(); }


CondCategory PPMacro::get_conditional_category() const
{ return condCat; }


std::list<token_type>
PPMacro::get_replacement_list_idlist() const
{ return rep_list.get_replacement_list_idlist(); }

std::list<token_type>
PPMacro::get_replacement_list_dep_idlist() const
{
  //will contain dependent identifiers in the ReplacementList
  //i.e. those which donot occur as function arguments
  std::list<token_type> dep_idlist;
  if(m_cat == MacroCategory::null_define)
    return dep_idlist;
  dep_idlist = rep_list.get_replacement_list_idlist();
  if(m_cat == MacroCategory::object_like)
    return dep_idlist;
  //iterator over function_like PPMacro's arguments
  std::vector<std::pair<token_type,unsigned int> >::const_iterator iter_args;
  //iterator for the ReplacementList tokens
  std::list<token_type>::iterator iter_rl_idlist = dep_idlist.begin();
  //remove all those identifiers which are in the function argument
  iter_args = identifier_parameters.begin();
  for(;iter_args != identifier_parameters.end();iter_args++) {
    iter_rl_idlist = dep_idlist.begin();
    for( ; iter_rl_idlist != dep_idlist.end(); iter_rl_idlist++) {
      if(iter_args->first == *iter_rl_idlist) {
        //remove the identifiers which are in the function argument
        dep_idlist.erase(iter_rl_idlist);
        //since the iterators will be invalidated after deletion
        iter_rl_idlist = dep_idlist.begin();
      }
    }
  }
  return dep_idlist;
}


std::pair<token_iterator,token_iterator>
PPMacro::get_use_case() const
{
  return use_case;
}

std::vector<std::string> const& PPMacro::get_use_case_string() const
{
  return invoArgs;
}

MacroScopeCategory PPMacro::get_macro_scope_category() const
{
  return m_scat;
}
  //comparison is based on the token id's to determine their equivalence

bool PPMacro::IsEquivalent(std::pair<token_iterator, token_iterator> token_iter_range) const
{
  return true;
/*
  typedef std::vector<token_type>::const_iterator tok_iter;
  token_iterator first = token_iter_range.first;
  token_iterator second = token_iter_range.second;
  tok_iter this_mac_tok_iter = macro_tokens.begin();
  tok_iter this_mac_tok_iter_end = macro_tokens.end();

  for( ;(this_mac_tok_iter!=this_mac_tok_iter_end) && (first != second);
      ++first,++this_mac_tok_iter) {
#ifdef DEBUG_MACRO_CLASS
    std::cout << "Comparing: " << boost::wave::token_id(*first)
              << "\tand\t" << boost::wave::token_id(*this_mac_tok_iter) << "\n";
#endif
    if(boost::wave::token_id(*first) != boost::wave::token_id(*this_mac_tok_iter))
      return false;
  }
return true;
*/
}

void PPMacro::set_macro_stat()
{
  m_stat->id_string = identifier_str;
  m_stat->rep_list = rep_list.get_replacement_list_str();
  m_stat->m_cat = m_cat;
  m_stat->rl_ccat = rep_list.get_replacement_list_closure_category();
  m_stat->rl_dcat = rep_list.get_replacement_list_dependency_category();
}

MacroStat const* PPMacro::get_macro_stat()
{
  return m_stat;
}

bool PPMacro::operator==(PPMacro const& mac) const
{ //return this->identifier == mac.identifier;
  return (this->identifier_str.compare(mac.identifier_str) == 0);
}

/*bool PPMacro::operator==(token_type const& tok) const
{
  return (this->identifier == tok);
}*/


bool PPMacro::operator<(PPMacro const& mac) const
{
  return (this->identifier_str.compare(mac.identifier_str) < 0);
}
