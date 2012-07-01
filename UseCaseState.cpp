#include "UseCaseState.h"
#include "ExceptionHandler.h"

#ifdef DEBUG_MACRO_USE_CASE
#include <iostream>
#endif

int UseCaseState::PutToken(token_iterator tok_iter)
{
  boost::wave::token_id id = boost::wave::token_id(*tok_iter);
  if(id == boost::wave::T_LEFTPAREN){
    ++numParens;
    if(numParens == 1)
      return 0;
  }
  if(id == boost::wave::T_RIGHTPAREN){
    if((numParens==1) && arg.str().size())// if there was only one argument
      argString.push_back(arg.str());
    --numParens;
  }

  if((id == boost::wave::T_COMMA) && (numParens == 1)) {
    argString.push_back(arg.str());
    arg.str(std::string());//clear the contents
  }
  else{
    arg<<tok_iter->get_value();
  }

  if(numParens == 0){
    PutArgEnd(tok_iter);
    MakeEntry(pp);
    SetUseCaseString(pp);
    done = true;
  }

  return numParens;
}
