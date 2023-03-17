/*
# Setup Arduino IDE:
- use this as board manager: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
- install "Raspberry Pi Pico/RP2040 by Earle F..."

# This is a modification of 'Wifi example'
*/



#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <LEAmDNS.h>
#include "myirconfigs.h"

#ifndef STASSID
#include "mywificredentials.h"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const int led = LED_BUILTIN;
WebServer server(80);



void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "available:\r\n\t- power\r\n\t- swing\r\n\t- minus\r\n\t- clock\r\n\t- plus\r\n\t- mode\r\n\t- dots\r\n");
  digitalWrite(led, 0);
}

void handleCmdPower() {
  digitalWrite(led, 1);
  IRRawDataType data = 0xFC03FC03FE01; // {0xFC03FE01, 0xFC03}
  IrSender.sendPulseDistanceWidthFromArray(38, 8900, 4350, 600, 1600, 600, 500, &data, 48, PROTOCOL_IS_LSB_FIRST, 0, NO_REPEATS);
  server.send(200, "text/plain", "power\r\n");
  digitalWrite(led, 0);
}

void handleCmdSwing() {
  digitalWrite(led, 1);
  IRRawDataType data = 0xEC13EC13FE01; // {0xEC13FE01, 0xEC13}
  IrSender.sendPulseDistanceWidthFromArray(38, 8850, 4400, 600, 1600, 600, 500, &data, 48, PROTOCOL_IS_LSB_FIRST, 0, NO_REPEATS);
  server.send(200, "text/plain", "swing\r\n");
  digitalWrite(led, 0);
}

void handleCmdMinus() {
  digitalWrite(led, 1);
  IRRawDataType data = 0xEE11EE11FE01; // {0xEE11FE01, 0xEE11}
  IrSender.sendPulseDistanceWidthFromArray(38, 8850, 4400, 550, 1650, 550, 550, &data, 48, PROTOCOL_IS_LSB_FIRST, 0, NO_REPEATS);
  server.send(200, "text/plain", "minus\r\n");
  digitalWrite(led, 0);
}

void handleCmdClock() {
  digitalWrite(led, 1);
  IRRawDataType data = 0xF40BF40BFE01; // {0xF40BFE01, 0xF40B}
  IrSender.sendPulseDistanceWidthFromArray(38, 8850, 4400, 600, 1600, 600, 550, &data, 48, PROTOCOL_IS_LSB_FIRST, 0, NO_REPEATS);
  server.send(200, "text/plain", "clock\r\n");
  digitalWrite(led, 0);
}

void handleCmdPlus() {
  digitalWrite(led, 1);
  IRRawDataType data = 0xF20DF20DFE01; // {0xF20DFE01, 0xF20D}
  IrSender.sendPulseDistanceWidthFromArray(38, 8900, 4400, 600, 1600, 600, 500, &data, 48, PROTOCOL_IS_LSB_FIRST, 0, NO_REPEATS);
  server.send(200, "text/plain", "plus\r\n");
  digitalWrite(led, 0);
}

void handleCmdMode() {
  digitalWrite(led, 1);
  IRRawDataType data = 0xF00FF00FFE01; // {0xF00FFE01, 0xF00F}
  IrSender.sendPulseDistanceWidthFromArray(38, 8900, 4400, 600, 1600, 600, 500, &data, 48, PROTOCOL_IS_LSB_FIRST, 0, NO_REPEATS);
  server.send(200, "text/plain", "mode\r\n");
  digitalWrite(led, 0);
}

void handleCmdDots() {
  digitalWrite(led, 1);
  IRRawDataType data = 0xD22DD22DFE01; // {0xD22DFE01, 0xD22D}
  IrSender.sendPulseDistanceWidthFromArray(38, 8850, 4400, 550, 1650, 550, 550, &data, 48, PROTOCOL_IS_LSB_FIRST, 0, NO_REPEATS);
  server.send(200, "text/plain", "dots\r\n");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("picow")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/power", handleCmdPower);
  server.on("/swing", handleCmdSwing);
  server.on("/minus", handleCmdMinus);
  server.on("/clock", handleCmdClock);
  server.on("/plus", handleCmdPlus);
  server.on("/mode", handleCmdMode);
  server.on("/dots", handleCmdDots);

  server.onNotFound(handleNotFound);

  /////////////////////////////////////////////////////////
  // Hook examples

  server.addHook([](const String & method, const String & url, WiFiClient * client, WebServer::ContentTypeFunction contentType) {
    (void)method;       // GET, PUT, ...
    (void)url;          // example: /root/myfile.html
    (void)client;       // the webserver tcp client connection
    (void)contentType;  // contentType(".html") => "text/html"
    Serial.printf("A useless web hook has passed\n");
    return WebServer::CLIENT_REQUEST_CAN_CONTINUE;
  });

  // Hook examples
  /////////////////////////////////////////////////////////

  server.begin();
  Serial.println("HTTP server started");

  IrSender.begin(MY_IR_SEND_PIN, DISABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
  Serial.print(F("Ready to send IR signals at pin"));
  Serial.println(MY_IR_SEND_PIN);
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
