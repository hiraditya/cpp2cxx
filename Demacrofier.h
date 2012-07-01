#ifndef DEMACROFIER
#define DEMACROFIER

// (c) Copyright
// ALL RIGHTS RESERVED
/**
 *  @file Demacrofier.h
 *  @brief demacrofies the macros and returns the demacrofied PPMacro as a string
 *  @version 1.0
 *  @author Aditya Kumar
 *  @details
 *  compiles with g++-4.5 or higher,
 *  for compiling pass -std=c++0x to the compiler
 */

#include "Macro.h"
#include "FunctionInfo.h"
#include "ValidatorMap.h"

#include <string>

/**
 * @class Parser
 * @brief forward declaration
 */
class Parser;

/**
 * @class Demacrofier
 * @brief demacrofies the macro on a per macro basis
 */
class Demacrofier {
  public:
    Demacrofier();
    /// @todo change pointer to const ref
    /// when cleaning up, the translations are put without any conditionals
    /// The \e Translate function will generate the demacrofication without
    /// any conditionals wrapped around it. The code generated will have
    /// several formatting modifications as compared to the original code.

    /// when cleanup = false, translations are put with #ifdef conditionals
    /// and by default they are disabled; the macros are enabled.
    std::string Translate(PPMacro const* m_ptr, std::ostream& stat, bool cleanup, bool demacrofy);

    void SetMacroInvocationStat(InvocationStat_t* pInvocationStat);
    void SetASTStat(ASTMacroStat_t*  pASTMacroStat);
    void SetValidator(ValidMacros_t const* v_macros);
  private:

/** 
 * #define FXY(X,Y,Z) ((X) + (Y))
 * template <class T1, class T2, class T3> 
 * auto FXY(T1 X, T2 Y, T3 Z) -> decltype(((X) + (Y)))
 * {
 *   return ((X) + (Y));
 * }
 */   
    std::string DemacrofyFunctionLike(PPMacro const* m_ptr) const;
    /**
     * #define FXY(X,Y,Z) ((X) + (Y))
     * auto FXY = [](decltype(i) X, decltype(j) Y, decltype(k) Z)
     *  { return ((X)+(Y)); }
     * int i = FXY(i,j,k);
     * @todo if type can be deduced from clang then do it
     * for statement like macros use void function
     */
    // lambda function transformation
    std::string DemacrofyFunctionLikePostponed(const PPMacro * m_ptr)const;
    std::string GetFunctionClosure(const PPMacro *m_ptr)const;
    std::string GetFunctionArgs(const PPMacro *m_ptr)const;
    std::string GetFunctionBody(const PPMacro *m_ptr)const;
    /**
      * #define shift2 var = var<<2
      * auto shift2 = [&var]()->void { var = var<<2; }
      */
    std::string DemacrofyObjectLikePostponed(const PPMacro *m_ptr) const;
/**
 * #define _abc 1000
 * constexpr auto _abc = 1000;
 */
    std::string DemacrofyObjectLike(PPMacro const* m_ptr) const;
/**
 * #define proc(X,Y) {  FXY(X,Y,0);  }
 * template<typename T1, typename T2>
 * void proc(T1 X, T2 Y)
 * {
 *   FXY(X,Y,0);
 * }
 */ 
    std::string DemacrofyMultipleStatements(PPMacro const* m_ptr) const;
    std::string DemacrofyStatementType(PPMacro const* m_ptr) const;
    bool IsDemacrofiable(PPMacro const& mac) const;

    std::string SuggestTranslation(std::string const& unique_macro_switch,
                std::string const& demacrofied_fstream,
                std::string const& original_str) const;

    std::string GenerateTranslation(std::string const& macro_iden,
                 std::string const& unique_macro_switch,
                 std::string const& demacrofied_fstream) const;

    std::string GenerateUniqueMacroSwitch(PPMacro const* m_ptr) const;

    void InsertToReadyQueue(std::stringstream const& macro_iden,
                            std::string const& outstr);

    bool CollectDemacrofiedString(PPMacro const* m_ptr, std::string& demacrofied_str) const;

  private:

    // it can change the readyQueue
    friend class Parser;

    // keeps a list of postponed macros according to the line number
    typedef std::map<std::size_t, std::string> ReadyQueue_t;

    ReadyQueue_t readyQueue;
    // contains all the line numbers where macros were invoked in current file
    InvocationStat_t* pInvocationStat;
    // containes all the information about macros in current file a/c clang
    ASTMacroStat_t* pASTMacroStat;

    //pointer to the container having all the valid macros
    //to be used only when the cleanup is in process
    ValidMacros_t const* pValidaMacros;
    std::string headerGuard;
    int count;
};

#endif /*DEMACROFIER*/
