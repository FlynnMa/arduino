#include "Arduino.h"
#include "trace.h"

const int Trace::traceDebugLevel = TRACE_DEBUG_LEVEL_INFO;

void Trace::traceInfo(String str)
{
  if (traceDebugLevel == TRACE_DEBUG_LEVEL_INFO)
  {
    Serial.println("INFO:" + str);  
  }
}

void Trace::traceWarn(String str)
{
  if (traceDebugLevel <= TRACE_DEBUG_LEVEL_WARN)
  {
    Serial.println("WARN:" + str);  
  }
  
}

void Trace::traceError(String str)
{
  if (traceDebugLevel <= TRACE_DEBUG_LEVEL_ERR)
  {
    Serial.println("ERR:" + str);  
  }
}

void Trace::traceFatal(String str)
{
  Serial.println("FATAL:" + str);  
  while(1);
}

void Trace::trace(String str)
{
  Serial.print(str);  
}



