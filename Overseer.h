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

#ifndef OVERSEER_H
#define OVERSEER_H

/**
 *  @file Overseer.h
 *  @brief the class which is responsible for regulating the
 *  functionality of all the classes. It takes the ConfigScheme from the
 *  Main and defines the operations for Parser, Demacrofier and FileManager
 *  @version 1.0
 *  @author Aditya Kumar
 *  @note
 *  compiles with g++-4.5 or higher,
 *  for compiling pass -std=c++0x to the compiler
 */
#include "FileManagerScheme.h"
#include "ConfigScheme.h"
#include "ASTConsumer.hpp"

#include<string>
#include<vector>

/// @todo for error handling each place where error is caught
/// the error message will be sent to the overseer class and this class will
/// log the error according to the demacrofication scheme

/**
 * @class ConfigScheme
 * forward declaration
 */
//class ConfigScheme;
/**
 * @class FileManager
 * forward declaration
 */
class FileManager;

/**
  * @class Parser
  * forward declaration
  */
class Parser;

/**
 * @class MacTree
 * forward declaration
 */
class MacTree;

/**
 * @class Overseer
 * @brief the class responsible for managing the whole project,
 * transferring controls to various other classes and getting
 * their outputs.
 */
class Overseer {
  public:
    Overseer(ConfigScheme& config_scheme);
    ~Overseer();

    void        ConfigureFileManager();
    /**
     * @function StartProcessing
     * @details processing is to be done in the following steps
     * 1. configure file manager
     * 2. get one file from file manager
     * 3. call the parser
     * 4. get the macro count
     * 5. run the dependency analyzer,print total order
     * 6. call the demacrofier as per the scheme
     * 7. update the demacrofied string to the file manager
     * 8. call the build class
     * 9. goto step2
     * 10. catch any errors
     */
    void        StartProcessing(bool demacrofy);
    void        RunParser(std::string file_name);
    void        RunDependencyAnalyzer();
    void        PrintTotalOrder();
    void        RunDemacrofier();
    void        UpdateFileManager();
    void        WriteOutputFile(std::ostream& os) const;
    //void        GetInformationFromExternalSource(const std::string& filename);
    void        GenerateExternalASTHandler(const std::string& filename);
    //void        GenerateExternalASTHandler();
    std::ostream& GetLogFile();
    std::ostream& GetMacroStatFile();
    std::ostream& GetStatFile();
    std::vector<std::string> const& GetInputFiles();
    DemacroficationScheme& GetDemacroficationScheme();
    FileManagerScheme& GetFileManagerScheme();



  private:
    ConfigScheme&       configScheme;
    FileManager*        pFileManager;
    Parser*             pParser;
    MacTree const*      pMacTree;
    MyASTConsumer*      pASTConsumer;
    /// \brief holds information returned from clang AST
    ASTMacroStat_t      ASTMacroStat;
};

#endif /*OVERSEER_H*/
