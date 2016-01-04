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

#ifndef RLPARSER_H
#define RLPARSER_H


/**
 *  @file RlParser.h
 *  @brief parses and classifies the replacement list of the macro and
 *         returns the result to the ReplacementList class.
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
#include <set>

/**
 * @class DemacroficationScheme
 * @brief forward declaration
 */
struct DemacroficationScheme;


/// @brief forward declaration
class ReplacementList;


/**
 * @class RlParser
 * @brief The parser of replacement list of the macro
 */
class RlParser {

  public:
    RlParser(DemacroficationScheme const& demacrofication_scheme,
                     std::ostream& log_file);
    void Parse(ReplacementList& rl);
    void Parser(std::vector<token_type>::iterator beg,
             std::vector<token_type>::iterator term);

    bool Match(boost::wave::token_id id);
    void ExpressionStatement();
    void Assignment();
    void Expression();
    void Expression1();
    void Expression2();
    void Expression3();
    void Expression4();
    void Expression5();
    void Expression6();
    void Expression7();
    void Expression8();
    bool IsRejectPredefinedMacro(std::string str) const;
    void FillFormattedRL(token_type tok);
  private:
    std::vector<token_type> funArgId;
    std::vector<token_type>::iterator it;
    std::vector<token_type>::iterator end;
    //formatted replacement list string
    std::string rl_str_formatted;
    //contains identifiers in the ReplacementList
    std::set<token_type,TokenOrder> rl_idlist;
    RlCCat rl_ccat;
    RlDCat rl_dcat;
    RlTokType rl_ttype;
    //information for the parser and demacrofier on how to demacrofy
    DemacroficationScheme const* pDemacroficationScheme;
    //log file to store all the errors and warnings etc.
    std::ostream& logFile;
};

#endif /*RLPARSER_H*/
