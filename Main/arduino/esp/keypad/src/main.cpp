#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

int endereco = 23;

LiquidCrystal_I2C lcd(endereco);  // Set the LCD I2C address

void setup() {
/*for (int i = 5; i >= 8; i++){
  pinMode(i,OUTPUT);
}
for (int i = 9; i >= 13; i++){
  pinMode(i,INPUT);
}*/

  lcd.begin(16,2);               // initialize the lcd 

const byte rows = 4; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'1','2','3','a'},
  {'4','5','6','b'},
  {'7','8','9','c'},
  {'#','0','*','d'}
};
byte rowPins[rows] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[cols] = {9, 10, 11, 12}; //connect to the column pinouts of the keypad
Keypad tecla = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

}

void loop() {
  // put your main code here, to run repeatedly:
lcd.clear();
lcd.setCursor(0,0);
lcd.print(tecla.getKey());
}