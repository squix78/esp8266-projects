#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>

#include "HX711.h"
#include "ssd1306_i2c.h"
#include "icons.h"
#include "font.h"


SSD1306 display(0x3c, 2, 0);
int counter = 0;

// your network SSID (name)
char ssid[] = "<ssid>";
// your network password
char pass[] = "<pwd>"; 

char thingspeakKey[] = "<thingspeakKey>";

const char* host = "api.thingspeak.com";

const char *xbm [] = {plant0000_bits, plant0125_bits, plant0250_bits, plant0375_bits, plant0500_bits, plant0625_bits, plant0750_bits, plant0875_bits, plant1000_bits};

// NodeMCU pins 
HX711 scale(12, 14);		// parameter "gain" is ommited; the default value 128 is used by the library

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
void (*frameCallbacks[2])(int x, int y) = {drawFrame1, drawFrame2};

// how many frames are there?
int frameCount = 3;

// on frame is currently displayed
int currentFrame = 0;

float emptyWeight = 550.f;
float fullWeight = 1000.f;
float waterMax = fullWeight - emptyWeight;
float currentWeight = 0;

Ticker ticker;
bool isReadyForThingspeakUpdate = true;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting up...");
  // The HX711 functions do not yield. Watchdog would reset
  ESP.wdtDisable();
  
  scale.set_scale(2158.45f); 
  scale.tare();
  currentWeight = scale.get_units(1);
  
  display.init();
  // set the drawing functions
  display.setFrameCallbacks(2, frameCallbacks);
  // how many ticks does a slide of frame take?
  display.setFrameTransitionTicks(10);
  display.setFrameWaitTicks(200);
  display.clear();
  display.display();
  
  WiFi.begin(ssid, pass);
  
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.clear();
    display.drawXbm(34,10, 60, 36, WiFi_Logo_bits);
    drawSpinner(3, counter % 3);
    display.display();
    counter++;
  }
  Serial.println("");  

  // update the weather information every 10 mintues only
  // forecast.io only allows 1000 calls per day
  ticker.attach(60, setReadyForThingspeakUpdate);

}

void setReadyForThingspeakUpdate() {
  isReadyForThingspeakUpdate = true;  
}

void loop() {
  // only update measuement, when frames not in transition
  if (display.getFrameState() == display.FRAME_STATE_FIX && counter % 10 == 0) {
    currentWeight = scale.get_units(5);
  }
  if (display.getFrameState() == display.FRAME_STATE_FIX && isReadyForThingspeakUpdate && currentWeight > 200) {
    isReadyForThingspeakUpdate = false;
    updateThingspeak();
  }
  
  display.clear();
  display.nextFrameTick();
  display.display();
  counter++;

}

void drawFrame1(int x, int y) {

  float currentWater = max(0, currentWeight - emptyWeight);
  float currentLevel = currentWater / waterMax;

  int icon = min(8, currentLevel * 8); 
  Serial.println(String(currentWeight) + "\t" + String(currentWater) + "\t"+ String(currentLevel) + "\t" + String(icon));

  
  display.drawXbm(0 + x, 0 + y, 60, 60, xbm[icon]);
  display.setFontScale2x2(false);
  display.drawString(64 + x, 0 + y, "Water:");
  display.setFontScale2x2(true);
  display.drawString(64 + x, 10 + y, String(int(currentLevel * 100)) + "%");
  display.drawString(64 + x, 30 + y, String(int(currentWater)) + "g");

}

void drawFrame2(int x, int y) {
  float currentWater = max(0, currentWeight - emptyWeight);
  float currentLevel = currentWater / waterMax;

  int icon = min(8, currentLevel * 8); 
  Serial.println(String(counter) + ".\t" +  String(currentWeight) + "\t" + String(currentWater) + "\t"+ String(currentLevel) + "\t" + String(icon));

  display.setFontScale2x2(true);
  display.drawString(0 + x, 0 + y, String(currentLevel * 100) + "%");
  display.drawString(0 + x, 20 + y, String(currentWater) + "ml");
  display.drawString(0 + x, 40 + y, String(currentWeight) + "g");

}

void drawSpinner(int count, int active) {
  for (int i = 0; i < count; i++) {
    const char *xbm;
    if (active == i) {
       xbm = active_bits;
    } else {
       xbm = inactive_bits;  
    }
    display.drawXbm(64 - (12 * count / 2) + 12 * i,56, 8, 8, xbm);
  }   
}

void updateThingspeak() {
  Serial.println("Updating thingspeak...");
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  float currentWater = max(0, currentWeight - emptyWeight);
  float currentLevel = currentWater / waterMax;

  String url = "/update?key=";
  url += thingspeakKey;
  url += "&field1=";
  url += String(currentWeight);
  url += "&field2=";
  url += String(currentWater);
  url += "&field3=";
  url += String(currentLevel * 100);

  // This will send the request to the server
  String request = String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n";
  Serial.println(request);
  client.print(request);
  delay(1000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

}


