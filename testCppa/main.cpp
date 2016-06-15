#include <iostream>
#include "trace.h"


int main()
{
  Trace logger(TRACE_DEBUG_LEVEL_INFO);

  logger.traceInfo("hello world!\r\n");

  logger.traceWarn("Compile at 2016.June.15\r\n");

  return 0;
}
