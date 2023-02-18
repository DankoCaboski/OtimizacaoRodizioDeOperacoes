
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>


#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

LiquidCrystal lcd(6, 7, 5, 4, 3, 2);

char st[20];

int bt = 11;//debug botão leonardo rfid
int BT;//estado botão leonardo rfid

void setup()
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  //Define o número de colunas e linhas do LCD:
  lcd.begin(16, 2);
//  mensageminicial();
  pinMode (bt, INPUT_PULLUP);
}

void loop()
{
  delay(360);
  if (BT == true) {
    Serial.write("+");
  }
  else {
    Serial.write("-");

  }
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  if (conteudo.substring(1) == "ED 78 03 CA") //UID 1 - Chaveiro
  {
    Serial.println("Ola, MT 01 !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ola MT 01!");
    lcd.setCursor(0, 1);
    lcd.print("Acesso liberado!");
    delay(3000);
    mensageminicial();
  }
  if (conteudo.substring(1) == "BD 9B 06 7D") //UID 2 - Cartao
  {
    Serial.println("Ola MT 02 !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ola MT 02");
    lcd.setCursor(0, 1);
    lcd.print("Acesso Negado !");
    delay(3000);
    mensageminicial();
  }
}

void mensageminicial()
{
  lcd.clear();
  lcd.print(" Aproxime o seu");
  lcd.setCursor(0, 1);
  lcd.print("cartao do leitor");
}

void abre_excel()
{

}
