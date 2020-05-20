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
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance

String message = "";
bool messageReady = false;

void setup() {
  Serial.begin(9600);
  emon1.current(A0, 75);             // Current: input pin, calibration.
  Serial.write("Waiting for Request");
}

void loop() {
  Serial.print("\n");
  delay(500);
  double Irms = emon1.calcIrms(1500);  // Calculate Irms only
  // Monitor serial communication
//  Serial.println("Waiting for String");
    while(!Serial.available()) {
  Serial.write(".");
  delay(1000);
  }
  while(Serial.available()) {
    delay(50);
    message = Serial.readString();
    messageReady = true;
  }
  // Only process message if there's one
  if(messageReady) {
    // The only messages we'll parse will be formatted in JSON
//    DynamicJsonDocument doc(1024); // ArduinoJson version 6+
    // Attempt to deserialize the message
    
//    Serial.print(Irms*230);
//    Serial.write("P = ");
    Serial.print(Irms*230);
//    Serial.write(" W");
//    Serial.write("\n");
    messageReady = false;
  }
}