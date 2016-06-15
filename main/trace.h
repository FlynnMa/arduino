#ifndef trace_h
#define trace_h

#include "Arduino.h"

const int TRACE_DEBUG_LEVEL_INFO   = 0;
const int TRACE_DEBUG_LEVEL_WARN   = 1;
const int TRACE_DEBUG_LEVEL_ERR    = 2;
const int TRACE_DEBUG_LEVEL_FATAL  = 3;






class Trace
{
  public:
    Trace(int level);
    void traceInfo(String str);
    void traceWarn(String str);
    void traceError(String str);
    void traceFatal(String str);
    void trace(String str);
  private:
    int traceDebugLevel;

};


#endif
