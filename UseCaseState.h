#ifndef USECASESTATE_H
#define USECASESTATE_H

#include "MacroScopeClassifier.h"
#include "Macro.h"
#include "DemacBoostWaveIncludes.h"
#include <sstream>

/**
 * @class UseCaseState 
 * @details The class to keep track of the use case of macro in the code
 * The use case only corresponds to the macro usage in code and not in the
 * replacement list of the macro
 * @todo When the macro is nested within another macro during usage, if the 
 * number of tokens in the identifier list of macro is not the same as the 
 * tokens in use case then the string which records the use case
 * will not contain the complete identifier list
 */
class UseCaseState {
  private:
      typedef std::vector<token_iterator> VecTokenIter_t;
  public:  
    UseCaseState()
      : numParens(0), done(true)
    {}

    int PutToken(token_iterator tok_iter);

    std::pair<token_iterator,token_iterator> Get()
    { return argIter; }

    void Init()
    { numParens = 0; done = true; arg.str(std::string()); argString.clear(); }

    bool DoneCollection()
    { return done; }

    void PutArgBegin(token_iterator tok_iter, PPMacro* p)
    { Init(); argIter.first = tok_iter; done = false; pp = p; }

    void PutArgEnd(token_iterator tok_iter)
    { argIter.second = tok_iter; }

    std::pair<token_iterator,token_iterator>
    GetUseCase()
    { return argIter; }

    void MakeEntry(PPMacro* pp)
    { pp->set_use_case(argIter); }

    void SetUseCaseString(PPMacro* pp)
    { pp->set_use_case_string(argString); }

    // the following function is only for function like macro
    std::vector<std::string> GetArgString()
    { return argString; }

  private:

    int numParens;
    bool done;
    std::pair<token_iterator,token_iterator> argIter;
    std::vector<std::string> argString;
    std::stringstream arg;
    PPMacro* pp;

};

#endif /*USECASESTATE_H*/
