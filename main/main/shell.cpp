#include "shell.h"

const CmdType Shell::defaultCmds[4] = {
  {"help", "display help message", doHelp},
  {"version", "display version",  showVersion},
  {"time", "show current time", showTime},
  {"led", "turn led on or off", ledControl}
};


Shell::Shell(CmdType custCmds)
{
  pCustCmds = &custCmds;
}

Shell::Shell(void)
{
}

void Shell::putChar(char ch)
{
  input += ch;  

  if (ch != '\n')
  {
    return;
  }

  parseString(input);
  input = "";
}

void Shell::putString(String str)
{
  parseString(str);
}

void Shell::parseString(String str)
{ 
  int i;
  int num;
  String strip;
  const CmdType  *pCurrentCmd;

  num = sizeof(defaultCmds) / sizeof(CmdType);
  for(i = 0; i < num; i++)
  {
    if (str.startsWith(defaultCmds[i].cmd) == true) 
    {
       break;
    }
  }

  if (i >= num)
  {
    traceInfo( "unrecognized command :\r\n");
    return; 
  }
  
  // now we have hit a cmd
  pCurrentCmd = &defaultCmds[i];

  // we are going to get the command parameter via strip
  strip = str;
  int inputLen = str.length();
  int cmdLen = pCurrentCmd->cmd.length();
  // when there are additinal characters
  // we are going to do strip
  if (inputLen > cmdLen)
  {
    strip.remove(0,cmdLen);
    String drop = " ";
    strip.trim();
  }
  defaultCmds[i].func(strip);
}

void Shell::doHelp(String param)
{
   int i;

   traceInfo("usage:\r\n");
   traceInfo("=============\r\n");

   for(i = 0; i < sizeof(Shell::defaultCmds) / sizeof(CmdType); i++)
   {
      traceInfo("\t\t" + Shell::defaultCmds[i].help + "\r\n");
   }
}

void Shell::showVersion(String param)
{
  traceInfo("ver 0.1\r\n"); 
  traceInfo("build at: 2016.Jun.18, 13:46\r\n"); 
  traceInfo("author : shmayunfei@qq.com\r\n");
}

void Shell::showTime(String param)
{
   traceInfo( "22:05\r\n" );
}

void Shell::ledControl(String param)
{
  if(param == "on")
  {
     traceInfo("turn led on...\r\n"); 
  }
  else if (param == "off")
  {
     traceInfo("turn led off...\r\n");
  }
}

