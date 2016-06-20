#include "shell.h"

const CmdType Shell::defaultCmds[4] = {
  {"help", "display help message", doHelp},
  {"version", "display version",  showVersion},
  {"time", "show current time", showTime},
  {"led", "turn led on or off", ledControl}
};


Shell::Shell(const CmdType *custCmds)
{
  pCustCmds = custCmds;
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
  int i = 0,j = 0;
  int num;
  String strip, cmd;
  const CmdType  *pCurrentCmd;
  bool custCmdsFound = false;

  num = sizeof(defaultCmds) / sizeof(CmdType);
  for(i = 0; i < num; i++)
  {
    if (str.startsWith(defaultCmds[i].cmd) == true) 
    {
        // now we have hit a cmd
       pCurrentCmd = &defaultCmds[i];
       break;
    }
  }

  j = 0;
  while(1)
  {
    cmd = pCustCmds[j].cmd;
    if (cmd.startsWith("endOfCmd"))
    {
        custCmdsFound = false;
        break;  
    }
    if (str.startsWith(cmd) == true) 
    {
       custCmdsFound = true;
       pCurrentCmd = &pCustCmds[j];
       break;
    }
    j++;
  }

  if ((i >= num) && (custCmdsFound == false))
  {
    //trace( "unrecognized command :\r\n");
    return; 
  }  

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
  pCurrentCmd->func(strip);
}

void Shell::doHelp(String param)
{
   int i,j;

   trace("\r\nusage:\r\n");
   trace("=============\r\n");

   for(i = 0; i < sizeof(Shell::defaultCmds) / sizeof(CmdType); i++)
   {
      trace(Shell::defaultCmds[i].cmd);
      trace("\t\t" + Shell::defaultCmds[i].help + "\r\n");
   }

/*
   while(1)
   {
      if (Shell::pCustCmds[j].cmd.startsWith("endOfCmd"))
      {
          break;  
      }
      trace(Shell::pCustCmds[j].cmd);
      trace("\t\t" + Shell::pCustCmds[j].help + "\r\n");
    }*/
}

void Shell::showVersion(String param)
{
  String dateTime = __DATE__ " " __TIME__;
  trace("ver 0.1\r\n"); 
  trace("build at:" + dateTime + "\r\n"); 
  trace("author : shmayunfei@qq.com\r\n");
}

void Shell::showTime(String param)
{
   trace( "22:05\r\n" );
}

void Shell::ledControl(String param)
{
  if(param == "on")
  {
     trace("turn led on...\r\n"); 
     digitalWrite(8, HIGH);
  }
  else if (param == "off")
  {
     trace("turn led off...\r\n");
     digitalWrite(8, LOW);
  }
}



