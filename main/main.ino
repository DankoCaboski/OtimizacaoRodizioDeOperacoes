#include <Wire.h> //Biblioteca para comunicação com dispositivos I2C
#include <SPI.h> //Biblioteca para comunicação com dispositivos SPI
#include <MFRC522.h> //Biblioteca para leitura e controle de tags RFID
#include <LiquidCrystal_I2C.h> //Biblioteca para controle de displays LCD I2C
#include <Keypad.h> //Biblioteca para leitura de teclado numérico
#include <SoftwareSerial.h>
#include "RTClib.h" //Biblioteca para controle de um relógio em tempo real DS3231

//Variáveis constantes da pinagem para comunicação com o módulo RFID
const int SS_PIN = 53;
const int RST_PIN = 2;

String msg = "";//string a ser enviada para o esp

//Objeto do tipo MFRC522
MFRC522 rfid(SS_PIN, RST_PIN);

//Objeto do tipo LiquidCrystal_I2C com as configurações necessárias
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Objeto do tipo RTC_DS3231
RTC_DS3231 rtc;

//Define as constantes da quantidade de colunas e linhas do teclado
const byte ROWS = 4;
const byte COLS = 4;

// Define a matriz de pinos usada pelo teclado
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Define os pinos conectados às linhas e colunas do teclado
byte rowPins[ROWS] = {10, 9, 8, 7};
byte colPins[COLS] = {6, 5, 4, 3};

// Cria um objeto Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

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
const String registro[] = {"00000", "67890", "54321", "09876"};

//Nome do totem
const String totem = "OverHead";

//Array do MM
String MM = "";

//Define a pinagem dos relés
int relePin1 = 43;
int relePin2 = 45;
int relePin3 = 47;

//bool cartaoReconhecido = false; //Variável para verificar se o cartão foi reconhecido
//bool cartaoFoiReconhecido = false;

unsigned long lastExecTime = 0; //Variável para armazenar o tempo da última execução do ciclo
const unsigned long cycleInterval = 60000; //Intervalo de tempo para repetir o ciclo (1 hora em milissegundos)

unsigned long lastMMTime = 0; //Variável para armazenar o tempo da última execução do ciclo
const unsigned long MMInterval = 60000; //Intervalo de tempo para repetir o ciclo zerando o MM anterior (1 hora em milissegundos)

void setup() {
  pinMode(relePin1, OUTPUT);
  pinMode(relePin2, OUTPUT);
  pinMode(relePin3, OUTPUT);

  Wire.begin();
  Serial.begin(9600); //Inicia a comunicação serial
  lcd.begin (16, 2); //Inicia LCD 16x2
  SPI.begin(); //Inicia a comunicação SPI
  rfid.PCD_Init(); //Inicia o leitor RFID
  rtc.begin(); //Inicia o módulo RTC
  //rtc.adjust(DateTime(2023, 4, 5, 8, 13, 00)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)

  lcd.setCursor (0, 0);
  lcd.print("Sistema iniciado");
  Serial.println("Sistema iniciado");
  Serial.println("");
  delay(1000);

  inputCar(); //Chama o void para input do número do carro
}

void inputCar() {
  digitalWrite(relePin1, LOW);
  digitalWrite(relePin2, HIGH);
  digitalWrite(relePin3, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Informe o MM: ");
  while (MM.length() < 4) { //Executa este loop enquanto o tamanho da entrada for menor que 4
    char key = keypad.getKey();
    if (key != NO_KEY) {
      if (isdigit(key)) { //Verifica se o caractere é um dígito
        MM += key; //Adiciona o dígito à entrada
        lcd.setCursor(0, 1);
        lcd.print("MM = ");
        lcd.print(MM);
      }
      else if (key == '*') { //Verifica se o usuário pressionou o asterisco para cancelar a entrada
        MM = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cancelado");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Informe o MM:");
      }
    }
  }
  Serial.println("MM: " + MM);

  callCardKeypad(); //Chama o void da mensagem loop para chamar cartão e keypad
}

void callCardKeypad() {
  bool i = true;
  long ii = 1000;
  long momento = 0;
  long ultimoTempo = 0; //variável para armazenar o último tempo de atualização da tela
  const long intervalo = 1000; //intervalo de tempo em milissegundos

  while (true) {
    momento = millis();

    if (momento - ultimoTempo >= intervalo) {
      ultimoTempo = momento;

      if (i) {
        lcd.clear();
        lcd.print("Aproxime o");
        lcd.setCursor(0, 1);
        lcd.print("cartao RFID...");
      } else {
        lcd.clear();
        lcd.print("Digite o");
        lcd.setCursor(0, 1);
        lcd.print("registro...");
      }

      i = !i;
    }

    char key = keypad.getKey();

    if (rfid.PICC_IsNewCardPresent() || key != NO_KEY) {
      break;
    }
  }
}

void loop() {
  DateTime now = rtc.now();

  if (millis() - lastExecTime >= cycleInterval) { //Verifica se já passou uma hora desde a última execução do ciclo
    lastExecTime = millis(); //Reinicia o tempo da última execução para o momento atual

    if (millis() - lastMMTime >= MMInterval) { //Reinicia a variável MM a cada hora
      MM = "";
      lastMMTime = millis();
    }
    inputCar(); //Chama o void para input do número do carro
  }

  //****** FIM DA LÓGICA DO RTC ********

  //if ((now.hour() >= 5 && now.minute() >= 50) && (now.hour() <= 22 && now.minute() <= 50)) { //Verifica se está dentro do intervalo de tempo


  /* // aguarda 5 minutos ou até que o cartão seja reconhecido
    unsigned long timeout = millis() + 300000; // tempo limite de 5 minutos em milissegundos
    while (millis() < timeout && !cartaoReconhecido) {*/

  //****** FIM DA LÓGICA DO RTC ********


  //*****INÍCIO DA LÓGICA DO RFID******

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) { //Verifica se há um cartão RFID presente
    String hexValue = ""; //Lê o valor hexadecimal do RFID
    for (byte i = 0; i < rfid.uid.size; i++) {
      hexValue += String(rfid.uid.uidByte[i], HEX);
    }
    delay(500);
    int index = -1; //Procura o índice correspondente ao valor hexadecimal no array
    for (int i = 0; i < 4; i++) {
      if (hexValues[i] == hexValue) {
        index = i; addmsg(hexValues[i]);
        break;
      }
    }
    if (index >= 0) { //Exibe as informações de treinamentos no monitor serial e no LCD
      bool foundTotem = false;
      for (int i = 0; i < 6; i++) {
        if (trainings[index][i] != "") {
          if (trainings[index][i] == totem) {
            foundTotem = true;
            break;
          }
        }
      }
      if (foundTotem) { //Exibe as informações de treinamentos no monitor serial e no LCD
        String nome = names[index];
        Serial.print("Nome: ");
        Serial.println(nome);
        addmsg(nome);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("");
        lcd.print(nome);

        String treinamentos = "";
        for (int i = 0; i < 6; i++) {
          if (trainings[index][i] != "") {
            Serial.print(" - ");
            Serial.println(trainings[index][i]);
            treinamentos += trainings[index][i];
            if (i != 6) {
              treinamentos += ',';
            }
          }
        }
        addmsg(treinamentos);

        String reg = registro[index];
        Serial.print("Registro: ");
        Serial.println(reg);
        addmsg(reg);

        lcd.setCursor(0, 1);
        lcd.print("");
        lcd.print(reg);

        addmsg(MM);

        Serial.println("FUNCIONARIO HABILITADO");
        Serial.println("");

        digitalWrite(relePin1, LOW);
        digitalWrite(relePin2, LOW);
        digitalWrite(relePin3, HIGH);
      }
      else {
        Serial.println("Funcionário não habilitado");
        Serial.print("Precisa-se treinar este funcionário para a operação: ");
        Serial.println(totem);
        Serial.println ("");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Funcionario nao");
        lcd.setCursor(0, 1);
        lcd.print("habilitado");

        digitalWrite(relePin1, HIGH);
        digitalWrite(relePin2, LOW);
        digitalWrite(relePin3, LOW);
      }
      //Aguarda a remoção do cartão RFID
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
    else {
      Serial.println("Cartão não cadastrado");
      Serial.println ("");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cartao nao");
      lcd.setCursor(0, 1);
      lcd.print("cadastrado");

      digitalWrite(relePin1, HIGH);
      digitalWrite(relePin2, LOW);
      digitalWrite(relePin3, LOW);
    }
    delay (100);
  }

  //*****FIM DA LÓGICA DO RFID*****


  //****INÍCIO DA LÓGICA DO TECLADO****

  String meuRegistro = ""; //Declara a variável fora do bloco condicional
  static byte count = 0; //Contador de dígitos
  static char codigo[6] = {'\0'}; //Vetor para armazenar os dígitos do código e finalizar com o caractere nulo
  String numRegistro = "";

  if (keypad.getKey() != NO_KEY) { //Se alguma tecla for pressionada
    int i = 10;
    bool teclado  = HIGH;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Registro: ");

    if (teclado) {
      while (teclado) {
        lcd.setCursor(0, i);
        char key = keypad.getKey();
        if (key != NO_KEY) {
          codigo[count] = key; //Armazena o dígito na posição correspondente
          numRegistro.concat(key);
          lcd.setCursor(i, 0);
          lcd.print(key);
          count++; //Incrementa o contador
          i++;
        }
        if (numRegistro.length() >= 5) {
          break;
        }
      }
      teclado = !teclado;
    }

    if (count == 5) { //Se o código estiver completo
      meuRegistro = String(codigo); //Converte o vetor em uma String
      Serial.println("Registro lido: " + meuRegistro); //Exibe o código na serial
      count = 0; //Reinicia o contador de dígitos
    }
    delay(500);
    int index = -1; //Procura o índice correspondente ao valor hexadecimal no array
    for (int i = 0; i < 4; i++) {
      if (registro[i] == meuRegistro) {
        index = i;
        break;
      }
    }
    if (index >= 0) { //Exibe as informações de treinamentos no monitor serial e no LCD
      bool foundTotem = false;
      for (int i = 0; i < 6; i++) {
        if (trainings[index][i] != "") {
          if (trainings[index][i] == totem) {
            foundTotem = true;
          }
        }
      }
      if (foundTotem) { //Exibe as informações de treinamentos no monitor serial e no LCD
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
        Serial.println("FUNCIONARIO HABILITADO");
        Serial.println("");

        digitalWrite(relePin1, LOW);
        digitalWrite(relePin2, LOW);
        digitalWrite(relePin3, HIGH);
      }
      else {
        Serial.println("Funcionário não habilitado");
        Serial.print("Precisa-se treinar este funcionário para a operação: ");
        Serial.println(totem);
        Serial.println ("");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Funcionario nao");
        lcd.setCursor(0, 1);
        lcd.print("habilitado");

        digitalWrite(relePin1, HIGH);
        digitalWrite(relePin2, LOW);
        digitalWrite(relePin3, LOW);
      }
    }
    else {
      Serial.println("Registro não cadastrado");
      Serial.println ("");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Registro nao");
      lcd.setCursor(0, 1);
      lcd.print("cadastrado");

      digitalWrite(relePin1, HIGH);
      digitalWrite(relePin2, LOW);
      digitalWrite(relePin3, LOW);
    }
    delay (100);
  }

  //****FIM DA LÓGICA DO TECLADO****

  /*if (cartaoFoiReconhecido) {
    cartaoReconhecido = true;
    digitalWrite(relePin1, HIGH);
    digitalWrite(relePin2, HIGH);
    digitalWrite(relePin3, HIGH);
    }*/
  //    if()
  Serial.println("ok!");
  Serial.println(msg);
}
//}
/*else {
  // Desativa todos os relés fora do intervalo de tempo
  digitalWrite(relePin1, LOW);
  digitalWrite(relePin2, LOW);
  digitalWrite(relePin3, LOW);
  //
  //*/
void addmsg(String paraADD) {
  msg += paraADD;
  msg += ";";
}
