#ifndef REPLACEMENTLIST_H
#define REPLACEMENTLIST_H

// (c) Copyright
// ALL RIGHTS RESERVED
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

    RlDCat const 
    get_replacement_list_dependency_category() const;

    RlCCat const 
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
