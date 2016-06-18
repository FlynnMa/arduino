#ifndef shell_h
#define shell_h
#include <iostream>
#include <string>

using namespace std;

typedef void (*CmdFuncType)(string param);

struct CmdType {
  string cmd;

  string help;

  CmdFuncType func;
};


class Shell {

  public:
  Shell(CmdType custCmds);
  Shell(void);
  void putChar(char ch);
  void putString(string str);

  private:
    void parseString(string str);

    static void doHelp(string param);
    static void showVersion(string param);
    static void showTime(string param);
    static void ledControl(string param);

    string input;
    static const CmdType defaultCmds[4];
    CmdType *pCustCmds;

};

#endif
