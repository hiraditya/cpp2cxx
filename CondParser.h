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

#ifndef CONDPARSER_H
#define CONDPARSER_H

// (c) Copyright
// ALL RIGHTS RESERVED
/**
 *  @file CondParser.h
 *  @brief parses the conditional block of the macro, and builds the tree
 *         with nodes as conditional blocks and macros as elements
 *  @version 1.0
 *  @author Aditya Kumar
 *  @note
 *  compiles with g++-4.5 or higher,
 *  for compiling pass -std=c++0x to the compiler
 */

#include "DepGraph.h"
#include "DemacBoostWaveIncludes.h"

#include <string>
#include <map>

typedef std::map<std::string,   //identifier
        std::string             //replacement text
        > MacroList_t;

/**
 * @class CondParser
 * To parse the tokens of the preprocessing conditional statements
 */
class CondParser {

  public:
    CondParser(std::string file_global_macros);
    void Parser(Node& tree_node, token_iterator t_it);

    bool Match(boost::wave::token_id id);

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

    bool PPCheckIdentifier(std::string id_str);
    token_iterator GetTokenPosition();
  private:
    CondCategory condCat;
    //std::vector<token_type> condStmt;
    Node* pNode;
    token_iterator it;
    MacroList_t macroList;
};

#endif/*CONDPARSER_H*/
