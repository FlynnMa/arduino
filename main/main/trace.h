#ifndef trace_h
#define trace_h

#include "Arduino.h"

#define TRACE_DEBUG_LEVEL_INFO   0
#define TRACE_DEBUG_LEVEL_WARN   1
#define TRACE_DEBUG_LEVEL_ERR    2
#define TRACE_DEBUG_LEVEL_FATAL  3

class Trace
{
  public:
    Trace(int level);
    Trace();
    void traceInfo(String str);
    void traceWarn(String str);
    void traceError(String str);
    void traceFatal(String str);
    void trace(String str);
  private:
    int traceDebugLevel;

};


#endif

