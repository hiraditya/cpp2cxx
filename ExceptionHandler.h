#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

/**
*  @file ExceptionHandler.h
*  @brief class that defines various exceptions other than the
*  std::exception which can be used in case of exception
*  @version 1.0
*  @author Aditya Kumar
*  @note
*  compiles with g++-4.5 or higher,
*  for compiling pass -std=c++0x to the compiler
*/

#include "DemacBoostWaveIncludes.h"

#include <string>
#include <sstream>
/**
 * @class ExceptionHandler
 * To handle exceptions
 * @TODO: Change the name to exception
 * Handle exception thrown as ExceptionHandler(const PPMacro* mac, std::string msg)
 */
class ExceptionHandler {
public:

  ExceptionHandler() : message("Unknown Exception")
  {}

  ExceptionHandler(int i) : error_code(i)
  {}

  ExceptionHandler(std::string msg) : message(msg)
  {}

  ExceptionHandler(token_type const& tok, std::string msg)
  {
    std::stringstream err_msg;
    err_msg << "  - exception note: ";
    err_msg //<<tok.get_position().get_file()<<":"
            <<tok.get_position().get_line()<<":"
            <<tok.get_position().get_column()<<": ";
    err_msg << msg << "\n";
    message = err_msg.str();
  }

/*
  ExceptionHandler(const PPMacro* mac, std::string msg)
  {
    std::stringstream err_msg;
    token_type tok = mac->get_identifier();
    err_msg <<tok.get_position().get_file()<<":"
            <<tok.get_position().get_line()<<":"
            <<tok.get_position().get_column()<<":";
    err_msg << msg;
    message = err_msg.str();
  }
*/
  inline const std::string& GetMessage() const
  {
    return message;
  }
  inline int GetErrorCode()
  {
    return error_code;
  }

private:
  std::string message;
  int error_code;

};

#endif /*EXCEPTIONHANDLER_H*/
