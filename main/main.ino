#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const int SS_PIN = 53;
const int RST_PIN = 2;

int rele[] = {43, 45, 47};

MFRC522 rfid(SS_PIN, RST_PIN); //Cria um objeto MFRC522

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //Cria um objeto LiquidCrystal_I2C com as configurações necessárias

//Array de valores hexadecimais
const String hexValues[] = {"8c2c564", "ba5d13a", "2A9C7E8B", "B24E758A"};

//Array de nomes correspondentes
const String names[] = {"Robson", "Goncalino", "José", "Ana"};

//Array de treinamentos
const String trainings[][6] = {
  {"OverHead", "TiranteD", "TiranteE", "Macaneta", "Capota", "Emblemas"}, // Robson
  {"Capota", "Emblemas"}, // Goncalino
  {"Treinamento 3", "Treinamento 4", "Treinamento 6"}, // José
  {"Treinamento 1", "Treinamento 4", "Treinamento 5"}  // Ana
};

//Array de registros correspondentes
const String registro[] = {"12345", "67890", "54321", "09876"};

//Nome do totem
const String totem = "OverHead";

void setup() {
  Serial.begin(9600); //Inicia a comunicação serial
  lcd.begin (16, 2);  //Inicia LCD 16x2
  SPI.begin();        //Inicia a comunicação SPI
  rfid.PCD_Init();    //Inicia o leitor RFID

    for (int i = 0; i < 3; i++) {
    pinMode(rele[i], OUTPUT);
    digitalWrite(rele[i], LOW);
  }

  lcd.setCursor (0, 0);
  lcd.print("Sistema iniciado");
  Serial.println("Sistema iniciado");
  Serial.println();
  delay(500);

  call_Card_Keypad();
}

void call_Card_Keypad() {
  //Define a variável i com o valor 1
  bool i = 1;
  //Inicia um loop infinito
  while (true) {
    //Verifica se a variável i é verdadeira
    if (i) {
      //Se sim, limpa o display LCD e exibe a mensagem
      lcd.clear();
      lcd.print("Aproxime o");
      lcd.setCursor(0, 1);
      lcd.print("cartao RFID...");
    }
    else {
      //Se não, limpa o display LCD e exibe a mensagem
      lcd.clear();
      lcd.print("Digite o");
      lcd.setCursor(0, 1);
      lcd.print("registro...");
    }
    delay(1000);
    //Inverte o valor da variável i
    i = !i;
    //Verifica se um novo cartão RFID foi detectado
    if (rfid.PICC_IsNewCardPresent()) {
      //Se sim, sai do loop while
      break;
    }
  }
}

void loop() {
  CartaoLido();
}
void CartaoLido() {
  //Verifica se há um cartão RFID presente
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    //Lê o valor hexadecimal do RFID
    String hexValue = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      hexValue += String(rfid.uid.uidByte[i], HEX);
    }
    delay(500);
    //Procura o índice correspondente ao valor hexadecimal no array
    int index = -1;
    for (int i = 0; i < 4; i++) {
      if (hexValues[i] == hexValue) {
        index = i;
        break;
      }
    }

    //Exibe as informações de treinamentos no monitor serial e no LCD
    if (index >= 0) {
      bool foundTotem = false;
      for (int i = 0; i < 6; i++) {
        if (trainings[index][i] != "") {
          if (trainings[index][i] == totem) {
            foundTotem = true;
          }
        }
      }

      if (foundTotem) {
        //Exibe as informações de treinamentos no monitor serial e no LCD
        String name = names[index];
        Serial.print("Nome: ");
        Serial.println(name);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("");
        lcd.print(name);

        String reg = registro[index];
        Serial.print("Registro: ");
        Serial.println(reg);

        lcd.setCursor(0, 1);
        lcd.print("");
        lcd.print(reg);

        for (int i = 0; i < 6; i++) {
          if (trainings[index][i] != "") {
            Serial.print(" - ");
            Serial.println(trainings[index][i]);
          }
        }
        Serial.println("");
      }
      else {
        //Funcionário não habilitada
        Serial.println("Funcionário não habilitado");
        Serial.print("Precisa-se treinar este funcionário para a operação: ");
        Serial.println(totem);
        Serial.println ("");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Funcionario nao");
        lcd.setCursor(0, 1);
        lcd.print("habilitado");
      }
      //Aguarda a remoção do cartão RFID
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
    else {
      //Cartão não cadastrado
      Serial.println("Cartao nao cadastrado");
      Serial.println ("");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cartao nao");
      lcd.setCursor(0, 1);
      lcd.print("cadastrado");
    }
  }
}
