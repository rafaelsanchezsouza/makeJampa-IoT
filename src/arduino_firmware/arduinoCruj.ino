#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance

String message = "";
bool messageReady = false;

void setup() {
  Serial.begin(9600);
  emon1.current(A0, 75);             // Current: input pin, calibration.
}

void loop() {
  delay(500);
  double Irms = emon1.calcIrms(1500);  // Calculate Irms only
  Serial.println(Irms*230);
}