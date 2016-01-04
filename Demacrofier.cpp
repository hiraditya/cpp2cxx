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

#include "Demacrofier.h"
#include "Parser.h"
#include "DepGraph.h"
#include "ExceptionHandler.h"
#include "DemacBoostWaveIncludes.h"
#include "RlCategory.h"
#include "string_utils.hpp"
#include "debug.h"

#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>

Demacrofier::Demacrofier()
  :headerGuard("#if defined(__cplusplus) && defined(__GXX_EXPERIMENTAL_CXX0X__)"),
   count(0)
{ }

std::string Demacrofier::Translate(PPMacro const* m_ptr, std::ostream& stat,
                                   bool cleanup, bool demacrofy)
{
  bool postponed = false;
  std::string demacrofied_str;
  std::stringstream demacrofied_fstream;
  //std::string instr;
  std::string outstr;
  std::string original_str;
  std::string unique_macro_switch;
  std::stringstream macro_iden;
  std::stringstream err_msg;

  DEBUG_DEMACROFIER(dbgs() << "Inside Demacrofier\n";);

  //in case not demacrofiable return the original_str
  //take the function str and the replacement list from *m_ptr
  original_str = "#define " + m_ptr->get_identifier_str()
               + " " + m_ptr->get_replacement_list_str_with_comments() + "\n";

  macro_iden << m_ptr->get_identifier().get_value();

  //move semantics should optimize pass by value
  unique_macro_switch = GenerateUniqueMacroSwitch(m_ptr);

  //during the cleanup phase see if the macro was validated or not
  if(cleanup && (pValidaMacros->find(unique_macro_switch) == pValidaMacros->end())){
    DEBUG_DEMACROFIER(
      dbgs() << "\nSearched for macro-switch: " << unique_macro_switch << ", not found.";
      dbgs() << "\nMacro: \'" << macro_iden.str() << "\' will not be translated\n";);
      demacrofy = false;
  }

  //test its transformability
  if(IsDemacrofiable(*m_ptr) && demacrofy) {
    //if transformable then collect the transformed string
    postponed = CollectDemacrofiedString(m_ptr, demacrofied_str);
    demacrofied_fstream << demacrofied_str;
  }
  else {
    DEBUG_DEMACROFIER(dbgs() << "replacement_list_closure_category::open";);
    return original_str;
  }//endif RlCCat and RlDCat test

  //should be after the previous if stmt,
  // as the previous one tests for boolean demacrofy.
  if(cleanup){
    outstr = GenerateTranslation(macro_iden.str(), unique_macro_switch,
                                 demacrofied_fstream.str());
    stat << "  - id:" << macro_iden.str() << "\n";
  }
  else{
    outstr = SuggestTranslation(unique_macro_switch, demacrofied_fstream.str(),
                                original_str);

    /**
    macro<count>:
      - id: identifier string
      - category: macro_category
      - header_guard_string: string
    */
    stat<<"  - macro"<<std::setw(sizeof(double)) << std::setfill('0')<<count++<<":\n"
        <<"    - id: "<<m_ptr->get_identifier_str()<<"\n"
        <<"    - category: "<<m_ptr->get_macro_category()<<"\n"
        <<"    - header_guard_string: "
        <<unique_macro_switch<<"\n";
  }

DEBUG_DEMACROFIER(
  dbgs() << "\noriginal_str: " << original_str << "\n";
  dbgs() << "demacrofied_str: " << outstr << "\n";
);
  if(postponed){
    //std::cout << "\nPutting macro: "
    //          << macro_iden.str()
    //          << ", into ready queue:\n" << outstr;
    InsertToReadyQueue(macro_iden, outstr);
    return "";
  }
  else
      return outstr;
}

std::string Demacrofier::DemacrofyFunctionLike(PPMacro const* m_ptr) const
{
  std::stringstream demacrofied_line;
  std::stringstream template_arg;
  std::stringstream arg_str;
  int parameter_count = 0;
  std::vector<std::pair<token_type,unsigned int> >::const_iterator p_it;
  //TODO: make check for function like macro
  //breaks in case when the function doesn't have parameters F(,,,)
  RlTokType token_cat =
    m_ptr->get_replacement_list().get_replacement_list_token_type();

  // F(X) {/**/}
  // F(X) X = X+1
  // F(X) X+Y;
  if(token_cat.braces_type)
    return DemacrofyMultipleStatements(m_ptr);

  if(token_cat.assignment_type || token_cat.statement_type)
    return DemacrofyStatementType(m_ptr);

  if(!m_ptr->get_identifier_parameters().empty()) {
    //for the first argument in the function like macro
    p_it = m_ptr->get_identifier_parameters().begin();
    parameter_count++;
    template_arg << "template <"
                 << "class _T"
                 << p_it->second;
    if(p_it->first != boost::wave::T_EOF) {
      arg_str << "_T"
              << p_it->second
              << " "  //space
              << p_it->first.get_value();
    }
//std::cout<<"the dummy value is "<< (*p_it).first.get_value()<<std::endl;
    //for the 2nd till the last argument in the function like macro
    while(++p_it != m_ptr->get_identifier_parameters().end()) {
      //  parameter_count++;
        template_arg << ", "; //comma then space
        template_arg << "class _T"
                      << p_it->second;

        arg_str << ", ";//comma then space
        if(p_it->first != boost::wave::T_EOF) {
          arg_str << "_T"
                  << p_it->second
                  << " "  //space
                  << p_it->first.get_value();
        }
    }
    template_arg << ">";
    demacrofied_line << template_arg.str();
  }
  demacrofied_line << "\nauto "  //auto then space
                   << m_ptr->get_identifier().get_value()
                   << "(" << arg_str.str()<< ")"
                   << " -> decltype"
                   << "("<< m_ptr->get_replacement_list_str()<< ")"
                   << "\n{\n"
                   << "  return "
                   << m_ptr->get_replacement_list_str()
                   << ";\n}\n";

return demacrofied_line.str();
}

std::string Demacrofier::DemacrofyStatementType(PPMacro const* m_ptr) const
{
  std::stringstream demacrofied_line;
  std::stringstream template_arg;
  std::stringstream arg_str;
  int parameter_count = 0;
  std::vector<std::pair<token_type,unsigned int> >::const_iterator p_it;
  if(!m_ptr->get_identifier_parameters().empty()) {
    //for the first argument in the function like macro
    p_it = m_ptr->get_identifier_parameters().begin();
    parameter_count++;
    template_arg << "template <"
                 << "class _T"
                 << p_it->second;
    if(p_it->first != boost::wave::T_EOF) {
      arg_str << "_T"
              << p_it->second
              << " && "  //space
              << p_it->first.get_value();
    }

    //for the 2nd till the last argument in the function like macro
    while(++p_it != m_ptr->get_identifier_parameters().end()) {
      //  parameter_count++;
        template_arg << ", "; //comma then space
        template_arg << "class _T"
                     << p_it->second;

        arg_str << ", ";//comma then space
        if(p_it->first != boost::wave::T_EOF) {
          arg_str << "_T"
                  << p_it->second
                  << " && "  //space
                  << p_it->first.get_value();
        }
    }
    template_arg << "> "; // '>' then space
    demacrofied_line << template_arg.str();
  }
  demacrofied_line << "\nvoid "  //auto then space
                   << m_ptr->get_identifier().get_value()
                   << "(" << arg_str.str()<< ")\n{\n"
                   << m_ptr->get_formatted_replacement_list_str()
          //semicolon has been added because many use cases
          //do not have semicolon. for the ones which have it doesn't hurt.
                   << ";\n}\n";

return demacrofied_line.str();
}



std::string Demacrofier::DemacrofyMultipleStatements(PPMacro const* m_ptr) const
{
  std::stringstream demacrofied_line;
  std::stringstream template_arg;
  std::stringstream arg_str;
  int parameter_count = 0;
  std::vector<std::pair<token_type,unsigned int> >::const_iterator p_it;
  if(!m_ptr->get_identifier_parameters().empty()) {
    //for the first argument in the function like macro
    p_it = m_ptr->get_identifier_parameters().begin();
    parameter_count++;
    template_arg << "template <"
                 << "class _T"
                 << p_it->second;
    if(p_it->first != boost::wave::T_EOF) {
      arg_str << "_T"
              << p_it->second
              << " && "  //space
              << p_it->first.get_value();
    }
//std::cout<<"the dummy value is "<< (*p_it).first.get_value()<<std::endl;
    //for the 2nd till the last argument in the function like macro
    while(++p_it != m_ptr->get_identifier_parameters().end()) {
      //  parameter_count++;
        template_arg << ", "; //comma then space
        template_arg << "class _T"
                     << p_it->second;

        arg_str << ", ";//comma then space
        if(p_it->first != boost::wave::T_EOF) {
          arg_str << "_T"
                  << p_it->second
                  << " && "  //space
                  << p_it->first.get_value();
        }
    }
    template_arg << "> "; // '>' then space
    demacrofied_line << template_arg.str();
  }
  demacrofied_line << "\nvoid "  //auto then space
                   << m_ptr->get_identifier().get_value()
                   << "(" << arg_str.str()<< ")\n"
                   << m_ptr->get_formatted_replacement_list_str()
                   << "\n";

return demacrofied_line.str();
}

std::string Demacrofier::DemacrofyObjectLike(PPMacro const* m_ptr)const
{
  std::stringstream demacrofied_line;
/*  std::stringstream template_arg;
  std::stringstream arg_str;
  //if it is a statement_type&&assignment_type
  //inside a function  --> lambda function
  //outside a function --> void fun()
  if((m_ptr->get_replacement_list().get_replacement_list_token_type()).assignment_type){
   std::list<token_type> dep_list = m_ptr->get_replacement_list_dep_idlist();
   std::list<token_type>::const_iterator dep_list_iter = dep_list.begin();
   int count = 1;
     if(!dep_list.empty()){
       template_arg << "template <"
                    << "class _T"
                    << count;
       if(*dep_list_iter != boost::wave::T_EOF) {
         arg_str << "_T"
                 << count
                 << " && "  //space
                 << dep_list_iter->get_value();
       }
       while(++dep_list_iter != dep_list.end()){
         ++count;
         template_arg << ", "; //comma then space
         template_arg << "class _T"
                      << count;

         arg_str << ", ";//comma then space
         if(*dep_list_iter != boost::wave::T_EOF) {
           arg_str << "_T"
                   << count
                   << " && "  //space
                   << dep_list_iter->get_value();
         }
      }
      template_arg << "> "; // '>' then space
      demacrofied_line << template_arg.str();
    }
     demacrofied_line << "\ninline void "
                      << m_ptr->get_identifier().get_value()
                      << "(" << arg_str.str()<< ")\n{\n"
                      << m_ptr->get_formatted_replacement_list_str()
                      << ";\n}\n";
     return demacrofied_line.str();
  }*/

  // do constexpr only for integer literals.
  // figure out how to identify integer literals
  demacrofied_line << "constexpr auto "  //auto then space
                   << m_ptr->get_identifier_str()
                   << " = "
                   << m_ptr->get_replacement_list_str()
                   << ";\n";
  return demacrofied_line.str();
}

std::string Demacrofier::DemacrofyObjectLikePostponed(const PPMacro *m_ptr)const
{
  // [closure] ()-> void {fun_body;}
  std::stringstream demacrofied_line;
  std::string type_str = "auto";
  // no need to get rid of these variables by putting the function calls
  // at the usage, compilers are pretty good at doing this
  std::string fun_closure = GetFunctionClosure(m_ptr);
  //std::string fun_args = GetFunctionArgs(m_ptr);
  std::string fun_body = GetFunctionBody(m_ptr);
  demacrofied_line << type_str
                   << " "
                   << m_ptr->get_identifier().get_value()
                   << " = ["
                   << fun_closure
                   << "]()->void { "
                   << fun_body
  // if it already has a semicolon or not
  // if(!(m_ptr->get_replacement_list().get_replacement_list_token_type()).statement_type)
  //  demacrofied_line << ";";
                   << " ; };\n";
  return demacrofied_line.str();
}

bool Demacrofier::IsDemacrofiable(PPMacro const& mac) const
{
  bool demacrofiable = false;
  RlTokType token_cat =
    mac.get_replacement_list().get_replacement_list_token_type();
  //std::cout<<"testing..."<<mac.get_identifier_str()<<"\n";
  //if(mac.get_conditional_category() == CondCategory::local) {
    //std::cout<<"condCat: local\n";
    //std::cout<<"RlCCat: closed\n";
    if(mac.get_replacement_list_closure_category() == RlCCat::closed) {
      MacroCategory m_cat = mac.get_macro_category();
      /// @brief no demacrofication for null_define, variadic or other types
      if(m_cat == MacroCategory::object_like) {
        if(token_cat.keyword_type ||
            (token_cat.assignment_type &&
              !mac.get_macro_scope_category().inside_function)||
          token_cat.braces_type || token_cat.reject_type ||
          token_cat.special_type || token_cat.unknown_type ||
           token_cat.out_of_order_dependent_type)
          demacrofiable = false;
        else
          demacrofiable = true;
      }
      else if (m_cat == MacroCategory::function_like) {
        //demacrofy braces type for the function like PPMacro
        if(token_cat.reject_type || token_cat.unknown_type ||
           token_cat.out_of_order_dependent_type) // || token_cat.braces_type
          demacrofiable = false;
        else
          demacrofiable = true;
        /// @brief if we couldnot capture any use case then
        /// it is not possible to apply the lambda function txform
        if(mac.get_macro_scope_category().inside_function &&
           !mac.get_use_case_string().size())
          demacrofiable = false;
      }
    }
  //}
  //if(mac.get_replacement_list_dependency_category == RlDCat::independent)

    return demacrofiable;
}

void Demacrofier::SetMacroInvocationStat(InvocationStat_t* stat)
{
  pInvocationStat = stat;
}

void Demacrofier::SetASTStat(ASTMacroStat_t *stat)
{
    pASTMacroStat = stat;
}

void Demacrofier::SetValidator(ValidMacros_t const* v_macros)
{
    pValidaMacros = v_macros;
}

std::string Demacrofier::DemacrofyFunctionLikePostponed(const PPMacro *m_ptr)const
{
  std::stringstream demacrofied_line;
  std::string type_str = "auto";
  // no need to get rid of these variables by putting the function calls
  // at the usage, compilers are pretty good at doing this
  std::string fun_closure = GetFunctionClosure(m_ptr);
  std::string fun_args = GetFunctionArgs(m_ptr);
  std::string fun_body = GetFunctionBody(m_ptr);
  demacrofied_line << type_str
                   << " "
                   << m_ptr->get_identifier().get_value()
                   << " = ["
                   << fun_closure
                   << "]("
                   << fun_args
                   << ") { return "
                   << fun_body
                   << "; };\n";
  return demacrofied_line.str();
}

std::string Demacrofier::GetFunctionClosure(const PPMacro *m_ptr)const
{
  std::string closure_str;
  std::list<token_type> dep_list = m_ptr->get_replacement_list_dep_idlist();
  std::list<token_type>::const_iterator dep_list_iter = dep_list.begin();
  if(!dep_list.empty()){
    // taking all the parameters by reference
    closure_str = "&" ;
    closure_str += dep_list_iter->get_value().c_str();
    while(++dep_list_iter != dep_list.end()) {
      closure_str += ", &";
      closure_str += dep_list_iter->get_value().c_str();
    }
  }
  return closure_str;
}

std::string Demacrofier::GetFunctionArgs(const PPMacro *m_ptr)const
{
  std::string dtype = "decltype(";
  std::stringstream arg_string;

  std::vector<std::string>::const_iterator invok_iter =
      m_ptr->get_use_case_string().begin();
  // identifier parameters iterator
  auto ip_iter = m_ptr->get_identifier_parameters().begin();
  if(!m_ptr->get_identifier_parameters().empty()) {
    arg_string << dtype
               << *invok_iter
               << ") "
               << ip_iter->first.get_value();

    while(++ip_iter != m_ptr->get_identifier_parameters().end()){
      arg_string << ", "
                 << dtype
                 << *(++invok_iter)
                 << ") "
                 << ip_iter->first.get_value();
    }
  }
return arg_string.str();
}

std::string Demacrofier::GetFunctionBody(const PPMacro *m_ptr) const
{
  return m_ptr->get_replacement_list_str();
}


//move semantics should optimize return by value
std::string Demacrofier::GenerateUniqueMacroSwitch(PPMacro const* m_ptr) const
{
  std::stringstream m_switch;
  std::string file_name = m_ptr->get_identifier().get_position().get_file().c_str();

  // i think move semantics should be able to optimize this
  file_name = general_utilities::keep_alpha_numeric(file_name);

  m_switch << "USE"
           << "_" << m_ptr->get_identifier().get_value() // = macro_iden.str()
           << "_" << file_name
           << "_" << m_ptr->get_identifier().get_position().get_line()
           << "_" << m_ptr->get_identifier().get_position().get_column();
  return m_switch.str();
}

std::string Demacrofier::SuggestTranslation(std::string const& unique_macro_switch,
                                std::string const& demacrofied_fstream,
                                std::string const& original_str) const
{
  std::string str;
  str =  headerGuard
         + " && defined(" + unique_macro_switch + ")\n"
         + demacrofied_fstream
         + "#else\n"
         + original_str
         + "#endif\n\n";
  DEBUG_SUGGESTION(dbgs() << "\nsuggested translation is:\n" << str;);
  return str;
}

std::string Demacrofier::GenerateTranslation(std::string const& macro_iden,
                                             std::string const& unique_macro_switch,
                                             std::string const& demacrofied_fstream) const
{
  std::string str;
  str =  "\n/** Demacrofication for the macro "
          + macro_iden
          + " with unique identifier "
          + unique_macro_switch + "*/\n"
          + demacrofied_fstream;
  return str;
}

void Demacrofier::InsertToReadyQueue(std::stringstream const& macro_iden,
                                     std::string const& outstr)
{
  // each macro has an entry in the pASTMacroStat
  ASTMacroStat_t::iterator ast_macro_iter = pASTMacroStat->find(macro_iden.str());
  if(ast_macro_iter != pASTMacroStat->end() &&
     ast_macro_iter->second.invoked_lines.size()){
    auto line_no = ast_macro_iter->second.invoked_lines[0];
    readyQueue.insert(std::make_pair(line_no,outstr));
  }
  else{
      //std::cout<<"\nmacro was not found in the ASTConsumer:"<<macro_iden.str();
  }
}

//move semantics should optimize return by value
bool Demacrofier::CollectDemacrofiedString(PPMacro const* m_ptr, std::string& demacrofied_str) const
{
  bool postponed = false;
  if(m_ptr->is_function_like()) {
    if(m_ptr->get_macro_scope_category().inside_function) {
      demacrofied_str = DemacrofyFunctionLikePostponed(m_ptr);
      postponed = true;

      DEBUG_SUGGESTION(dbgs() << "\nThis macro is inside function:\n"
                              << demacrofied_str;);
    }
    else demacrofied_str = DemacrofyFunctionLike(m_ptr);
  }
  else if(m_ptr->is_object_like()) {
    RlTokType token_cat =
      m_ptr->get_replacement_list().get_replacement_list_token_type();
    // only statement like functions can have the lambda function tx
    if(token_cat.assignment_type || token_cat.statement_type || token_cat.braces_type){
        demacrofied_str = DemacrofyObjectLikePostponed(m_ptr);
        postponed = true;
    }
    else demacrofied_str = DemacrofyObjectLike(m_ptr);
  }

  DEBUG_SUGGESTION(if(postponed)
                     dbgs() << "\nTranslation postponed for this macro";);
  return postponed;
}
