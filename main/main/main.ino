#include "trace.h"
#include "shell.h"
#include <SoftwareSerial.h>

void gsmInit(String param);
void gsmSMS(String param);
void softserialCheck(void);
void emptyFunc(String param){
  
  }

void gsmInit(String param);
void gsmSMS(String param);


/*
 * Defintions
*/
const int pinLEDYellow = 13;
const int pinLEDRed = 8;
const int pinButton = 9;
bool replyGSM = false;
String inputGSM = "";
volatile bool waitSendSms = false;

SoftwareSerial gsmPort(2,3);

const CmdType custCommands[] = {
  {"gsm", "start gsm module", gsmInit},
  {"sms", "send SMS", gsmSMS},
  {"endOfCmd", "end of commands", emptyFunc},
};

const char compile_date[] = __DATE__ " " __TIME__;
Shell sh(custCommands);

/*
 *  Initialize generic functions
 *  
 */
void setup() {
  // put your setup code here, to run once:
  pinMode(pinLEDYellow, OUTPUT);

  // initialize customized Red LED
  pinMode(pinLEDRed, OUTPUT);

  // initialize button
  pinMode(pinButton, INPUT_PULLUP);

  Serial.begin(115200);

  if (Serial) {
    Serial.write("keyfoot technolodgy!\r\n");
  }
  Trace::trace("author : shmayunfei@qq.com\r\n");
  String str = compile_date;
  Trace::trace(str);

}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
    SerialReadInput();
}

/*
 * Flash yellow led
 * Show the usage of OUTPUT GPIO pin
 * 
 */
void flashYellowLED(void)
{
  digitalWrite(pinLEDYellow, HIGH);
  delay(500);
  digitalWrite(pinLEDYellow, LOW);
  delay(500);
}

/*
 * Flash yellow and red led
 * Demostrate connecting led manually
 */
void flashLED(void)
{
  digitalWrite(pinLEDYellow, HIGH);
  digitalWrite(pinLEDRed, LOW);
  delay(500);
  digitalWrite(pinLEDYellow, LOW);
  digitalWrite(pinLEDRed, HIGH);
  delay(500);
}

void serialReadLine()
{
  String inputStr;
  
  inputStr = Serial.readStringUntil('\n');
  sh.putString(inputStr);
}

void SerialReadInput()
{
  int len = Serial.available();
  char value = 0;
  
  while(len-- > 0) {
      value = (char)Serial.read();
//      Serial.print(value);
      sh.putChar(value);
  }

  
}

/*
 * main loop function
 */
void loop() {
  // put your main code here, to run repeatedly:
  int buttonValue;

  softserialCheck();
//  serialReadLine();

  buttonValue = digitalRead(pinButton);
  if (buttonValue == LOW) {
    flashLED();
  }
}


void softserialCheck(void)
{
  int len;
  char value;
  int endOfText = 0x1a;

  len = gsmPort.available();
  while (len-- > 0)
  {
    value = (char) gsmPort.read();
    Serial.print(value);
    inputGSM += value;
    if (value == '>') {
        gsmPort.write("012345678911234567892123456789312345678941234567895123456789612345678971234567898123456789\n");
        gsmPort.write(endOfText);
    }
  }
  if (inputGSM.indexOf("OK") > 0){
    replyGSM = true;
  }
}

void gsmInit(String param)
{
   int i;
   gsmPort.begin(9600);
   gsmPort.write("AT;\n");
   gsmPort.write("ATE1;\n");
   gsmPort.write("AT+CSCLK=0;\n");
   delay(100);
   gsmPort.write("AT+CMGF=1;\n");
   gsmPort.write("AT+CSCS=\"GSM\"\n");
   delay(50);
   gsmPort.write("AT+CSMP=17,167,2,241\n");
}

void gsmSMS(String param)
{  
   gsmPort.write("AT+CMGS=\"+phonenumber\"\n");
   waitSendSms = true;
   
}

