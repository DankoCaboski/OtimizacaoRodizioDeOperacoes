#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Clock.h>

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define SS_PIN 53
#define RST_PIN 2

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String conteudo = "";

Clock meuRelogio;

horario prog1;
horario prog2;

byte ultimoPrograma = 0;

// Init array that will store new NUID
byte nuidPICC[4];
int rele[] = {43, 45, 47};

String totem = "Macaneta";

int n = 0;

String nomes[4][8] = {{"Robinson", "8C 2C 56 04", "Over", "Tirante Direito", "Tirante Esquerdo", "Macaneta", "Capota", "Emblemas"},
  {"Goncalino", "BA 5D 13 0A", "N/A", "N/A", "N/A", "N/A", "Capota", "Emblemas"},
  {"Andre L", "N/A", "N/A", "N/A", "Macaneta", "Capota", "Emblemas"},
  {"Jaleel", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A"}
};

void setup() {
  for (int i = 0; i < 1; i++) {
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

  prog1.hora = 05;
  prog1.minuto = 46;

  meuRelogio.ajustaHorario (05, 45, 0);

  if (meuRelogio.horaMinutoIgual(prog1, meuRelogio.horarioAtual())) {
    if (ultimoPrograma != 1) {
      sinaleiroAmarelo();
      Serial.println("*** Rodízio 1 ***");
      ultimoPrograma = 1;
    }
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

  meuRelogio.quandoMillisZerar();
  Serial.println(meuRelogio.horaParaTexto(meuRelogio.horarioAtual()));

  ver_cartao();

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
  }
  else Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  lcd.clear();
  delay(300);
}


void ver_cartao() {
  while (!rfid.PICC_IsNewCardPresent()) {
    // Wait for a card to be present
  }

  // Verify if the NUID has been read
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Print the card type
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.print(F("PICC type: "));
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check if the card is of type MIFARE Classic
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
}

void call_cartao() {
  Serial.println("");
  Serial.print("Aproxime o cartao no leitor");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aproxime seu");
  lcd.setCursor(0, 1);
  lcd.print("cartao no leitor");
}

void printHex(byte * buffer, byte bufferSize) {
  int teste = -1 ;
  conteudo = "";
  for (byte i = 0; i < bufferSize; i++) {
    conteudo.concat(buffer[i] < 0x10 ? " 0" : " ");
    conteudo.concat(String(buffer[i], HEX));
  }
  conteudo.toUpperCase();
  Serial.println(conteudo);
  lcd.clear();
  lcd.setCursor(0, 0);


  for (int i = 0; i < 4; i++) {
    if (nomes[i][1] == conteudo) {
      teste = i;
      break; // interrompe o loop quando encontra o valor de "conteudo"
    }
  }

  if (teste != -1) {
    Serial.print(nomes[teste][0]); // imprime o valor correspondente na primeira posição do subarray
  } else {
    Serial.println("Valor não encontrado."); // imprime uma mensagem caso o valor não seja encontrado
  }

  for (int i = 1; i < 6; i++) {
    if (nomes[n][i] = totem) {
      lcd.setCursor(0, 1);
      lcd.print("habilitado");
      break;
    }
    else {
      lcd.print(" nao habilitado ");
    }
  }

  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }

}

void sinaleiroVerde() {
  digitalWrite (rele[1], LOW);
  digitalWrite (rele[2], HIGH);

}
void sinaleiroAmarelo() {
  digitalWrite (rele[1], HIGH);
  delay (20000);
  sinaleiroVerde();
}
void sinaleiroVermelho() {

}
