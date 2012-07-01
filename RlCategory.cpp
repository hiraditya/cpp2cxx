#include "RlCategory.h"
//TODO: have to change the nomenclature at every place
//
std::ostream& operator<<(std::ostream& os,RlDCat const& cat)
{
  switch(cat){
    case RlDCat::dependent:
      os<<"dependent";
      break;
    case RlDCat::independent:
      os<<"closed"; //previously "independent";
      break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os,RlCCat const& cat)
{
  switch(cat){
    case RlCCat::closed:
      os<<"complete"; //previously "closed";
      break;
    case RlCCat::open:
      os<<"partial"; //previously "open";
      break;
  }
  return os;
}
