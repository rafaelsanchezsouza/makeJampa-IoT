/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EmonLib.h"                   // Include Emon Library
#include <SoftwareSerial.h>

#define D7 13
#define D8 15
#define emonTxV3

SoftwareSerial mySerial(D7, D8); // RX = D7, TX  = D8
EnergyMonitor emon1;                   // Create an instance



// Update these with values vsuitable for your network.

// const char* ssid = "Rareriroru2G";
// const char* password = "INTEGRAL";
const char* ssid = "VIVOFIBRA-0CA4";
const char* password = "EAEA8D0CA4";
const char* mqtt_server = "54.174.96.185";
const int mqtt_port = 1883;
const char* outTopic = "@sensores/sensor1";

WiFiClient espClient;
PubSubClient client(espClient);
int SupplyVoltage=3300;
int sampleI;
double offsetI;                          //Low-pass filter output
double sqV,sumV,sqI,sumI,instP,sumP;              //sq = squared, sum = Sum, inst = instantaneous
double filteredI;
double ICAL;

//Serial Communication
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
int i = 0;


long before = 0;
long msgSend = 0;
char msg[50];
int value = 0;
int msgNumber = 0;
int sensor = 0;
long CurrentMillis, PreviousMillis = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "hello cruj");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void receiveString() {
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
    //envia mensagem
    snprintf (msg, 50, "%s",inputString);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(outTopic, msg);

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  delay(50);
}

void setupIoT() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  mySerial.begin(115200); //Start mySerial
  pinMode(D7,INPUT); //d7 is RX, receiver, so define it as input
  pinMode(D8,OUTPUT); //d8 is TX, transmitter, so define it as output
  inputString.reserve(200);
  emon1.current(A0, 80);             // Current: input pin, calibration.
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loopIoT() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  // double Irms = emon1.calcIrmsPB(200);  // Calculate Irms only

  
  
  if (now - before > 2000) {
    ++value;
    receiveString()
    before = now;
  }
}

