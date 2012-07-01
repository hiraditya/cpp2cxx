/*
 *  @author Aditya Kumar 
 *
 *  This file is distributed under the MIT License. See 
 *  LICENCE.txt attached with this project or visit
 *  http://www.opensource.org/licenses/mit-license.php 
 *  for terms and conditions.
 */

#include "print_typename.hpp"

#include<iostream>


int main()
{
  print_type(std::cout, "name");
  std::cout<<std::endl;
  print_typeref(std::cout, "name");
  std::cout<<std::endl;
  return 0;
}
