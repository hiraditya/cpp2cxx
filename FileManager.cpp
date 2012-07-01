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

#include "Overseer.h" //observable
#include "FileManager.h"
#include "FileManagerScheme.h"
#include "ExceptionHandler.h"

FileManager::FileManager(FileManagerScheme const& fs,
                         DemacroficationScheme const& ds)
  : fileManagerScheme(fs),
    demacroficationScheme(ds),
    inputFileIndex(0),
    outputFileIndex(0)
{

  if(!SanityCheck())
    throw ExceptionHandler("Failed Sanity Check in the File Manager");
  PrepareMacroStatFile();
  PrepareDemacrofiedMacroStatFile();
}

void FileManager::Configure(FileManagerScheme const& fs)
{
  //fileManagerScheme = fs;
}

std::vector<std::string>const& FileManager::OutputFiles()
{
  return fileManagerScheme.outputFiles;
}

std::vector<std::string>const& FileManager::InputFiles()
{
  return fileManagerScheme.inputFiles;
}

std::string const& FileManager::OutputDirectory()
{
  return fileManagerScheme.outputDirectory;
}

std::string const& FileManager::InputDirectory()
{
  return fileManagerScheme.inputDirectory;
}

//check whether this file is there in the list of output_files or not
void FileManager::UpdateFile(std::ostream& fp, std::string const& file_str)
{
  fp<<file_str;
}

void FileManager::UpdateFile(std::string const& file_str)
{
  std::string output_file =  GetOutputFile();
  if(output_file.length()){
    std::ofstream fp(output_file);
    if(fp.is_open()) {
      UpdateFile(fp,file_str);
      fp.close();
    }
    else {
      std::string error_msg = "could not open the file "
          + fileManagerScheme.outputFiles[outputFileIndex]
          + " for writing";
      WriteLog(error_msg);
      throw ExceptionHandler(error_msg);
    }
  }
  else {
    ///For no output file name output shall be
    ///redirected to the standard output";
    std::string log_msg;
    log_msg = "for input file"
              + fileManagerScheme.inputFiles[outputFileIndex]
              + "no output file was found"
              + "output shall be redirected to the standard output";
    WriteLog(log_msg);
    UpdateFile(std::cout,file_str);
  }
}

void FileManager::UpdateFile(Overseer const& overseer)
{
  std::string output_file =
      GetOutputFile();
  if(output_file.length()){
    std::ofstream fp(output_file);
    if(fp.is_open()) {
      overseer.WriteOutputFile(fp);
      fp.close();
    }
    else {
      /// @brief if output file is null then the output will be
      /// printed on the screen
      std::string error_msg = "could not open the file "
          + fileManagerScheme.outputFiles[outputFileIndex]
          + " for writing output shall be redirected to the standard output";
      WriteLog(error_msg);
      overseer.WriteOutputFile(std::cout);
    }
  }
  else {
    ///For no output file name output shall be
    ///redirected to the standard output";
    std::string log_msg;
    log_msg = "for input file"
              + fileManagerScheme.inputFiles[outputFileIndex]
              + "no output file was found"
              + "output shall be redirected to the standard output";
    WriteLog(log_msg);
    overseer.WriteOutputFile(std::cout);
  }
}


std::string FileManager::GetOutputFile()
{
  std::string output_file_name = fileManagerScheme.outputFiles[outputFileIndex];
  ++outputFileIndex;
  return output_file_name;
}

std::vector<std::string> const& FileManager::GetSearchPaths() const
{
  return fileManagerScheme.searchPaths;
}

std::vector<std::string> const& FileManager::GetInputFiles()const
{
  return fileManagerScheme.inputFiles;
}

///@todo perform checks
bool FileManager::SanityCheck()
{
  return true;
}

void FileManager::WriteLog(std::string const& str)
{
  *(fileManagerScheme.pLogFile) << str << "\n";
}

void FileManager::PrepareDemacrofiedMacroStatFile()
{
  std::string header;
  if(demacroficationScheme.performCleanup){
    header = "###################################################################\n"
        "#This file contains the details of each macro processed by the demacrofier\n"
        "#The file can be read by any yaml parser\n"
        "#The format is as follows:\n"
        "#file-name:"
        "#  - id: identifier string\n"
        "###################################################################\n";
  }

  else{
    header = "###################################################################\n"
        "#This file contains the details of each macro processed by the demacrofier\n"
        "#The file can be read by any yaml parser\n"
        "#The format is as follows:\n"
        "#macro<count>\n"
        "#  - id: identifier string\n"
        "#  - category: macro_category\n"
        "#  - header_guard_string: string\n"
        "###################################################################\n";
  }
  GetDemacrofiedMacroStatFile()<<header;
}

void FileManager::PrepareMacroStatFile()
{
  std::string header;
  header = "###################################################################\n"
      "#This file contains the details of each macro present in the files processed\n"
      "#The file can be read by any yaml parser\n"
      "#The format is as follows:\n"
      "#macro<count>\n"
      "#  - m_id : identifier string\n"
      "#  - m_cat: macro_category\n"
      "#  - c_cat: if the replacement text maps to C++ expression then c_cat is complete otherwise partial\n"
      "#  - d_cat: if the replacement text contains free variable(s) then d_cat is dependent otherwise closed\n"
      "###################################################################\n";


  GetMacroStatFile()<<header;
}

std::ostream& FileManager::GetLogFile()
{
  return *(fileManagerScheme.pLogFile);
}

std::ostream& FileManager::GetMacroStatFile()
{
  return *(fileManagerScheme.pMacroStatFile);
}

std::ostream& FileManager::GetDemacrofiedMacroStatFile()
{
  return *(fileManagerScheme.pDemacrofiedMacroStatFile);
}
