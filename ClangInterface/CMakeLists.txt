cmake_minimum_required(VERSION 2.8)
project(ASTConsumer)

include_directories("${CMAKE_SOURCE_DIR}")

add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-D__STDC_LIMIT_MACROS)
ADD_DEFINITIONS(-D_GNU_SOURCE)
#add_definitions(-DCLANG_AST_DEBUG)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}  "-std=c++0x -fno-rtti -g")
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}  "-cc1")

link_directories("${CFE_PREFIX_PATH}/lib")

add_library(ASTConsumer SHARED
    TrackMacro.cpp
    ASTConsumer.cpp
  )

target_link_libraries(ASTConsumer  
 libclang
  )

