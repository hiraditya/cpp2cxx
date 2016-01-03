// Definitions.

#include "debug.h"

std::ostream *dbg_stream = &std::cout;

std::ostream& dbgs()
{
  return *dbg_stream;
}

void set_dbg_stream(std::ostream &os)
{
  dbg_stream = &os;
}
