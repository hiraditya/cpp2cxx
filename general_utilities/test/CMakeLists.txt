CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(test)

SET(CMAKE_CXX_FLAGS "-pedantic -Wall -g -std=c++0x")
 SET(FOOL_DIR /home/vmplanet/Documents/examples/cplusplus/fool/trunk/)
INCLUDE_DIRECTORIES("${FOOL_DIR}/general_utilities")


ADD_EXECUTABLE(test
  #testprint_typename.cpp
  test_sizeof.cpp
  )
