/*
   --------------------------------------------------------------------------------------------------------------------
   Example sketch/program showing how to read new NUID from a PICC to serial.
   --------------------------------------------------------------------------------------------------------------------
   This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid

   Example sketch/program showing how to the read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
   Reader on the Arduino SPI interface.

   When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
   then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
   you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
   will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages
   when removing the PICC from reading distance too early.

   @license Released into the public domain.

   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS)      10            53        D10        10               10
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

   More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define SS_PIN 53
#define RST_PIN 5

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;
String conteudo = "";


// Init array that will store new NUID
byte nuidPICC[4];
int rele[] = {43, 45, 47};

String totem = "Macaneta";

//String nomes[] = {"Robinson", "Goncalino", "Andre L", "Jaleel"};
int n = 0;
String nomes[4][7] = {{"8C 2C 56 04", "Over", "Tirante Direito", "Tirante Esquerdo", "Macaneta", "Capota", "Emblemas"},
                         {"N/A", "N/A", "N/A", "N/A", "N/A", "Capota", "Emblemas"},
                         {"N/A", "N/A", "N/A", "Macaneta", "Capota", "Emblemas"},
                         {"N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A"}};
  
void setup() {
  for (int i = 0; i > 1; i++) {
    pinMode(rele[i], OUTPUT);
  }
  for (int i = 0; i > 1; i++) {
    digitalWrite(rele[i], LOW);
  }

  lcd.begin (16, 2);
  Serial.begin (9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print("Sistema iniciado");
  lcd.print("  Bem vindo!   ");
  Serial.println("Sistema iniciado");
  delay(500);
  call_cartao();
}

void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] ||
      rfid.uid.uidByte[1] != nuidPICC[1] ||
      rfid.uid.uidByte[2] != nuidPICC[2] ||
      rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    Serial.println(F("NUID tag:"));
    Serial.print(F("Em hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("Em dec: "));
//    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
  else Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  lcd.clear();
  delay(300);
}


/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void call_cartao() {
  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print("Aproxime seu");
  lcd.setCursor (0, 1);
  lcd.print("cartao");
}

void printHex(byte *buffer, byte bufferSize) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(nomes[n][n]);
 /* for (int i = 0; i>3; i++){
      if (conteudo = nomes[n]) {
        }*/
  
  if (nomes[n][n] = "Robinson") {
    for (int i = 1; i < 5; i++) {
      if (nomes[i][i] = totem) {
        lcd.setCursor(0, 1);
        lcd.print("habilitado");
        break;
      }
      else {
        return;
      }
    }
  }
  

  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  while (n == 0)
  {}
}

/**
   Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}
