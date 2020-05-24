#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// const char* ssid = "Rareriroru2G";
// const char* password = "INTEGRAL";
// const char* ssid = "VIVOFIBRA-0CA4";
// const char* password = "EAEA8D0CA4";
#define WLAN_SSID       "Rareriroru2G"
#define WLAN_PASS       "INTEGRAL"


const char* mqtt_server = "54.174.96.185";
const int mqtt_port = 1883;
const char* outTopic = "@sensores/sensor1";


WiFiClient espClient;
PubSubClient client(espClient);


char msg2[50];
boolean chegouMensagem = false;


void setup() {
  Serial.begin(9600);
  delay(10);
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  // Setup MQTT subscription for onoff feed.

}

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (Serial.available() > 0) {
    int i = 0;
    msg2[0] = 0;
    while (Serial.available()  > 0 || !chegouMensagem ) {
      char inChar = (char)Serial.read();
      msg2[i] = inChar;
      delay(50);
      i++;
      if (inChar == '\n')
      {
        delay(50);
        chegouMensagem = true;
      }
    }
  }
  if (chegouMensagem) {
    Serial.print("Enviando mensagem: ");
    Serial.println(msg2);
    chegouMensagem = false;
    client.publish(outTopic, msg2);
  }
}



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

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
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}