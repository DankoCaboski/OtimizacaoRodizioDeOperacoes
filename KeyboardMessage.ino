/*
  Keyboard Message test

  For the Arduino Leonardo and Micro.

  Sends a text string when a button is pressed.

  The circuit:
  - pushbutton attached from pin 4 to +5V
  - 10 kilohm resistor attached from pin 4 to ground

  created 24 Oct 2011
  modified 27 Mar 2012
  by Tom Igoe
  modified 11 Nov 2013
  by Scott Fitzgerald

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/KeyboardMessage
*/

#include "Keyboard.h"
  bool BT = 5;

void setup() {
  Keyboard.begin();
  Serial.begin(9600);
  pinMode (5,INPUT_PULLUP);
}

void loop() {
  BT = digitalRead (5);
  if (BT == false){
 Keyboard.press(KEY_LEFT_GUI);
 Keyboard.press('r');
 Keyboard.releaseAll();
 delay (10);
 Keyboard.println("excel");
 BT = true
 ;
 
 }
 delay(360);
   if (BT == true){
 Serial.write("+");
}
else{
  Serial.write("-");
  
}
}
