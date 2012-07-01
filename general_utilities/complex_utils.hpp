#ifndef UTILS_COMPLEX_HPP
#define UTILS_COMPLEX_HPP
/*
 *  @author Aditya Kumar 
 *
 *  This file is distributed under the MIT License. See 
 *  LICENCE.txt attached with this project or visit
 *  http://www.opensource.org/licenses/mit-license.php 
 *  for terms and conditions.
 */

#include<complex>
#include<iostream>

#include<iomanip>
namespace general_utilities{
  template<typename T>
  std::ostream& operator<<(std::ostream& os, std::complex<T> c)
  {
    os<<'('<<c.real()<<","<<c.imag()<<')';
    return os;
  }
}

#endif // UTILS_COMPLEX_HPP
