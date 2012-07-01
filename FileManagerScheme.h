#ifndef FILEMANAGERSCHEME_H
#define FILEMANAGERSCHEME_H

/**
*  @file FileManagerScheme.h
*  @brief defines the configuration for the FileManager
*  @version 1.0
*  @author Aditya Kumar
*  @note
*  compiles with g++-4.5 or higher,
*  for compiling pass -std=c++0x to the compiler
*/

#include<iostream>
#include<vector>
#include<string>

/**
*  @struct FileManagerScheme
*  @brief defines the configuration for the FileManager
*/
struct FileManagerScheme {
  std::ostream*             pLogFile;
  std::ostream*             pDemacrofiedMacroStatFile;
  std::ostream*             pMacroStatFile;
  std::vector<std::string>  inputFiles;
  std::vector<std::string>  outputFiles;
  std::vector<std::string>  searchPaths;
  std::string               inputDirectory;
  std::string               outputDirectory;
  std::string               backupDirectory;
  std::string               cleanup_directory;
  std::string               validator_file;
};

#endif // FILEMANAGERSCHEME_H
