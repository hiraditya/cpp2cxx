#ifndef RL_CATEGORY_H
#define RL_CATEGORY_H
///  (c) Copyright
///  ALL RIGHTS RESERVED
/**
 *  @file RlCategory.h
 *  @brief contains various enum classes which are helpful in classifying the
 *         replacement list of macro, it also contains replacement_list_token_type
 *         which gives information about what all different types of tokens
 *         the replacement list has.
 *  @version 1.0
 *  @author Aditya Kumar
 *  @note
 *  compiles with g++-4.5 or higher,
 *  for compiling pass -std=c++0x to the compiler
 */
#include<iostream>
/**
 * @class ReplacementListDependencyCategory
 * @brief To tell whether the replacement list of the macro is independent of
 * the identifiers previously defined
 */
enum class ReplacementListDependencyCategory
{
  /// having any identifier previously defined
  dependent,
  /// no identifiers only constants
  independent
};

/**
 * @class ReplacementListClosureCategory
 * @brief To tell whether the replacement list of macro is a closed/complete expression
 */
enum class ReplacementListClosureCategory
{
  /// incomplete expressions
  open,
  /// complete expressions in the sense of C++ expressions without the semicolon
  closed
};

/**
 * @struct ReplacementListTokenType
 * @brief token types present in the Replacement list
 */
struct ReplacementListTokenType
{
  ReplacementListTokenType()
  {
    Reset();
  }

  void Reset()
  {
    identifier_type = false;
    /// string numeric etc.
    literal_type = false;
    /// unary operators like !, ~ , unary plus, unary minus
    unary_operator_type = false;
    /// all binary operators except concatenation and stringification
    binary_operator_type = false;
    ///  ? : conditional operator
    ternary_operator_type = false;
    ///  > , < , == , != etc
    relational_operator_type = false;
    /// +=, -= , = , etc
    assignment_type = false;
    /// ()
    paren_type = false;
    /// {}
    braces_type = false;
    /// all c++ keywords
    keyword_type = false;
    /// reject_type
    reject_type = false;
    /// all others which cannot be categorised
    special_type = false;
    /// when the replacement text has a statement, i.e. semicolon at the end
    statement_type = false;
    /// unknown tokens
    unknown_type = false;
    ///  when atleast one of the tokens is dependent on another macro
    ///  and the dependency is out of topological order
    out_of_order_dependent_type = false;
  }
  /// expression_type when there are more than two tokens
  /// identifiers
  bool identifier_type;
  /// string numeric etc.
  bool literal_type;
  /// unary operators like !, ~ , unary plus, unary minus
  bool  unary_operator_type;
  /// all binary operators except concatenation and stringification
  bool  binary_operator_type;
  ///  ? : conditional operator
  bool  ternary_operator_type;
  ///  > , < , == , != etc
  bool  relational_operator_type;  
  /// +=, -= , = , etc
  bool assignment_type;
  /// ()
  bool paren_type;
  /// {}
  bool braces_type;
  /// all c++ keywords
  bool keyword_type;
  /// e.g. __FILE__ and __LINE__
  bool reject_type;
  /// when the replacement text has a statement, i.e. semicolon at the end
  bool statement_type;
  /// all others which cannot be categorised
  bool special_type;
  /// unknown tokens
  bool unknown_type;
  ///  when atleast one of the tokens is dependent on another macro
  ///  and the dependency is out of topological order
  bool out_of_order_dependent_type;
};

/**
 * @typedef ReplacementListDependencyCategory RlDCat
 */
typedef ReplacementListDependencyCategory RlDCat;
/**
 * @typedef ReplacementListClosureCategory RlCCat;
 */
typedef ReplacementListClosureCategory RlCCat;
/**
 * @typedef ReplacementListTokenType RlTokType;
 */
typedef ReplacementListTokenType RlTokType;
std::ostream& operator<<(std::ostream& os,RlCCat const& cat);
std::ostream& operator<<(std::ostream& os,RlDCat const& cat);

#endif /*RL_CATEGORY_H*/
