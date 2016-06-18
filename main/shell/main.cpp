#include "shell.h"

volatile int g_exit = 1;

void emptyCmd(string param){

}

const CmdType custCmd[2] = {
  {"test", "test cust cmd", emptyCmd},
  {"test2", "another cmd", emptyCmd}
};

int main()
{
  Shell sh;
  string input;

  while(g_exit){
    cout << "please input cmd:";
    cin >> input;
    cout << endl;
    sh.putString(input);
  }
  return 0;

}
