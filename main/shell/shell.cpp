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
  input.clear();
}

void Shell::putString(string str)
{
  parseString(str);
}

void Shell::parseString(string str)
{ 
  int i;
  int num;
  string strip;
  const CmdType  *pCurrentCmd;

  num = sizeof(defaultCmds) / sizeof(CmdType);
  for(i = 0; i < num; i++)
  {
    if (str.compare(0, defaultCmds[i].cmd.length(), defaultCmds[i].cmd) == 0) 
    {
       break;
    }
  }

  if (i >= num)
  {
    cout << "unrecognized command :" << str << endl;
    str.clear();
    return; 
  }
  
  // now we have hit a cmd
  pCurrentCmd = &defaultCmds[i];

  // we are going to get the command parameter via strip
  strip = str;
  int len = str.length();
  // when there are additinal characters
  // we are going to do strip
  if (len > pCurrentCmd->cmd.length())
  {
    strip = str.at(len);
    string drop = " ";
    strip.erase(0,
      strip.find_first_not_of(drop));
  }
  defaultCmds[i].func(strip);
}

void Shell::doHelp(string param)
{
   int i;

   cout << "usage:" << endl; 
   cout << "==========" << endl; 

   for(i = 0; i < sizeof(Shell::defaultCmds) / sizeof(CmdType); i++)
   {
      cout << Shell::defaultCmds[i].cmd; 
      cout << "\t\t" << Shell::defaultCmds[i].help;
      cout << endl;
   }
}

void Shell::showVersion(string param)
{
  cout <<"ver 0.1" << endl; 
  cout <<"build at:" << __DATE__ << "\t" << __TIME__ << endl; 
  cout << "shmayunfei@qq.com" << endl;
}

void Shell::showTime(string param)
{
  cout << "22:05" << endl;
}

void Shell::ledControl(string param)
{
  if(param == "on")
  {
     cout << "turn led on..." << endl; 
  }
  else if (param == "off")
  {
     cout << "turn led off..." << endl;
  }
}
