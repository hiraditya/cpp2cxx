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
