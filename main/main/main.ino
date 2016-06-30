#include <core_build_options.h>
#include <swRTC.h>

#include "trace.h"
#include "shell.h"

#include "pitches.h"
#ifdef __SUPPORT_GSM_MODULE__
#include <SoftwareSerial.h>
//TODO change softwareSerial to newSoftwareSerial
#endif //#ifdef __SUPPORT_GSM_MODULE__

void emptyFunc(String param){
  
  }

#ifdef __SUPPORT_GSM_MODULE__
void gsmInit(String param);
void gsmSMS(String param);
void softserialCheck(void);
#endif // #ifdef __SUPPORT_GSM_MODULE__

void ledControl(String param);
void setDate(String param);
void setTime(String param);
void showTime(String param);
void playTone(String param);
void playMelody(String param);
void cmdRGB(String rgbValueStr);
void cmdPlayRGB(String aniDuration);
void onAnimateRGB(void);
/*
 * Defintions
*/
const byte pinLEDYellow = 13;
const byte pinLEDRed = 8;
const byte pinButton = 9;
const byte pinTone = 4;
const byte pinMelody = 7;

#ifdef __SUPPORT_RGBLED__
const byte pinRGBR = 10;
const byte pinRGBG = 5;
const byte pinRGBB = 6;
#endif //__SUPPORT_RGBLED__

#ifdef __SUPPORT_GSM_MODULE__
bool replyGSM = false;
String inputGSM = "";
volatile bool waitSendSms = false;
#endif
unsigned long toneEndTime = 0;
byte melodyIdx = -1; /* -1 means melody stop, >=0 melody is playing at certain index */
int melodyEndTime = 0;
swRTC rtc;

#ifdef __SUPPORT_RGBLED__
bool animateRGB = false;
byte aniR, aniG, aniB; // animated r,g,b led value
#endif // __SUPPORT_RGBLED__

#ifdef __SUPPORT_GSM_MODULE__
SoftwareSerial gsmPort(2,3);
#endif // #ifdef __SUPPORT_GSM_MODULE__

const CmdType custCommands[] = {

#ifdef __SUPPORT_GSM_MODULE__
  {"gsm", "start gsm module", gsmInit},
  {"sms", "send SMS", gsmSMS},
#endif // #ifdef __SUPPORT_GSM_MODULE__

  {"led", "control led on or off", ledControl},
  {"setDate", "set date, formate dd-mm-yyyy", setDate},
  {"setTime", "set time, formate hh:mm:ss", setTime},
  {"rtc", "show RTC time", showTime},
  {"tone", "tone milli-seconds, play a tone in milliseconds", playTone},
  {"melody", "play melody", playMelody},
#ifdef __SUPPORT_RGBLED__
  {"rgb", "rgb rrr,ggg,bbb to set a rgb LED color", cmdRGB},
  {"playrgb", "playrgb milli-seconds, rgb as animation", cmdPlayRGB},
#endif //#ifdef __SUPPORT_RGBLED__
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

#ifdef __SUPPORT_RGBLED__
  pinMode(pinRGBR, OUTPUT);
  pinMode(pinRGBG, OUTPUT);
  pinMode(pinRGBB, OUTPUT);
  setRGBvalue(255,255,255);
#endif //#ifdef __SUPPORT_RGBLED__

  Serial.begin(115200);

  if (Serial) {
    Serial.write("keyfoot technolodgy!\r\n");
  }
  Trace::trace("https://github.com/marduino/arduino.git\n");
  Trace::trace("author : shmayunfei@qq.com\r\n");
//  Trace::trace("http://i.youku.com/keyfoot");
  String str = compile_date;
  str += "\n";
  Trace::trace(str);

  setupRTC();

  IRRemoteSetup();

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
      sh.putChar(value);
  }

  
}

/*
 * main loop function
 */
void loop() {
  // put your main code here, to run repeatedly:
  int buttonValue;

  IRRemoteMeasure();
  
#ifdef __SUPPORT_RGBLED__
  onAnimateRGB();
#endif //#ifdef __SUPPORT_RGBLED__

#ifdef __SUPPORT_GSM_MODULE__
  softserialCheck();
#endif // #ifdef __SUPPORT_GSM_MODULE__

  buttonValue = digitalRead(pinButton);
  if (buttonValue == LOW) {
    flashLED();
    playTone("500");
  }

  stopToneAuto();
  stopMelodyAuto();
}

#ifdef __SUPPORT_GSM_MODULE__
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
#endif // #ifdef __SUPPORT_GSM_MODULE__

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
      toneEndTime = 0;
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

  if (duration < 100)
    duration = 100;

  toneEndTime = millis() + duration;

  digitalWrite(pinTone, HIGH);
  
}


/*--------------------------------------------------------------*/
/*----------------------Melody----------------------------------*/
/*--------------------------------------------------------------*/
const int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

/* define melody durations in milliseconds */
const int melodyDurations[] = {
  200, 400, 400, 200, 200, 200, 200, 200
};

void stopMelodyAuto(void)
{
  /* if melody is not playing */
  if (melodyIdx == -1)
    return;

  /* if is playing but not reached the switching time */
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

  /* switch to next tone */
  melodyIdx++;
  melodyEndTime = millis() + melodyDurations[melodyIdx];
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

/******************************/
/* section RGB LED functions */
/******************************/
#ifdef __SUPPORT_RGBLED__
void setRGBvalue(byte r, byte g, byte b)
{
  analogWrite(pinRGBR, r);
  analogWrite(pinRGBG, g);
  analogWrite(pinRGBB, b);
  
}

void cmdRGB(String rgbValueStr)
{
  String tmp;
  byte value, idxr, idxg, r, g, b;

  // get red value
  idxr = rgbValueStr.indexOf(',');
  tmp = rgbValueStr.substring(0,idxr);
  Trace::trace("r:" + tmp );
  value = (byte)tmp.toInt();
  if (value > 255) value = 255;
  r = value;

  //get green value
  idxg = rgbValueStr.indexOf(',', idxr+1);
  tmp = rgbValueStr.substring(idxr+1, idxg);
  Trace::trace(" g :" + tmp );
  value = (byte)tmp.toInt();
  if (value > 255) value = 255;
  g = value;

  //get blue value
  tmp = rgbValueStr.substring(idxg+1);
  Trace::trace(" b :" + tmp + "\n");
  value = (byte)tmp.toInt();
  if (value > 255) value = 255;
  b = value;
  
  setRGBvalue(r, g, b);
}

void cmdPlayRGB(String aniDuration)
{
  animateRGB = true;
  aniR = 255;
  aniG = 255, aniB = 255; // animated r,g,b led value
  setRGBvalue(aniR, aniG, aniB);
  Trace::traceInfo("play rgb animation\n");


}

void onAnimateRGB(void)
{
  /* if animation is stopped */
  if (animateRGB == false)
    return;
    
  if (aniR > 0)
    aniR--;
  else if(aniG > 0)
    aniG--;
  else if(aniB > 0)
    aniB--;
  else
  {
    animateRGB = false;
    aniR = 255;
    aniG = 255;
    aniB = 255;
  }

  /* set diplay color */
  setRGBvalue(aniR, aniG, aniB);
  delay(3);
  
}
#endif //#ifdef __SUPPORT_RGBLED__

/**
 *================================================================
 * Section IR remote functions
 * https://github.com/marduino/arduino/wiki
 * 
 * https://github.com/arduino/ArduinoCore-samd/blob/master/cores/arduino/main.cpp
 */

/* a start high pulse of 2400 micro-seconds indicates start transferring a set of bits */
#define IR_REMOTE_START_TIMING    2100 

/* a 'one' timing means to set a bit to 1, indicated by 1200micro-seconds high pulse */
#define IR_REMOTE_ONE_TIMING      900

/* a 'zero' timing means to set a bit to 0, indicated by 600micro-seconds high pulse */
#define IR_REMOTE_ZERO_TIMING     400

unsigned long startTiming;
bool isDataValid;
int irData;
byte irDataIdx;

const int pinIRRemote = 2;


void IRRemoteSetup()
{
  startTiming = 0;
  irDataIdx = 0;
  irData = 0;
  isDataValid = false;
  pinMode(pinIRRemote, INPUT);
}

/*
 * Continua
 */
void IRRemoteMeasure()
{
  int value;
  unsigned long dur;

  value = digitalRead(pinIRRemote);
  if ((startTiming == 0) && (HIGH == value))
  {
    startTiming = micros();
    return;
  }

  /* wait until pin value turn to low, always return when HIGH */
  if (HIGH == value)
  {
    return;
  }

  dur = micros() - startTiming;
  startTiming = 0;
  
  /* read data if is a valid data frame */
  if (isDataValid == true) {
    /* convert data according with timing */
    if (dur >= IR_REMOTE_ONE_TIMING)
    {
       bitSet(irData, irDataIdx);
    }
    else if(dur >= IR_REMOTE_ZERO_TIMING)
    {
      bitClear(irData, irDataIdx);
    }
  }
  /* check duration if it is a new start frame */
  else if (dur >= IR_REMOTE_START_TIMING)
  {
      isDataValid = true;
      irDataIdx = 0;
      irData = 0;
  }

  Serial.println(dur, OCT);

}


