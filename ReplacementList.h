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

#ifndef REPLACEMENTLIST_H
#define REPLACEMENTLIST_H

/**
 *  @file ReplacementList.h
 *  @brief responsible for housekeeping of the replacement list of macro.
 *         It calls the RlParser class which parses and classifies the replacement list.
 *  @version 1.0
 *  @author Aditya Kumar
 *  @note
 *  compiles with g++-4.5 or higher,
 *  for compiling pass -std=c++0x to the compiler
 */

#include "RlCategory.h"
#include "DemacBoostWaveIncludes.h"

#include <string>
#include <vector>
#include <list>
#include <set>

/// @brief forward declaration
class RlParser;

/**
 * @class ReplacementList
 * @brief class to keep all the details of replacement list of a macro
 * This class is a member variable of the PPMacro class
 */
class ReplacementList {

  public:
    //use token_iterator const&
    ReplacementList();
    typedef std::vector<std::pair<token_type,unsigned int> >vpTokInt;
    void set_replacement_list(token_type tok);
    void set_replacement_list_str(std::string const& str, vpTokInt argId);
    void set_replacement_list_category(RlParser & rl_parser);
    /// set by the dependency analyzer at a later stage
    void set_replacement_list_dependency_category(bool c);
    std::string get_replacement_list_str() const;
    std::string const& get_formatted_replacement_list_str() const;
    std::string const& get_replacement_list_str_with_comments() const;
    std::vector<token_type> const& get_replacement_list_tokens() const;

    RlDCat
    get_replacement_list_dependency_category() const;

    RlCCat
    get_replacement_list_closure_category() const;

    RlTokType const&
    get_replacement_list_token_type() const;

    RlTokType&
    get_replacement_list_token_type();

    std::list<token_type>
    get_replacement_list_idlist() const;

  private:
    /** friend class
     *  @class RlParser
     */
    friend class RlParser;
    std::vector<token_type> funArgId;

    /// tokens of replacement_list
    std::vector<token_type> rl_tokens;

    /// replacement list string
    std::string rl_str;

    /// formatted replacement list string
    std::string rl_str_formatted;

    /// identifiers in the replacement list of the macro
    std::set<token_type,TokenOrder> rl_idlist;

    RlCCat rl_ccat;
    RlDCat rl_dcat;
    RlTokType rl_ttype;
};

#endif /* REPLACEMENTLIST_H */
