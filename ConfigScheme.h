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

#ifndef CONFIGSCHEME_H
#define CONFIGSCHEME_H

/**
 * @file ConfigScheme.h
 * @class ConfigScheme
 * @note defines the specifications for demacrofication
 * 1. To demacrofy one macro at a time or one file at a time
 * 2. Enable the warning flag or not
 * 3. Where should the errors to be put to stderr or stdout or to some file
 * 4. Where should the warnings to be put to stderr or stdout or to some file
 * 5. Whether to demacrofy the configurational macros or not
 * 6. Whether to invoke compiler/make command for each file demacrofied or not
 */

#include "FileManagerScheme.h"
#include "DemacroficationScheme.h"
#include "BuildScheme.h"

#include<iostream>
#include<vector>
#include<string>
#include<set>


class ConfigScheme {

  public:
    ConfigScheme();
    ~ConfigScheme();
    void SetFileManagerScheme(std::vector<std::string>const& input_files,
                              std::vector<std::string>const& output_files,
                              std::vector<std::string>const& search_paths,
                              std::string const& input_directory,
                              std::string const& output_directory,
                              std::string const& backup_directory,
                              std::string const& cleanup_directory,
                              std::string const& validator_file,
                              std::ostream* log_file,
                              std::ostream* stat_file,
                              std::ostream* macro_list_file
                              );
    /// @todo demacrofication granularity to be implemented
    void SetDemacroficationScheme(std::string const& demac_gran,
                                  std::vector<std::string> const& mac_prev_demac,
                                  bool enable_warning,
                                  std::string const& global_mac_raw,
                                  std::string const& global_mac_formatted,
                                  bool multiple_definitions,
                                  bool cleanup
                                  );
    void SetBuildScheme(std::string const& make_command);
    FileManagerScheme& GetFileManagerScheme();
    DemacroficationScheme& GetDemacroficationScheme();
    BuildScheme& GetBuildScheme();

  private:
    FileManagerScheme* pFileManagerScheme;
    DemacroficationScheme* pDemacroficationScheme;
    BuildScheme* pBuildScheme;

};
#endif /*CONFIGSCHEME_H*/
