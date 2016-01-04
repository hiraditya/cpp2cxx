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

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

/**
 *  @file FileManager.h
 *  @brief maintains the files, directories.
 *  acts as an interface which can be used to get input files one by one
 *  or all at a time. also provides the interface to write to an output file
 *  @version 1.0
 *  @author Aditya Kumar
 *  @note
 *  compiles with g++-4.5 or higher,
 *  for compiling pass -std=c++0x to the compiler
 */

#include<vector>
#include<string>
#include<fstream>
#include<iostream>

//depending upon the demacrofication scheme the demacrofier should
//return to the subject class which should call the FileManager for file handling
/**
 * @class FileManagerScheme
 * @brief forward declaration
 */
class FileManagerScheme;

/**
  * @class DemacroficationScheme
  * @brief forward declaration
  */
class DemacroficationScheme;

/**
 * @class OverSeer
 * @brief forward declaration
 */
class Overseer;

/**
 * @class FileManager
 * @brief  will be responsible for maintaining all the files that are required
 * to be demacrofied and
 */
class FileManager {

  public:
    /// @function FileManager(FileManagerScheme const& fs)
    /// @brief constructor takes the file scheme FileManagerScheme
    /// uses the FileManagerScheme to configure the settings
    /// configuration can also be changed using the Configure function.
          FileManager(FileManagerScheme const& fs, DemacroficationScheme const& ds);

    /// @brief uses the FileManagerScheme to configure the settings
    void  Configure(FileManagerScheme const& fs);

    /// @brief performs checks etc., not implemented as of now
    /// check if the directory exists or not, if the files
    /// in the list are actually found or not
    bool  SanityCheck();
    //

    //not useful as of now
    /*
    FileManager(std::vector<std::string> const& input_files,
                std::vector<std::string> const& output_files,
                std::string const& input_directory,
                std::string const& output_directory,
                std::string const& backup_directory);
    //not useful as of now
    FileManager(std::vector<std::string> const& input_files,
                std::vector<std::string> const& output_files,
                std::string const& backup_directory);
    //not useful as of now
    FileManager(std::vector<std::string> const& input_files);
*/

    std::vector<std::string>const&  OutputFiles();
    std::vector<std::string>const&  InputFiles();
    std::string const&              OutputDirectory();
    std::string const&              InputDirectory();

    //check whether this file is there in the list of output_files or not
    void          UpdateFile(std::string const& file_str);
    void          UpdateFile(std::ostream& fp, std::string const& file_str);

    /// @brief observer, gets updated whenever a new file is demacrofied
    void          UpdateFile(Overseer const& overseer);

    /// @brief returns the list of input files
    std::vector<std::string> const& GetInputFiles() const;

    /// @brief returns the output file which will be updated in the
    /// current iteration
    std::string   GetOutputFile();

    /// @brief returns the search paths
    std::vector<std::string> const&  GetSearchPaths() const;

    /// @brief writes the string to the log file fileManagerScheme.pConfigFile
    void          WriteLog(std::string const& str);

    /// @brief writes the header to the stat file
    void PrepareMacroStatFile();

    /// @brief writes the header to the demacrofied macro stat file
    void          PrepareDemacrofiedMacroStatFile();

    /// @brief returns the reference to the pLogFile where the messages
    /// can be logged
    std::ostream& GetLogFile();

    /// @brief returns the reference to the pListMacroFile where the macros
    /// can be listed
    std::ostream& GetMacroStatFile();

    /// @brief returns the reference to the pDemacrofiedMacroStatFile
    /// where the list of macros and related information can be logged
    std::ostream& GetDemacrofiedMacroStatFile();

  private:
    /// @var reference to the fileManagerScheme
    FileManagerScheme const&  fileManagerScheme;

    DemacroficationScheme const& demacroficationScheme;
    /// @todo try to make a map from input files to output files
    unsigned int              inputFileIndex;
    unsigned int              outputFileIndex;

};

#endif /*FILEMANAGER_H*/
