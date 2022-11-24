#include "Keyboard.h"
  bool BT = 5;

void setup() {
  Keyboard.begin();
  Serial.begin(9600);
  pinMode (5,INPUT_PULLUP);//botao abrir o excel
}

void loop() {
  BT = digitalRead (5);
  if (BT == false){
 Keyboard.press(KEY_LEFT_GUI);
 Keyboard.press('r');
 Keyboard.releaseAll();
 delay (10);
 Keyboard.println("C:\Users\aluno\Documents\GestaoDeRodizio");//localiza
 Keyboard.println();//abre
 
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
