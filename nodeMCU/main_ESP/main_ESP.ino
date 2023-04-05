#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(D6, D7); // RX, TX
String mensagem = "";

const char* ssid = "thiago 2.4G";
const char* password = "mailow087";

const char* host = "192.168.1.178";

String recebida = "";

String hexadecimal;
String nome;
String treinamentos;
String registro;
String primeira;
String ultima;

void setup() {
  Serial.begin(9600);
  delay(10);

  Serial.begin(9600); // Inicia a comunicação serial com o monitor serial
  mySerial.begin(9600); // Inicia a comunicação serial com o Arduino MEGA

  Serial.println();
  Serial.println();
  Serial.print("Conectando com ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.print("Conectando com ");
  Serial.print(host);

  WiFiClient client;

  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println(" Falha na conexao");
    delay(5000);
    return;
  }

  if (mySerial.available() > 0) { // Verifica se há dados disponíveis na serial
    if (mySerial.readStringUntil('\n') == "ok!") {
      mensagem = mySerial.readStringUntil('\n');
      mensagem.trim(); //Remove espaços em branco do início e do fim da string

      //Divisão da  em substrings usando o caractere ";" como delimitador
      int index1 = mensagem.indexOf(';');
      if (index1 != -1) {
        hexadecimal = mensagem.substring(0, index1);

        int index2 = mensagem.indexOf(';', index1 + 1);
        if (index2 != -1) {
          nome = mensagem.substring(index1 + 1, index2);

          int index3 = mensagem.indexOf(';', index2 + 1);
          if (index3 != -1) {
            treinamentos = mensagem.substring(index2 + 1, index3);

            int index4 = mensagem.indexOf(';', index3 + 1);
            if (index4 != -1) {
              registro = mensagem.substring(index3 + 1, index4);

              int index5 = mensagem.indexOf(';', index4 + 1);
              if (index5 != -1) {
                primeira = mensagem.substring(index4 + 1 , index5);
                ultima = mensagem.substring(index5 + 1);
              }
            }
          }
        }
      }
    }

    //Impressão das substrings
    Serial.println(hexadecimal);
    Serial.println(nome);
    Serial.println(treinamentos);
    Serial.println(registro);
    Serial.println(primeira);
    Serial.println(ultima);
  }

  /*
    // Função para gerar um valor hexadecimal aleatório de 7 dígitos
    hexadecimal = String(random(0xFFFFFFF), HEX);

    // Função para gerar um nome aleatório
    String nomes[] = {"Alice", "Bob", "Charlie", "Dave"};
    nome = nomes[random(0, 4)];

    // Função para gerar uma lista de treinamentos aleatórios
    String treinamento[] = {"Treinamento1", "Treinamento2", "Treinamento3", "Treinamento4"};
    treinamentos = treinamento[random(0, 4)];

    // Função para gerar um registro aleatório de 5 dígitos
    registro = String(random(10000, 99999));

    // Função para gerar um número aleatório de 4 dígitos
    primeira = String(random(1000, 9999));

    // Função para gerar um número aleatório de 4 dígitos
    ultima = String(random(1000, 9999));

    /////////////  SALVA NO SERVIDOR   ////////////////
    String url = "/nodeMCU/salvar.php?";
    url += "hexadecimal=";
    url += hexadecimal;
    url += "&nome=";
    url += nome;
    url += "&treinamentos=";
    url += treinamentos;
    url += "&registro=";
    url += registro;
    url += "&primeira=";
    url += primeira;
    url += "&ultima=";
    url += ultima;

    Serial.print (" Requisitando URL: ");
    Serial.println (url);

    client.print (String("GET ") + url + " HTTP/1.1\r\n" +
                "HOST: " + host + "\r\n" +
                "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
    }

    while (client.available()) {
    String line = client.readStringUntil ('\r');
    //Serial.print(line);

    if (line.indexOf("salvo_com_sucesso") != -1) {
      Serial.println();
      Serial.println("Uhuu, foi salvo com sucesso");
    }
    else if (line.indexOf("erro_ao_salvar") != -1) {
      Serial.println();
      Serial.println ("Ops, ocorreu um erro");
    }
    }

    Serial.println();
    Serial.println("Conexao fechada");

    delay(20000);
  */
}
