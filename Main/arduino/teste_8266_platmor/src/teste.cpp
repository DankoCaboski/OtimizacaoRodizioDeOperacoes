#include <Arduino.h>
void setup(){
    pinMode(2,OUTPUT);
}
void loop(){
    digitalWrite(2,1);
    delay(400);
    digitalWrite(2,0);
    delay(400);
}