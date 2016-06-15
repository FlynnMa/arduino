#ifndef trace_h
#define trace_h
#include <string>

using namespace std;

const int TRACE_DEBUG_LEVEL_INFO   = 0;
const int TRACE_DEBUG_LEVEL_WARN   = 1;
const int TRACE_DEBUG_LEVEL_ERR    = 2;
const int TRACE_DEBUG_LEVEL_FATAL  = 3;






class Trace
{
  public:
    Trace(int level);
    void traceInfo(string str);
    void traceWarn(string str);
    void traceError(string str);
    void traceFatal(string str);
    void trace(string str);
  private:
    int traceDebugLevel;

};


#endif
