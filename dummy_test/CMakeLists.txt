CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(dummy_test)
# Compiler options
IF(CMAKE_COMPILER_IS_GNUCXX)
  MESSAGE(STATUS "GCC detected - Adding compiler flags: -pedantic -O2 -std=c++0x")
  SET(CMAKE_CXX_FLAGS "-pedantic -g -Wall -std=c++0x -imacros Defined.h")
ELSEIF(MSVC)
  MESSAGE(STATUS "MSVC detected - Adding compiler flags")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

##############RELATED TO CLANG#########################
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

ADD_EXECUTABLE(dummy_test
              example.cpp
              )

