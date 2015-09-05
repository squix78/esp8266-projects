#include <DHT.h>

/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

See more at http://blog.squix.ch
*/

#include <Wire.h>
#include <Ticker.h>

#include "ssd1306_i2c.h"
#include "icons.h"
#include <ESP8266WiFi.h>
#include "WeatherClient.h"

#define DHTTYPE DHT22
#define DHTPIN  4

// Initialize DHT sensor 
// NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
// you need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// This is for the ESP8266 processor on ESP-01 
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266
float humidity, temp_f;  // Values read from sensor
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

// Initialize the oled display for address 0x3c
// sda-pin=0 and sdc-pin=2
SSD1306 display(0x3c, 0, 2);
WeatherClient weather;
Ticker ticker;

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
void (*frameCallbacks[4])(int x, int y) = {drawFrame1, drawFrame2, drawFrame3, drawFrame4};

// how many frames are there?
int frameCount = 4;
// on frame is currently displayed
int currentFrame = 0;

// your network SSID (name)
char ssid[] = "AdyShan";
// your network password
char pass[] = "aditya1tannu2";  

// Go to forecast.io and register for an API KEY
String forecastApiKey = "51a65846eba22ea29bf003366702ef20";

// Coordinates of the place you want
// weather information for
double latitude = 34.4274;
double longitude = -118.5458;

// flag changed in the ticker function every 10 minutes
bool readyForWeatherUpdate = true;

void setup() {

  dht.begin();           // initialize temperature sensor

  
  // initialize dispaly
  display.init();
  // set the drawing functions
  display.setFrameCallbacks(4, frameCallbacks);
  // how many ticks does a slide of frame take?
  display.setFrameTransitionTicks(10);

  display.clear();
  display.display();
  
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.clear();
    display.drawXbm(34,10, 60, 36, WiFi_Logo_bits);
    display.setColor(WHITE);
    display.fillRect(10, 10, 108, 44);
    display.setColor(WHITE);
    drawSpinner(4, counter % 4);
    display.display();
    counter++;
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // update the weather information every 10 mintues only
  // forecast.io only allows 1000 calls per day
  ticker.attach(60 * 10, setReadyForWeatherUpdate);
}

void loop() {
  if (readyForWeatherUpdate && display.getFrameState() == display.FRAME_STATE_FIX) {
    gettemperature();       // read sensor  

    
    readyForWeatherUpdate = false;
    weather.setUnits("us");
    weather.updateWeatherData(forecastApiKey, latitude, longitude);

    
  }
  display.clear();
  display.nextFrameTick();
  display.display();
}

void setReadyForWeatherUpdate() {
  readyForWeatherUpdate = true;  
}

const char* getIconFromString(String icon) {
   //"clear-day, clear-night, rain, snow, sleet, wind, fog, cloudy, partly-cloudy-day, or partly-cloudy-night"
  if (icon == "clear-day") {
    return clear_day_bits;
  } else if (icon == "clear-night") {
    return clear_night_bits;  
  } else if (icon == "rain") {
    return rain_bits;  
  } else if (icon == "snow") {
    return snow_bits;  
  } else if (icon == "sleet") {
    return sleet_bits;  
  } else if (icon == "wind") {
    return wind_bits;  
  } else if (icon == "fog") {
    return fog_bits;  
  } else if (icon == "cloudy") {
    return cloudy_bits;  
  } else if (icon == "partly-cloudy-day") {
    return partly_cloudy_day_bits;  
  } else if (icon == "partly-cloudy-night") {
    return partly_cloudy_night_bits;  
  } 
  return cloudy_bits;
}

void drawFrame1(int x, int y) {
   display.setFontScale2x2(false);
   display.drawString(65 + x, 0 + y, "Weather");
   display.drawXbm(x,y, 60, 60, xbmtemp);
   display.setFontScale2x2(true);
   display.drawString(64 + x, 14 + y, String(weather.getCurrentTemp()) + "F");
   display.setFontScale2x2(false);
   display.drawString(66 + x, 40 + y, String(weather.getMinTempToday()) + "F/" + String(weather.getMaxTempToday()) + "F");  
}

void drawFrame2(int x, int y) {
   display.setFontScale2x2(false);
   display.drawString(65 + x, 8 + y, "Inside");
   display.drawXbm(x+7,y+7, 50, 50, getIconFromString(weather.getCurrentIcon()));
   display.setFontScale2x2(true);
   display.drawString(64+ x, 20 + y, String((int)temp_f) + "F");
   display.setFontScale2x2(false);
   display.drawString(104+ x, 50 + y, String((int)humidity) + "%"); 
      
}

void drawFrame3(int x, int y) {
   display.setFontScale2x2(false);
   display.drawString(65 + x, 8 + y, "Outside");
   display.drawXbm(x+7,y+7, 50, 50, getIconFromString(weather.getCurrentIcon()));
   display.setFontScale2x2(true);
   display.drawString(64+ x, 20 + y, String(weather.getCurrentTemp()) + "F"); 
}

void drawFrame4(int x, int y) {
   display.drawXbm(x+7,y+7, 50, 50, getIconFromString(weather.getIconTomorrow()));
   display.setFontScale2x2(false);
   display.drawString(65 + x, 7 + y, "Tomorrow");
   display.setFontScale2x2(true);
   display.drawString(64+ x, 20 + y, String(weather.getMaxTempTomorrow()) + "F");     
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


void gettemperature() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor 
    previousMillis = currentMillis;   

    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    humidity = dht.readHumidity();          // Read humidity (percent)
    temp_f = dht.readTemperature(true);     // Read temperature as Fahrenheit
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp_f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
}

