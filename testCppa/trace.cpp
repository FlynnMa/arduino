#include <iostream>
#include "trace.h"


Trace::Trace(int level)
{
  traceDebugLevel = level;
}

void Trace::traceInfo(string str)
{
  if (traceDebugLevel == TRACE_DEBUG_LEVEL_INFO)
  {
    cout << "INFO:" + str;  
  }
}

void Trace::traceWarn(string str)
{
  if (traceDebugLevel <= TRACE_DEBUG_LEVEL_WARN)
  {
    cout << "WARN:" + str;  
  }
  
}

void Trace::traceError(string str)
{
  if (traceDebugLevel <= TRACE_DEBUG_LEVEL_ERR)
  {
    cout << "ERR:" + str;  
  }
}

void Trace::traceFatal(string str)
{
  cout << "FATAL:" + str;  
  while(1);
}

void Trace::trace(string str)
{
  cout << str;  
}


