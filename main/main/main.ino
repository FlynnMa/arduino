#include <core_build_options.h>
#include <swRTC.h>

#include "trace.h"
#include "shell.h"

#include "pitches.h"
#include <SoftwareSerial.h>
//TODO change softwareSerial to newSoftwareSerial

void gsmInit(String param);
void gsmSMS(String param);
void softserialCheck(void);
void emptyFunc(String param){
  
  }

void gsmInit(String param);
void gsmSMS(String param);
void ledControl(String param);
void setDate(String param);
void setTime(String param);
void showTime(String param);
void playTone(String param);
void playMelody(String param);


/*
 * Defintions
*/
const int pinLEDYellow = 13;
const int pinLEDRed = 8;
const int pinButton = 9;
const int pinTone = 4;
const int pinMelody = 5;
bool replyGSM = false;
String inputGSM = "";
volatile bool waitSendSms = false;
unsigned long toneEndTime = 0;
int melodyIdx = -1; /* -1 means melody stop, >=0 melody is playing at certain index */
int melodyEndTime = 0;
swRTC rtc;

SoftwareSerial gsmPort(2,3);

const CmdType custCommands[] = {
  {"gsm", "start gsm module", gsmInit},
  {"sms", "send SMS", gsmSMS},
  {"led", "control led on or off", ledControl},
  {"setDate", "set date, formate dd-mm-yyyy", setDate},
  {"setTime", "set time, formate hh:mm:ss", setTime},
  {"rtc", "show RTC time", showTime},
  {"tone", "play a tone", playTone},
  {"melody", "play melody", playMelody},
  {"endOfCmd", "end of commands", emptyFunc},
};

const char compile_date[] = __DATE__ " " __TIME__;
Shell sh(custCommands);

void setupRTC()
{
  rtc.stopRTC(); // stop the RTC
  rtc.setTime(0,0,0); // init rtc
  rtc.setDate(21,6,2016);
  rtc.startRTC(); 
}

/*
 * format dd-mm-yyyy
 * @param param[i] input the date string as format date-month-year
*/
void setDate(String param)
{
  int year;
  byte day, month;
  
  param.trim();
  day = param.substring(0,2).toInt();
  month = param.substring(3,5).toInt();
  year = param.substring(6).toInt();

  Trace::traceInfo("input :" + param + "\r\n");
  String dbg, d(day), m(month), y(year);
  dbg = "date converted:" + d + m + y;
  Trace::traceInfo(dbg);
  rtc.setDate(day,month,year);

}

void setTime(String param)
{
  byte h,m,s;
  
  param.trim();
  h = param.substring(0,2).toInt();
  m = param.substring(3,5).toInt();
  s = param.substring(6).toInt();

  Trace::traceInfo("input :" + param + "\r\n");
  String dbg, hs(h), ms(m), ss(s);
  dbg = "time converted:" + hs + ms + ss;
  Trace::traceInfo(dbg);
  rtc.setTime(h,m,s);

}

void showTime(String param)
{
  String dateStr("date:"), timeStr("time:");
  byte d,m,h,min,s;
  int y;

  d = rtc.getDay();
  m = rtc.getMonth();
  y = rtc.getYear();
  dateStr += d;
  dateStr += "-";
  dateStr += m;
  dateStr += "-";
  dateStr += y;
  dateStr += "\n";

  h = rtc.getHours();
  min = rtc.getMinutes();
  s = rtc.getSeconds();
  timeStr += h;
  timeStr += ":";
  timeStr += min;
  timeStr += ":";
  timeStr += s;
  timeStr += "\n";
  Trace::trace(dateStr);
  Trace::trace(timeStr);
}

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

  pinMode(pinTone, OUTPUT);
  digitalWrite(pinTone, LOW);

  pinMode(pinMelody, OUTPUT);

  Serial.begin(115200);

  if (Serial) {
    Serial.write("keyfoot technolodgy!\r\n");
  }
  Trace::trace("author : shmayunfei@qq.com\r\n");
  String str = compile_date + "\r\n";
  Trace::trace(str);

  setupRTC();

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
    playTone("500");
  }

  stopToneAuto();
  stopMelodyAuto();
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

void ledControl(String param)
{
  if(param == "on")
  {
     Trace::trace("turn led on...\r\n"); 
     digitalWrite(8, HIGH);
  }
  else if (param == "off")
  {
     Trace::trace("turn led off...\r\n");
     digitalWrite(8, LOW);
  }
}

/*
 * Automatically stop tone when time out
 * 
 */
void stopToneAuto()
{
  unsigned long currentTime;

  /* return when the tone is not playing */
  if (toneEndTime == 0)
    return;

  currentTime = millis();
  if (currentTime >= toneEndTime)
  {
      digitalWrite(pinTone, LOW);
      currentTime = 0;
  }
}

/*
 * input param as the duration of tone
 *
 * @param param[i] tone play duration in milli-seconds
 */
void playTone(String param)
{
  int duration = param.toInt();

  Trace::traceInfo("Play a tone...\r\n");
  toneEndTime = millis() + duration;
  digitalWrite(pinTone, HIGH);
  
}


/*--------------------------------------------------------------*/
/*----------------------Melody----------------------------------*/
/*--------------------------------------------------------------*/
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

/* define melody durations in milliseconds */
int melodyDurations[] = {
  200, 400, 400, 200, 200, 200, 200, 200
};

void stopMelodyAuto(void)
{
  /* if melody is not playing */
  if (melodyIdx == -1)
    return;

  if (millis() < melodyEndTime)
  {
    return;
  }

  /* check if end play */
  if (melodyIdx >= sizeof(melody)/sizeof(int)){
    noTone(pinMelody);
    melodyIdx = -1;
    melodyEndTime = 0;
    return;

  }

  /* play next tone */
  melodyIdx++;
  melodyEndTime = millis() + melodyDurations[0];
  noTone(pinMelody);
  tone(pinMelody, melody[melodyIdx]);
}

void playMelody(String param)
{
  Trace::traceInfo("play melody...");

  melodyEndTime = millis() + melodyDurations[0];
  melodyIdx = 0;
  tone(pinMelody, melody[melodyIdx]);
}



