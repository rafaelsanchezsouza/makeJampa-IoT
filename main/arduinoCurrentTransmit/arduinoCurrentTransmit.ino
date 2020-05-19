#include <SoftwareSerial.h>
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance
SoftwareSerial mySerial(2,3); // RX = D2, TX  = D3

void setup() {
  Serial.begin(115200); //Start Serial
  mySerial.begin(115200); //Start mySerial
  pinMode(2,INPUT); //d2 is RX, receiver, so define it as input
  pinMode(3,OUTPUT); //d3 is TX, transmitter, so define it as output

  emon1.current(A0, 75);             // Current: input pin, calibration.
} // end of setup

void loop() {
  
  
  double Irms = emon1.calcIrms(1500);  // Calculate Irms only

//  String message = ("Cruj Cruj I = %ld A, P = %ld W", Irms, Irms*230);
  mySerial.print("P = ");
  mySerial.print(Irms*230);
  mySerial.print("W");
  mySerial.print("\n");
  
  delay(20); // Wait for a sec.

} // end of loop
