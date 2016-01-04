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

#ifndef MACRO_HPP
#define MACRO_HPP
#include "MacroScopeClassifier.h"
#include "ReplacementList.h"
#include "RlCategory.h"

#include "DemacBoostWaveIncludes.h"

#include <list>
#include <string>
#include <vector>
#include <ostream>

  // (c) Copyright
  // ALL RIGHTS RESERVED
/**
*  @file Macro.h
*  @brief contains the PPMacro class and various enum classes useful
* for classification of macros into different categories.
*  @version 1.0
*  @author Aditya Kumar
*  @details
*  compiles with g++-4.5 or higher,
*  for compiling pass -std=c++0x to the compiler
*/



/** forward declaration
 * @class DemacroficationScheme
 */
struct DemacroficationScheme;

/** forward declaration
 * @class RlParser
 */
class RlParser;

/** forward declaration
 * @class MacroStat
 */
class MacroStat;

/**
 * @class PPMacro
 * The class to keep all the details of a macro
 */
class PPMacro
{
  public:
    PPMacro(std::ostream& log_file);
    ~PPMacro();
    void set_identifier(token_type const& tok);
    void put_tokens(std::vector<token_type> const& vec_tokens);
    void set_identifier_parameters(token_type const& tok,
                                   unsigned int parameter_count);
    void set_identifier_str(std::string str);
    void set_replacement_list(token_type tok);
    void set_replacement_list_str(std::string str, RlParser & rl_parser);
    void set_operation(PPOperation op);
    void set_macro_category(MacroCategory m_cat);
    void set_replacement_list_category(RlParser & rl_parser);
    void set_conditional_category(CondCategory condCat);
    void set_macro_scope_category(MacroScopeCategory m_scat);
    //for the object like macro first == last for the use_case
    // keep only the first use case
    // as a complete list of use case is already returned by clang
    void set_use_case(std::pair<token_iterator, token_iterator>& token_iter_range);
    void set_use_case_string(std::vector<std::string>& vec_string);

    token_type const get_identifier() const;
    std::vector<token_type> const& get_tokens() const;
    int get_num_tokens() const;
    std::string const& get_identifier_str() const;
    void dump() const;
    std::string get_replacement_list_str() const;
    std::string const& get_formatted_replacement_list_str() const;
    std::string const& get_replacement_list_str_with_comments() const;
    std::vector<std::pair<token_type,unsigned int> > const&
    get_identifier_parameters() const;

    ReplacementList& get_replacement_list();
    ReplacementList const& get_replacement_list() const;
    PPOperation get_operation() const;
    MacroCategory get_macro_category() const;

    bool is_function_like() const
    { return m_cat == MacroCategory::function_like; }

    bool is_object_like() const
    { return m_cat == MacroCategory::object_like; }

    RlDCat
    get_replacement_list_dependency_category() const;

    RlCCat
    get_replacement_list_closure_category() const;

    std::list<token_type>
    get_replacement_list_idlist() const;

    std::list<token_type>
    get_replacement_list_dep_idlist() const;

    CondCategory get_conditional_category() const;

    std::pair<token_iterator,token_iterator>
    get_use_case() const;

    std::vector<std::string> const&
    get_use_case_string() const;

    MacroScopeCategory get_macro_scope_category() const;

    bool IsEquivalent(std::pair<token_iterator, token_iterator> token_iter_range) const;
    void AnalyzeIdentifier() const;
    /// @brief keeps important details about macro for printing to a file
    void set_macro_stat();
    MacroStat const* get_macro_stat();

    bool operator==(PPMacro const& mac) const;
    //bool operator==(token_type const& tok) const;
    //no less than operator should be defined but why??
    bool operator<(PPMacro const& mac) const;

  private:
    bool HasLowerCase() const;
    bool HasLeadingUnderscore() const;

  private:
    typedef std::vector<std::pair<token_type,unsigned int> >vpTokInt;
    // the macro identifier token
    token_type identifier;
    //the complete macro -- operation + identifier +(args)opt + rep_text
    std::vector<token_type> macro_tokens;
    //the complete identifier string including arguments
    std::string identifier_str;
    //keep the function_like PPMacro's arguments and their position
    vpTokInt identifier_parameters;
    PPOperation operation;//define or undefine etc...
    MacroCategory m_cat; //function like or object like etc...
    MacroScopeCategory m_scat;// inside function, inside class, etc...
    //log file to store all the errors and warnings etc.
    std::ostream& logFile;
    ReplacementList rep_list;
    CondCategory condCat;//either config or local
    // keep only the first use case
    std::pair<token_iterator,token_iterator> use_case;
    std::vector<std::string> invoArgs;
    bool use_case_set;
    MacroStat* m_stat;
};

/**
 * @struct MacroOrder
 * @details Used to sort the macros in a map, the sorting is based on pointers
 * so as to handle multiple definitions
 */
struct MacroOrder
{
  bool operator()(const PPMacro* m1, const PPMacro* m2=NULL) const
  {
    //comparing the pointers to handle multiple definitions
    if(m2==NULL)
      return true;
    return m1 < m2;
  }
};

#endif /*MACRO_HPP*/
