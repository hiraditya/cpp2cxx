#ifndef UTILS_SET_HPP
#define UTILS_SET_HPP
/*
 *  @author Aditya Kumar 
 *
 *  This file is distributed under the MIT License. See 
 *  LICENCE.txt attached with this project or visit
 *  http://www.opensource.org/licenses/mit-license.php 
 *  for terms and conditions.
 */





#include<iostream>
#include<iterator>
#include<set>

namespace general_utilities{
  // A helper function to simplify the main part.
  template<class T>
  std::ostream& operator<<(std::ostream& os, const std::set<T>& s)
  {

      //std::for_each(s.begin(), s.end(), [&os](const T& t){os<<t<<"\n";});
      std::copy(s.begin(), s.end(), std::ostream_iterator<T>(os, "\n"));
      return os;
  }
}
#endif // PRINTSET_H
