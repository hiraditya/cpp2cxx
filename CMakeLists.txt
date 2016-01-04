CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(demacrofier)

###############################################################################
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(HEADER_OUTPUT_PATH ${PROJECT_BINARY_DIR}/include)

SET(CFE_PREFIX_PATH "/home/hiraditya/work/llvm/install")

SET(UTILITIES_PREFIX_PATH ".")

INCLUDE_DIRECTORIES("${UTILITIES_PREFIX_PATH}/general_utilities")
INCLUDE_DIRECTORIES("${CFE_PREFIX_PATH}/include")
###############################################################################
#THIS HAS TO BE ADDED BEFORE SUPPLYING THE COMPILER FLAGS FOR THIS DIRECTORY FILES
#THE CLANG LIBRARIES COMPILE WITH THE -fno-rtti FLAGS
#AFTER THE LIBRARY HAS BEEN BUILT WE NEED TO SET THE COMPILER FLAGS ONCE AGAIN 
#BECAUSE THE BOOST LIBRARIES USE rtti
ADD_SUBDIRECTORY(ClangInterface)

###############################################################################
# Compiler options
SET(LLVM_CXXFLAGS "-fPIC -fvisibility-inlines-hidden")
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "-pedantic -Wall -std=c++11 -g ${LLVM_CXXFLAGS}")
  MESSAGE(STATUS "GCC detected - Adding compiler flags ${CMAKE_CXX_FLAGS}")
ELSEIF(MSVC)
  MESSAGE(STATUS "MSVC detected - Adding compiler flags")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

###############################################################################
#INCLUDE OTHER SUBDIRECTORIES HERE WHICH ARE FINE WITH -fno-rtti.
ADD_SUBDIRECTORY(general_utilities)

###############################################################################
INCLUDE_DIRECTORIES("${UTILITIES_PREFIX_PATH}/general_utilities")
INCLUDE_DIRECTORIES("${CFE_PREFIX_PATH}/include")

############## INCLUDE DIRECTORIES RELATED TO CLANG ###########################
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/ClangInterface")

############################ DEBUG MACROS #####################################
#ADD_DEFINITIONS(-DBUILD_NEW_MACRO_LIST)
ADD_DEFINITIONS(-DCLANG_AST_DEBUG_PRINT)
#ADD_DEFINITIONS(-DDEBUG_PARSER_PRINT)
#ADD_DEFINITIONS(-DDEBUG_CONDITIONALS_PRINT)
#ADD_DEFINITIONS(-DDEBUG_CONDITIONALS1_PRINT)
#ADD_DEFINITIONS(-DDEBUG_DEMACROFIER_PRINT)
#ADD_DEFINITIONS(-DDEBUG_RLPARSER_PRINT)
#ADD_DEFINITIONS(-DDEBUG_RLPARSER2_PRINT)
#ADD_DEFINITIONS(-DDEBUG_TREE_PRINT)
#ADD_DEFINITIONS(-DBUILD_NEW_MACRO_LIST_PRINT)
#ADD_DEFINITIONS(-DDEBUG_MACRO_DEPENDENCY_PRINT)
#ADD_DEFINITIONS(-DDEBUG_MACRO_DEPENDENCY2_PRINT)
#ADD_DEFINITIONS(-DENABLE_WARNING_PRINT)
#ADD_DEFINITIONS(-DDEBUG_MACRO_CLASS_PRINT)
#ADD_DEFINITIONS(-DDEBUG_MACRO_CLASS2_PRINT)
#ADD_DEFINITIONS(-DDEBUG_MACRO_USE_CASE_PRINT)
#ADD_DEFINITIONS(-DDEBUG_CLEANUP_CODE_PRINT)
#ADD_DEFINITIONS(-DDEBUG_VALIDATOR_PRINT)
ADD_DEFINITIONS(-DDEBUG_SUGGESTION_PRINT)
#ADD_DEFINITIONS(-DGMACROS="${HEADER_OUTPUT_PATH}/gMacros.dat")

############## DEFINITIONS RELATED TO CLANG ###################################
ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)
ADD_DEFINITIONS(-D__STDC_FORMAT_MACROS)
ADD_DEFINITIONS(-D__STDC_LIMIT_MACROS)
ADD_DEFINITIONS(-D_GNU_SOURCE)
###############################################################################
ADD_LIBRARY(cpp2cxx-core SHARED
                RlCategory.cpp
                ReplacementList.cpp
                RlParser.cpp
                Macro.cpp
                MacroStat.cpp
                UseCaseState.cpp
                DepGraph.cpp
                CondParser.cpp
                Demacrofier.cpp
                Parser.cpp
                FileManager.cpp
                Overseer.cpp
                ConfigScheme.cpp
              )

#################### SPECIFYING THE LINK DIRECTORIES ##########################
# Library directory paths
LINK_DIRECTORIES(
   "/usr/lib"
   "/usr/local/lib"
   "${CMAKE_PREFIX_PATH}/lib"
   "${CFE_PREFIX_PATH}/lib"
)

###############################################################################
ADD_EXECUTABLE(cpp2cxx-suggest
                MainSuggest.cpp
              )

ADD_EXECUTABLE(cpp2cxx-finalize
                MainFinalize.cpp
              )
FILE(COPY "cpp2cxx-validate.py" DESTINATION ${EXECUTABLE_OUTPUT_PATH})
FILE(COPY "gMacros.dat" DESTINATION ${HEADER_OUTPUT_PATH})
###############################################################################
#ADD_LIBRARY()
#FIND_LIBRARY()
TARGET_LINK_LIBRARIES(cpp2cxx-suggest cpp2cxx-core boost_system boost_wave
 boost_program_options ASTConsumer cpp2cxx-debug clang
)

TARGET_LINK_LIBRARIES(cpp2cxx-finalize cpp2cxx-core boost_system boost_wave
 boost_program_options ASTConsumer cpp2cxx-debug clang
)
