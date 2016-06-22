#ifndef shell_h
#define shell_h
#include "arduino.h"
#include "trace.h"

typedef void (*CmdFuncType)(String param);

struct CmdType {
  String cmd;

  String help;

  CmdFuncType func;
};


class Shell : public Trace{

  public:
  Shell(const CmdType *custCmds);
  Shell(void);
  void putChar(char ch);
  void putString(String str);
 
  private:
    void parseString(String str);

    static void doHelp(String param);
    static void showVersion(String param);
    static void showTime(String param);
    static void ledControl(String param);
    static void gsmInit(String param);
    static void gsmSMS(String param);

    String input;


    static const CmdType defaultCmds[];
    static const CmdType *pCustCmds;

};

#endif

