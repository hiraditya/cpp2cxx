#ifndef UTILS_VECTOR_HPP
#define UTILS_VECTOR_HPP
/*
 *  @author Aditya Kumar 
 *
 *  This file is distributed under the MIT License. See 
 *  LICENCE.txt attached with this project or visit
 *  http://www.opensource.org/licenses/mit-license.php 
 *  for terms and conditions.
 */


#include<vector>
#include<iostream>
namespace general_utilities{
  // A helper function to simplify the main part.
    template<typename T>
    std::ostream& operator<<(std::ostream& os,const std::vector<T>& v)
    {
      for(typename std::vector<T>::const_iterator i = v.begin(); i!= v.end(); ++i){
        os<<*i;
        os<<"\n";
      }
      return os;
    }

#ifdef USE_ITERATORS
#include<iterator>

  template<class T>
  std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
  {
      std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, "\n"));
      return os;
  }

#endif

}
#endif // UTILS_VECTOR_HPP
