/*
Test program to check connection and I2C protocol between ESP-01 and SHT21 temp and hum sensor.
Test sending data in MQTT PROTOCOL
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#define OUT 1

const char* ssid = SSID;
const char* password = PASSWORD;
const char* mqtt_server = MQTT_SERVER;
const char* mqtt_topic = MQTT_TOPIC;
const char* mqtt_username = MQTT_USERNAME;
const char* mqtt_password = MQTT_PASSWORD;
const char* clientID = CLIENT_ID;
String control = "0";

WiFiClient espClient;
PubSubClient client(mqtt_server, 1883, espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(100)
char msg[MSG_BUFFER_SIZE];
int value = 0;


void setup()
{
  Serial.begin(115200);
  delay(50);
  con_wifi_mqtt();
  delay(500);
  client.setCallback(callback);
  client.subscribe("heating/+");
  pinMode(OUT, OUTPUT); 
  delay(1000);
  digitalWrite(OUT, LOW);
}

void loop()
{
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 10000) {
    
    if (control == "1"){
      Serial.print("Heating ON");
      digitalWrite(OUT, HIGH);
    }
    else{
      digitalWrite(OUT, LOW);
      Serial.print("Heating OFF");
    }


    
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "PING");
    delay(500);
    client.publish(mqtt_topic, msg);
    Serial.print("MSG published to topic: ");
    Serial.println(mqtt_topic);
    delay(500);
  }
  
}

void con_wifi_mqtt() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
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

  while (!client.connected()){
    Serial.println("Connecting to MQTT broker...");
    client.connect(clientID, mqtt_username, mqtt_password);
    if (!client.connected()){
      Serial.println("Reconnecting in 5s. ...");
      delay(5000);
    }
  }
  Serial.print("Connected to MQTT broker.");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    control = (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
