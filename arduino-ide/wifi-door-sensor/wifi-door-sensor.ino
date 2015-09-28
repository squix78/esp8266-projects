/*
 *  This sketch sends data via HTTP GET requests to thingspeak service every 5 minutes
 *  You have to set your wifi credentials and your thingspeak key.
 */

#include <ESP8266WiFi.h>


const char* ssid     = "SSID";
const char* password = "PASSWORD";


const char* host = "api.thingspeak.com";
const char* thingspeak_key = "XXX";

#define MINUTES_SLEEPING  5

#define DOOR_PIN 5 // NODEMCU: D1

int retryCounter = 0;

void setup() {
  Serial.begin(115200);
  pinMode(DOOR_PIN, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retryCounter++;
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/update?key=";
  url += thingspeak_key;
  url += "&field1=";
  url += String(digitalRead(DOOR_PIN));
  url += "&field2=";
  url += retryCounter;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection. going to sleep...");
  ESP.deepSleep(1000 * 1000 * 60 * MINUTES_SLEEPING, WAKE_NO_RFCAL);
}

