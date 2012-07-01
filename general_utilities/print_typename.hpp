#ifndef PRINT_TYPENAME_HPP
#define PRINT_TYPENAME_HPP

/*
 *  @author Aditya Kumar 
 *
 *  This file is distributed under the MIT License. See 
 *  LICENCE.txt attached with this project or visit
 *  http://www.opensource.org/licenses/mit-license.php 
 *  for terms and conditions.
 */


#include<typeinfo>
#include<iostream>


template<typename T>
void print_typeref(std::ostream& os, T const& x)
{
  os<<typeid(x).name();
}


template<typename T>
void print_type(std::ostream& os, T x)
{
  os<<typeid(x).name();
}

#endif //PRINT_TYPENAME_HPP
