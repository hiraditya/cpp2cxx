#ifndef UTILS_STRING_HPP
#define UTILS_STRING_HPP
/*
 *  @author Aditya Kumar 
 *
 *  This file is distributed under the MIT License. See 
 *  LICENCE.txt attached with this project or visit
 *  http://www.opensource.org/licenses/mit-license.php 
 *  for terms and conditions.
 */


#include<string>
namespace general_utilities{
// instead of all these function i can use my mapper
// and then apply the predicate to each character in the string

/// \brief removes the character pointed to by c_ptr
/// and returns a new string.
inline std::string remove_char(std::string const& str, const char* c_ptr)
{
  std::string::size_type i = 0;
  std::string s_new(str,0,str.size()-1);
  for( ; i < str.size(); ++i)
    if(*c_ptr == str[i])
      break;
  for(; i < str.size()-1; ++i)
    s_new[i] = str[i+i];
return s_new;
}

/// @brief removes all occurences of character c in string str
inline std::string remove_char(std::string const& str, char c)
{
  std::string::size_type i = 0;
  std::string s_new;
  for(;i<str.size();++i)
    if(str[i] != c)
      s_new += str[i];
  return s_new;
}

inline std::string keep_alpha_numeric(std::string const& str)
{
  std::string::size_type i = 0;
  std::string s_new;
  for(;i<str.size();++i)
    if(std::isalnum(str[i]))
      s_new += str[i];
  return s_new;
}

inline bool ends_with(std::string const& str, std::string const& suffix)
{
  auto str_size = str.size();
  auto suf_size = suffix.size();
  for(std::string::size_type i = 0; i< suffix.size();++i){
    if(str[str_size-i-1] != suffix[suf_size-i-1])
      return false;
  }
  return true;
}


struct SortString {
  bool operator()(std::string const& s1,std::string const& s2) const
  {
    return s1.compare(s2) < 0;
  }
};

}
#endif // UTILS_STRING_HPP
