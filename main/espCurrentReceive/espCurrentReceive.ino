#include <SoftwareSerial.h>

#define D7 13
#define D8 15

SoftwareSerial mySerial(D7, D8); // RX = D7, TX  = D8

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
int i = 0;
char msg2[100];

void setup() {
  Serial.begin(115200); //Start Serial
  mySerial.begin(115200); //Start mySerial
  pinMode(D7,INPUT); //d7 is RX, receiver, so define it as input
  pinMode(D8,OUTPUT); //d8 is TX, transmitter, so define it as output
  inputString.reserve(200);
} // end of setup

void loop() {
//  Serial.println("Cruj");'
  
  if(!stringComplete) {

    // while (mySerial.available()>0) {
      // get the new byte:
      // Serial.print("CRUJ");
      char inChar = (char)mySerial.read();
      // add it to the inputString:
      inputString += inChar;
      // msg2[i] = inChar;
      // if the incoming character is a newline, set a flag so the main loop can
      // do something about it:
      // i++;
      if (inChar == '\n')
      {
        delay(50);
        stringComplete = true;
        // Serial.print("STRING TRUE");
      }
      delay(50);
    }

  if (stringComplete) {
    Serial.print(inputString);
    // Serial.print("String Complete");
    // Serial.print(msg2);
    // clear the string:
    inputString = "";
    stringComplete = false;
    // Serial.print("String Complete False");
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
