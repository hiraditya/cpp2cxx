#ifndef BUILDSCHEME_H
#define BUILDSCHEME_H
// (c) Copyright
// ALL RIGHTS RESERVED
/**
*  @file BuildScheme.h
*  @brief defines how the project being demacrofied can be build
*  @version 1.0
*  @author Aditya Kumar
*  @note
*  compiles with g++-4.5 or higher,
*  for compiling pass -std=c++0x to the compiler
*/

#include<string>

struct BuildScheme {
  /// @var makeCommand
  /// @brief the command string to be invoked for compiling the package
  /// to be sent to the Parser class(as of now)
  std::string makeCommand;
};

#endif // BUILDSCHEME_H
