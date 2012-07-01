/**
cpp2cxx is an open source software distributed under terms of the
Apache2.0 licence.

Copyrights remain with the original copyright holders.
Use of this material is by permission and/or license.

Copyright [2012] Aditya Kumar, Andrew Sutton, Bjarne Stroustrup

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
