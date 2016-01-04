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

// (c) Copyright
// ALL RIGHTS RESERVED
/**
 *  @file Main.cpp
 *  @brief main file of the project
 *  @version 1.0
 *  @author Aditya Kumar
 *  @brief takes in the config file, generates configuration scheme for
 *  other classes and then calls the class Overseer.
 *  catches all possible unhandled exceptions and displayes them to user
 *  compiles with g++-4.5 or higher,
 *  for compiling pass -std=c++0x to the compiler
 */

#include "Overseer.h"
#include "ConfigScheme.h"
#include "ExceptionHandler.h"
#include "vector_utils.hpp"

//#include "PrintVector.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
#include <algorithm>
#include <fstream>


/**
  * @main

  typical command line argument:
  ./demacrofier configFile.cfg

  ./a.out -i ifile.cpp -o ofile.cpp --global-macros-formatted=gMacros.dat\
   --input-directory=. --output-directory=demac_dir --log-file=log.txt\
   -p __FILE__ -p __LINE__ --backup-directory=backup
  */

/// @todo add object like, fucntion like, open,closed, config etc..
/// to the macro_stat_file
int main(int argc, char* argv[])
{
  //std::cout<<"\nName of executable is:"<<argv[0];
  //configuration file --to be loaded instead of the command line parameters
  std::string config_file;
  if(argc<2) {
    config_file = "ConfigFile.cfg";
    //std::cerr<<"usage: config-file\n";
    //return -1;
  }
  else{
    config_file = argv[1];
  }

/**     ***********   BEGIN PRORAM OPTIONS   ************/
  try {

    //the default file where the errors and warnings will be put to
    std::ostream* err_outstream = &std::cerr;
    std::ofstream plog_file;
    //the default file where the demacrofied macros will be listed
    std::ostream* stat_outstream = &std::cerr;
    std::ofstream pstat_file;
    std::ofstream mac_stat_file;
    std::vector<std::string>input_files;
    std::vector<std::string>output_files;
    std::vector<std::string>search_paths;
    //the file where all the macros will be listed
    std::string macro_list_file;
    std::string log_file;
    std::string stat_file;
    std::string validator_file;
    std::string input_directory;
    std::string output_directory;
    std::string cleanup_directory;
    std::string backup_directory;
    bool enable_warning = false;
    bool no_translate = false;
    bool cleanup = false;
    bool multiple_definitions = false;
    std::string make_command;
    std::vector<std::string> macros_preventing_demacrofication;
    std::string demacrofication_granularity;
    std::string global_macros_raw;
    std::string global_macros_formatted;

    po::options_description config_file_options("Allowed options");
    config_file_options.add_options()
      ("help,h", "produce help message")
      ("verbose,v", po::value<int>()->implicit_value(1), //not used
            "enable verbosity (optionally specify level)")
      ("enable-warning", po::value<bool>(&enable_warning),"display warning message")
      ("no-translate", po::value<bool>(&no_translate),"only analyze the macros without translation")
      ("cleanup", po::value<bool>(&cleanup),"perform cleanup based on already validated refactorings")
      ("mul-def",po::value<bool>(&multiple_definitions),"allow demacrofication when multiple definitions of macros are allowed")
      ("input-file,i",
          po::value< std::vector<std::string> >(&input_files),
          "input file")
      ///@todo if output file is null then the output will be printed on the screen
      ///check this
      ("output-file,o",
          po::value< std::vector<std::string> >(&output_files),
          "output file")
      ("search-path,sp",
          po::value< std::vector<std::string> >(&search_paths),
          "paths where the clang front end will search for header files")
      ("macro-stat-file",
         po::value<std::string>(&macro_list_file),
       "file having all the macros listed for statistics")
      ("log-file,l",
          po::value<std::string>(&log_file),
          "file where the error and warnings will be logged")
      ("stat-file,s",
          po::value<std::string>(&stat_file),
          "file where the list of macros processed will be logged along with other details")
      ("validator-file,s",
          po::value<std::string>(&validator_file)->default_value("Defined.h"),
          "File which has been generated by the Validator script. It should contain all the macro-switches which needs to be finally replaced")
      ("ignore-macros,p",
          po::value<std::vector<std::string> >(&macros_preventing_demacrofication),
          "those predefined macros if they are in the replacement text of any other macro will prevent it from being demacrofied")
      ("global-macros-formatted",
          po::value<std::string>(&global_macros_formatted),
         "file containing global macros formatted in following way\n<Macro-id>\n<Replacement-list>")
      ("global-macros-raw",
          po::value<std::string>(&global_macros_raw),
          "file containing global macros")
      ("demacrofication-granularity",
          po::value<std::string>(&demacrofication_granularity)->default_value("OneFileAtATime"),
       "granularity of demacrofication, either OneFileAtATime or OneMacroAtATime")
      ("input-directory",
          po::value<std::string>(&input_directory)->default_value("."),
          "input directory")
      ("output-directory",
          po::value<std::string>(&output_directory)->default_value("dm_dir"),
          "the directory where translated files should be put to")
      ("cleanup-directory",
          po::value<std::string>(&cleanup_directory)->default_value("cleanup"),
          "the directory where (finally) demacrofied files should be put to")
      ("backup-directory",
          po::value<std::string>(&backup_directory)->default_value("."),
          "the directory where source files should be copied for backup, this directory will be deleted later")
      ("make-command",
          po::value<std::string>(&make_command)->default_value("make"),
          "the command which needs to be invoked in the shell to compile the project")
    ;

    //po::positional_options_description p;
    //p.add("input-file", -1);

    po::variables_map vm;
    std::ifstream cfg(config_file);
    if(!cfg.is_open())
    {
         std::cerr << "can not open config file: " << config_file << "\n";
         return -1;
    }
    else
    {
        po::store(po::parse_config_file(cfg, config_file_options), vm);
        po::notify(vm);
    }

/*
    po::store(po::command_line_parser(argc, argv).
              options(config_file_options).positional(p).run(), vm);
              */
    po::notify(vm);

    if(vm.count("help")) {
      std::cout << "usage: <Executable> [options]\natleast input file is a must.\n";
      std::cout << "If no output file is specified then the output "
                << "will be redirected to the standard output\n";
      std::cout << config_file_options;
      return 0;
    }

    if(vm.count("no-translate"))
    {
      if(no_translate)
        std::cout<<"processing the macros for just analysis\n"
                 <<"-- if you want to translate, set option no-translate to false\n";
      else
        std::cout<<"processing and translating the macros\n"
                 <<"-- if you don\'t want to translate, set option no-translate to true\n";
    }

    if(vm.count("input-file"))
    {
      /*
      input_files = vm["input-file"].as<std::vector<std::string> >();
      std::cout << "Input files are: "
                << vm["input-file"].as<std::vector<std::string> >()<< "\n";
      */
     /// @todo check if prefixing the output directory path to the output
     /// file name is useful or not
     std::for_each(input_files.begin(), input_files.end(),
                      [input_directory](std::string& input_file) {
                        input_file =  input_directory +"/"+ input_file;
                      });
      std::cout<<"input files are:\n"<<input_files<<"\n";
    }
    else
    {
      std::cerr << "error: no input file specified. exiting...\n";
      std::cerr << config_file_options;
      return -1;
    }

    //during the cleanup process the output files are taken corresponding
    //to the cleanup files
    if(vm.count("output-file"))
    {
      //if output files are given then total number of output files
      //should be equal to the total number of input files
      if(input_files.size() && (input_files.size() != output_files.size())) {
        std::cerr<<"Number of input files and output files are not equal";
        return -1;
      }
      else {
        /*
        if(cleanup)
            output_directory = cleanup_directory;
        */
        /// @todo check if prefixing the output directory path to the output
        /// file name is useful or not
        std::for_each(output_files.begin(),output_files.end(),
                      [output_directory](std::string& output_file) {
                        output_file =  output_directory +"/"+ output_file;
                      });
      }
      //std::cout << "Output files are: "
      //          << vm["output-file"].as<std::vector<std::string> >() << "\n";
      std::cout<<"output files are:\n"<<output_files<<"\n";
    }
    else
    {
      std::cout<<"warning: no output filename specified.";
      std::cout<<"output shall be redirected to the standard output\n";
      output_files.resize(input_files.size());
    }

    if(vm.count("search-path"))
    {
      std::cout<<"search paths:\n"<<search_paths;
    }
    else
    {
      std::cerr<<"no search path provided the tool will search only"
               <<" in the present directory\n";
    }

    if(vm.count("macro-stat-file"))
    {
      macro_list_file = output_directory + "/" + macro_list_file;
      mac_stat_file.open(macro_list_file);
      if(!mac_stat_file.is_open())
        std::cerr <<"file to list macros: "<<macro_list_file
                  <<" could not be opened, "
                  <<"redirecting the list to std::cerr";
    }

    if(vm.count("log-file"))
    {
      log_file = output_directory + "/" + log_file;
      plog_file.open(log_file);
      if(plog_file.is_open())
        err_outstream = &plog_file;
      else {
        std::cerr <<"log-file: "<<log_file
                  <<" could not be opened, "
                  <<"redirecting the errors/warnings to std:cerr";
      }
    }

    if(vm.count("stat-file"))
    {
      stat_file = output_directory + "/" + stat_file;
      pstat_file.open(stat_file);
      if(pstat_file.is_open())
        stat_outstream = &pstat_file;
      else {
        std::cerr <<"stat-file: "<<stat_file
                  <<" could not be opened, "
                  <<"redirecting the errors/warnings to std::cerr";
      }
    }

    if(vm.count("validator-file"))
    {
      // if reading of validator_file is successful
      std::cout <<"the validator-file is: "<<validator_file<<"\n";
    }

    if(vm.count("global-macros-raw"))
    {
      //global_macros_raw = vm["global-macros-raw"].as<std::string>();
      std::cout << "file containing global macro is: "
                << global_macros_raw
                << "\nthe global macros after being parsed will be kept"
                << "in file gMacros.dat in a formatted form, this file can be"
                << "used later to avoid parsing of global macros all the time"
                << "when no new global macros are added.\n";
    }

#ifdef BUILD_NEW_MACRO_LIST
    else
    {
      std::cerr<<"when BUILD_NEW_MACRO_LIST is defined,"
               <<"a file containing global macros"
               <<"should be specified. exiting...\n";
      std::cerr<<config_file_options;
      return -1;
    }
#endif

    if(vm.count("global-macros-formatted"))
    {
      //global_macros_formatted = vm["global-macros-formatted"].as<std::string>();
      std::cout << "file containing formatted global macro is: "
                << global_macros_formatted << "\n";
    }

#ifndef BUILD_NEW_MACRO_LIST
    else
    {
      std::cerr<<"when BUILD_NEW_MACRO_LIST is not defined,"
               <<"a file containing global formatted macros"
               <<"should be specified. exiting...\n";
      std::cerr<<config_file_options;
      return -1;
    }
#endif

    if(vm.count("input-directory"))
    {
      //input_directory = vm["input-directory"].as<std::string>();
      std::cout << "files to be taken from directory: " << input_directory<<"\n";
    }

    if(vm.count("output-directory"))
    {
      //output_directory = vm["output-directory"].as<std::string>();
      std::cout << "files to be put to directory: " << output_directory<<"\n";
    }

    if(vm.count("verbose"))
    {
      std::cout << "verbosity enabled.  Level is " << vm["verbose"].as<int>()
                << "\n";
    }

    if(vm.count("ignore-macros"))
    {
      std::cout << "macros to prevent demacrofication are:\n"
                << macros_preventing_demacrofication
                << "\n";
    }

    if(vm.count("backup-directory"))
    {
      std::cout << "backup directory: "
                << backup_directory
                << "\n";
    }

    if(vm.count("cleanup-directory"))
    {
      std::cout << "cleanup directory: "
                  << cleanup_directory
                  << "\n";
    }

    if(vm.count("make-command"))
    {
      std::cout << "make command: "
                << make_command
                << "\n";
    }
    else
    {
      std::cout<<"no make command/script provided. "
               <<"default command `make` will be used\n";
    }

    if(vm.count("demacrofication-granularity"))
    {
      if(demacrofication_granularity.compare("OneFileAtATime") &&
          demacrofication_granularity.compare("OneMacroAtATime")) {
        std::cerr<<"error: invalid value for demacrofication_granularity\n"
                 <<config_file_options;
        return -1;
      }
      std::cout << "demacrofying: "
                << demacrofication_granularity
                << "\n";
    }

/**     ***********     END PRORAM OPTIONS   ************/

    ConfigScheme pConfigScheme;// = new ConfigScheme();

    //set file manager scheme should be called before
    //the set demcrofication scheme because the demacrofication scheme
    //uses the names of files (e.g. validator_file)
    pConfigScheme.SetFileManagerScheme(input_files,
                                        output_files,
                                        search_paths,
                                        input_directory,
                                        output_directory,
                                        backup_directory,
                                        cleanup_directory,
                                        validator_file,
                                        err_outstream,
                                        stat_outstream,
                                        &mac_stat_file
                                        );
    pConfigScheme.SetDemacroficationScheme(demacrofication_granularity,
                                            macros_preventing_demacrofication,
                                            enable_warning,
                                            global_macros_raw,
                                            global_macros_formatted,
                                            multiple_definitions,
                                            false //passing false until a separate cleanup tool is developed
                                            );
    pConfigScheme.SetBuildScheme(make_command);

    //step1. load the Overseer class with the configuration scheme
    Overseer* pOverseer = new Overseer(pConfigScheme);
    //step2. start processing
    pOverseer->StartProcessing(!no_translate);
    std::cout<<"\ndone...\n";


    ///@todo when global_macros_formatted is provided use that one
    plog_file.close();
    pstat_file.close();
    delete pOverseer;
//    delete pConfigScheme;

  } catch(const char* s) {
      std::cerr<<"Error:"<<s<<std::endl;
  } catch(ExceptionHandler& e){
      std::cerr<<"Exception Handler: "<<e.GetMessage();
  } catch ( const boost::program_options::multiple_occurrences& e ) {
        std::cerr << e.what() << " from option: " << e.get_option_name() << "\n";
  } catch(std::exception& e) {
      std::cerr<<"std::exception: "<<e.what();
  } catch(...) {
      std::cerr<<"unknown error in the program\n";
  }
  return 0;
}
