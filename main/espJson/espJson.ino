/*------------------------------------------------------------------------------
  06/25/2019
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: esp8266_firmware.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to communicate between an Arduino UNO
  and an ESP8266.
  https://youtu.be/6-RXqFS_UtU
  
  Do you like my videos? You can support the channel:
  https://patreon.com/acrobotic
  https://paypal.me/acrobotic
  ------------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 
  https://acrobotic.com/
  https://amazon.com/acrobotic
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define D7 13
#define D8 15

SoftwareSerial mySerial(D7, D8); // RX = D7, TX  = D8

ESP8266WebServer server;
char* ssid = "Rareriroru2G";
char* password = "INTEGRAL";

// char* ssid = "VIVOFIBRA-0CA4";
// char* password = "EAEA8D0CA4";

void setup()
{
  WiFi.begin(ssid,password);
  Serial.begin(9600);  
  mySerial.begin(9600); //Start mySerial
  pinMode(D7,INPUT); //d7 is RX, receiver, so define it as input
  pinMode(D8,OUTPUT); //d8 is TX, transmitter, so define it as output
  while(WiFi.status()!=WL_CONNECTED)
  {
    mySerial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // server.on("/",handleIndex);
  // server.begin();
}

void loop()
{
  // Send a JSON-formatted request with key "type" and value "request"
  // then parse the JSON-formatted response with keys "gas" and "Irms"
  DynamicJsonDocument doc(1024);
  double Irms = 0;
  // Sending the request
  doc["type"] = "request";
  serializeJson(doc,mySerial);
  // Reading the response
  boolean messageReady = false;
  String message = "";
  while(messageReady == false) { // blocking but that's ok
    if(mySerial.available()) {
      message = mySerial.readString();
      messageReady = true;
    }
  }
  // Attempt to deserialize the JSON-formatted message
  DeserializationError error = deserializeJson(doc,message);
  if(error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  Irms = doc["Irms"];
  // gas = doc["gas"];
  // Prepare the data for serving it over HTTP
  String output = "Irms: " + String(Irms) + "\n";
  // output += "CO level: " + String(gas);
  // Serve the data as plain text, for example
  Serial.print(output);
}
