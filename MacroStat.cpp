#include "MacroStat.h"
#include "Macro.h"
#include<iomanip>
#include<algorithm>
static unsigned int macro_count = 0;
std::ostream& operator<<(std::ostream& os,MacroStat const& m_stat)
{
  os<< "- macro"
    << std::setw(sizeof(double))
    << std::setfill('0')
    << macro_count++<<":\n"
    <<"  - m_id : "<<m_stat.id_string<<"\n"
    <<"  - m_cat: "<<m_stat.m_cat<<"\n"
    <<"  - c_cat: "<<m_stat.rl_ccat<<"\n"
    <<"  - d_cat: "<<m_stat.rl_dcat<<"\n";
  return os;
}
