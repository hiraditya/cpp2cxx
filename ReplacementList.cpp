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

#include "ReplacementList.h"
#include "RlParser.h"
#include "debug.h"

#include <algorithm>
#include <sstream>

ReplacementList::ReplacementList()
:rl_ccat(RlCCat::open),
 rl_dcat(RlDCat::dependent)
{ }

void ReplacementList::set_replacement_list(token_type tok)
{
  rl_tokens.push_back(tok);
//    std::cout<<"ReplacementList: "<<(*it).get_value()<<std::endl;
}

void ReplacementList::set_replacement_list_str(std::string const& str,
                                                vpTokInt argId)
{
  rl_str = str;
  if(!argId.empty()) {
    std::for_each(argId.begin(),argId.end(),
      [this](std::pair<token_type,int> p_ti) {
      this->funArgId.push_back(p_ti.first);
    });
  }
//    std::cout<<"repl_string: "<<replacement_list_str<<std::endl;
}

void ReplacementList::set_replacement_list_category(RlParser & rl_parser)
{
  //typedef replacement_list_token_category rl_tcat;
  DEBUG_RLPARSER(std::cout<<"ReplacementList:"<<get_replacement_list_str(););
  rl_parser.Parse(*this);
}

/// set by the dependency analyzer at a later stage
void ReplacementList::set_replacement_list_dependency_category(bool c)
{ rl_ttype.out_of_order_dependent_type = c; }

//the rl_tokens will not have any comments, just the pure ReplacementList
std::string
ReplacementList::get_replacement_list_str() const
{
  std::stringstream strm;
  //since the last token is a new line so ignoring the last token
  //by taking rl_tokens.back() instead of rl_tokens.end()
  std::for_each(rl_tokens.begin(),rl_tokens.end()-1,[&strm](token_type tok) {
    strm<<tok.get_value();
  });
  return strm.str();
}

std::string const&
ReplacementList::get_formatted_replacement_list_str() const
{
  return rl_str_formatted;
}
//the replacement_list_str contains all the tokens including the comments
std::string const&
ReplacementList::get_replacement_list_str_with_comments() const
{
  return rl_str;
}

std::vector<token_type> const&
ReplacementList::get_replacement_list_tokens() const
{
  return rl_tokens;
}

RlDCat
ReplacementList::get_replacement_list_dependency_category() const
{
  return rl_dcat;
}

RlCCat
ReplacementList::get_replacement_list_closure_category() const
{
  return rl_ccat;
}

RlTokType const&
ReplacementList::get_replacement_list_token_type() const
{
  return rl_ttype;
}

/// called by the PPMacro class
RlTokType&
ReplacementList::get_replacement_list_token_type()
{
  return rl_ttype;
}


std::list<token_type>
ReplacementList::get_replacement_list_idlist() const
{
  std::list<token_type> id_list;
  std::for_each(rl_idlist.begin(),rl_idlist.end(),
    [&id_list](token_type tok) {
      id_list.push_back(tok);
    });
  return id_list;
}
