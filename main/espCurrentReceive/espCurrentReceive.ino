#include <SoftwareSerial.h>

#define D7 13
#define D8 15

SoftwareSerial mySerial(D7, D8); // RX = D7, TX  = D8

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
int i = 0;

void setup() {
  Serial.begin(115200); //Start Serial
  mySerial.begin(115200); //Start mySerial
  pinMode(D7,INPUT); //d7 is RX, receiver, so define it as input
  pinMode(D8,OUTPUT); //d8 is TX, transmitter, so define it as output
  inputString.reserve(200);
} // end of setup

void loop() {
//  Serial.println("Cruj");
  while(!stringComplete) {
        // get the new byte:
        i++;
        char inChar = (char)mySerial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
        if (inChar == '\n')
        {
            delay(50);
            stringComplete = true;
        }
        delay(50);
        
  }
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  delay(50);
}

//void serialEvent()
//{
//    while (Serial.available())
//    {
//        // get the new byte:
//        i++;
//        Serial.print("i = ");
//        char inChar = (char)Serial.read();
//        // add it to the inputString:
//        inputString += inChar;
//        // if the incoming character is a newline, set a flag so the main loop can
//        // do something about it:
//        if (inChar == '\n')
//        {
//            stringComplete = true;
//        }
//    }
//}
