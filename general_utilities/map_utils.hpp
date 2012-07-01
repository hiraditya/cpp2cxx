#ifndef UTILS_MAP_HPP
#define UTILS_MAP_HPP
/*
 *  @author Aditya Kumar 
 *
 *  This file is distributed under the MIT License. See 
 *  LICENCE.txt attached with this project or visit
 *  http://www.opensource.org/licenses/mit-license.php 
 *  for terms and conditions.
 */


#include<map>
#include<iostream>
namespace general_utilities{
  template<typename T1,typename T2>
  std::ostream& operator<<(std::ostream& os,const std::map<T1,T2>& m)
  {
    for(typename std::map<T1,T2>::const_iterator i = m.begin(); i!= m.end(); ++i){
      os<<i->first;
      os<<"\t";
      os<<i->second;
      os<<"\n";
    }
    return os;
  }
}
#endif // UTILS_MAP_HPP
