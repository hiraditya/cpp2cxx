#ifndef UTILS_FILETYPE_HPP
#define UTILS_FILETYPE_HPP

/*
 *  @author Aditya Kumar 
 *
 *  This file is distributed under the MIT License. See 
 *  LICENCE.txt attached with this project or visit
 *  http://www.opensource.org/licenses/mit-license.php 
 *  for terms and conditions.
 */



#include "string_utils.hpp"

#include <string>

namespace general_utilities {
bool header_file(std::string const& filename)
{
  if(ends_with(filename, ".h")
      || ends_with(filename, ".hpp"))
      return true;
  return false;
}

bool cpp_file(std::string const& filename)
{
  if(ends_with(filename, ".cpp")
      || ends_with(filename, ".cxx")
      || ends_with(filename, ".C"))
      return true;
  return false;

}
}
#endif // FILETYPE_HPP
