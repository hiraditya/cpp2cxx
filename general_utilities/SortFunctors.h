#ifndef SORTFUNCTORS_H
#define SORTFUNCTORS_H
/*
 *  @author Aditya Kumar 
 *
 *  This file is distributed under the MIT License. See 
 *  LICENCE.txt attached with this project or visit
 *  http://www.opensource.org/licenses/mit-license.php 
 *  for terms and conditions.
 */

#include<string>
/*
bool SortString(std::string const& s1,std::string const& s2){
  return s1.compare(s2) < 0;
}
*/
namespace general_utilities {
  struct SortString {
    bool operator()(std::string const& s1,std::string const& s2) const
    {
      return s1.compare(s2) < 0;
    }
  };

}
#endif // SORTFUNCTORS_H
