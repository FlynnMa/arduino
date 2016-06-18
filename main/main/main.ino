#include "trace.h"

/*
 * Defintions
*/
const int pinLEDYellow = 13;
const int pinLEDRed = 8;
const int pinButton = 9;

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
    Serial.write("hello world!\r\n");
  }

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

void SerialReadInput()
{
  int len = Serial.available();
  char value = 0;
  
  while(len-- > 0) {
      value = (char)Serial.read();
      Serial.print(value);
  }
}

/*
 * main loop function
 */
void loop() {
  // put your main code here, to run repeatedly:
  int buttonValue;

  buttonValue = digitalRead(pinButton);
  if (buttonValue == LOW) {
    flashLED();
  }
}
