/*********
  ESP32 Web server
  Includes an lcd H44780 conected via I2C bus. The IP addres obtained via 
  DHCP protocol is printed in the display. 
  The dispay even shows the status of each of two relays conected
  and handled by the HTML code.
  This sketch it is based on the Rui Santos original Web server example 
  and was modified by me in Ramos Mejía, ARGENTINA, August 2020.
  - Osvaldo Cantone  correo@cantone.com.ar
  reles:
    Relé1 GPIO.15
    Relé2 GPIO.2
  display:
    SDA GPIO.21
    SCL GPIO.22
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>

// Replace the X's with your network credentials
String ssid = "Caboski";
String password = "Cafe&Live22";

// Set web server port number to 80
WiFiServer server(80);

void setup() {

  Serial.begin(9600);

  //Initializing the display
Wire.begin();
  Wire.beginTransmission(0x27);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println("Caboski");
  WiFi.begin("Caboski", "Cafe&Live22");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

delay(2000);

  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {  // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");



            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            client.println("<p>Adapted by Osvaldo Cantone <a href=http://www.cantone.com.ar/wordpress/esp32-web-server-relay-control-lcd-i2c-display/>tecteach.net</a> </p>");
          }
        }
      }
    }
  }
}