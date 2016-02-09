/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn
Copyright (c) 2016 by Erdem Umut Altinyurt

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
#include "ForecastIOParser.h"

// Initialize the oled display for address 0x3c
// sda-pin=14 and sdc-pin=12
SSD1306 display(0x3c, 14, 12);
extern ForecastIO forecast;
Ticker ticker;

// your network SSID (name)
char ssid[] = "<SSID>";
// your network password
char pass[] = "<PASSWORD>";  

// Go to forecast.io and register for an API KEY
String forecastApiKey = "<APIKEY>";

// Use celsius by default, set 0 for fahrenheit
#define UNIT_CELSIUS 1

// Coordinates of the place you want
// weather information for Istanbul
double latitude = 40.98;
double longitude = 28.86;

void drawFrame1(int x, int y);
void drawFrame2(int x, int y);
void drawFrame3(int x, int y);
void drawSpinner(int count, int active);
void setReadyForWeatherUpdate();
const char* getIconFromString(String icon);

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
void (*frameCallbacks[3])(int x, int y) = {drawFrame1, drawFrame2, drawFrame3};

// how many frames are there?
int frameCount = 3;
// on frame is currently displayed
int currentFrame = 0;

// flag changed in the ticker function every 10 minutes
bool readyForWeatherUpdate = true;

void setup() {
  // initialize dispaly
  display.init();
  // For some users
  //display.flipScreenVertically();
  // set the drawing functions
  display.setFrameCallbacks(3, frameCallbacks);
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
  int heartbeat=1;
  int heartspeed=+15;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    display.clear();
    display.drawXbm(34, 10, 60, 36, WiFi_Logo_bits);
    //display.setColor(INVERSE);
    //display.fillRect(10, 10, 108, 44);
    //display.setColor(WHITE);
    display.setContrast(heartbeat%256);
    heartbeat+=heartspeed;
    if(heartbeat > 255){
      heartspeed*=-1;
      heartbeat=255;
      }
    else if(heartbeat <1){
      heartspeed*=-1;
      heartbeat=1;
    }
    display.drawString(27, 50 , "Connecting" );

    //drawSpinner(3, counter % 3);
    display.display();
    delay(5);
    counter++;
    /*
    delay(500);
    Serial.print(".");
    display.clear();
    display.drawXbm(34,10, 60, 36, WiFi_Logo_bits);
    display.setColor(INVERSE);
    display.fillRect(10, 10, 108, 44);
    display.setColor(WHITE);
    drawSpinner(3, counter % 3);
    display.display();
    counter++;
    */
  }
  display.setContrast(0);//Power Save and keep oleds from burning.

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
    readyForWeatherUpdate = false;
    forecast.updateWeatherData(forecastApiKey, latitude, longitude);  
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

String unitConv( String fahrenheit ){
  float fvar = fahrenheit.toFloat();
  int val = fvar;
  if( UNIT_CELSIUS )
    val = (fvar-32)/1.8 ; //that deletes floating point
  String ret(val);
  ret+= UNIT_CELSIUS ? "C":"F";
  return ret;
  }

void drawFrame1(int x, int y) {
  display.setFontScale2x2(false);
  display.drawString(65 + x, 8 + y, "Now");
  display.drawXbm(x + 7, y + 7, 50, 50, getIconFromString( forecast.current.icon ));
  display.setFontScale2x2(true);
  display.drawString(64 + x, 20 + y, unitConv(forecast.current.temperature) );
  display.setFontScale2x2(false);
  display.drawString(64 + x, 40 + y, forecast.current.summary);
}

void drawFrame2(int x, int y) {
  display.setFontScale2x2(false);
  display.drawString(65 + x, 0 + y, "Today");
  display.drawXbm(x, y, 60, 60, xbmtemp);
  display.setFontScale2x2(true);
  display.drawString(64 + x, 14 + y, unitConv(forecast.current.temperature) );
  display.setFontScale2x2(false);
  display.drawString(66 + x, 40 + y, unitConv( forecast.today.temperatureMin) + "/" + unitConv(forecast.today.temperatureMax) );
}

void drawFrame3(int x, int y) {
  display.drawXbm(x + 7, y + 7, 50, 50, getIconFromString(forecast.tomorrow.icon) );
  display.setFontScale2x2(false);
  display.drawString(65 + x, 7 + y, "Tomorrow");
  display.setFontScale2x2(true);
  display.drawString(64 + x, 20 + y, unitConv(forecast.tomorrow.temperatureMax) );
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

