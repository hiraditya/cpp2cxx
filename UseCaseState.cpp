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

#include "UseCaseState.h"
#include "ExceptionHandler.h"

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
