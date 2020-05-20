/*------------------------------------------------------------------------------
  06/25/2019
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: uno_firmware.ino
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
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance
SoftwareSerial mySerial(2,3); // RX = D2, TX  = D3

String message = "";
bool messageReady = false;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600); //Start mySerial
  pinMode(2,INPUT); //d2 is RX, receiver, so define it as input
  pinMode(3,OUTPUT); //d3 is TX, transmitter, so define it as output
}

void loop() {
  // Monitor serial communication
  while(mySerial.available()) {
    message = mySerial.readString();
    messageReady = true;
  }
  // Only process message if there's one
  if(messageReady) {
    // The only messages we'll parse will be formatted in JSON
    DynamicJsonDocument doc(1024); // ArduinoJson version 6+
    // Attempt to deserialize the message
    DeserializationError error = deserializeJson(doc,message);
    if(error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      messageReady = false;
      return;
    }
    if(doc["type"] == "request") {
      doc["type"] = "response";
      // Get data from analog sensors
      double Irms = emon1.calcIrms(30);  // Calculate Irms only
      doc["Irms"] = Irms*1000;  // Calculate Irms only
      // doc["Irms"] = random(300);  // Calculate Irms only
      // doc["gas"] = analogRead(A1);
      serializeJson(doc,mySerial);
    }
    messageReady = false;
  }
}