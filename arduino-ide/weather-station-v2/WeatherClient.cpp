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

#include "WeatherClient.h"
#include <ESP8266WiFi.h>

void WeatherClient::updateWeatherData(String apiKey, double lat, double lon) {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect("217.26.50.8", httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/rest/weather?apiKey=" + apiKey + "&lat=" + String(lat) + "&lon=" + String(lon) + "&units=" + myUnits;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: www.squix.org\r\n" + 
               "Connection: close\r\n\r\n");
  while(!client.available()) {
    
    delay(200); 
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\n');
    //Serial.println(line);
    String key = getKey(line);
    if (key.length() > 0) {
      String value = getValue(line);
        if (key=="CURRENT_TEMP") {
         currentTemp = value.toInt();
        } else if(key =="CURRENT_HUMIDITY") {
         currentHumidity = value.toInt();
        } else if (key =="CURRENT_ICON") {
         currentIcon = value;
        } else if (key =="CURRENT_SUMMARY") {
         currentSummary = value;
        } else if (key =="MAX_TEMP_TODAY") {
         maxTempToday = value.toInt();
        } else if (key =="MIN_TEMP_TODAY") {
         minTempToday = value.toInt();
        } else if (key =="ICON_TODAY") {
         iconToday = value;
        } else if (key =="SUMMARY_TODAY") {
         summaryToday = value;
        } else if (key =="MAX_TEMP_TOMORROW") {
         maxTempTomorrow = value.toInt();
        } else if (key =="ICON_TOMORROW") {
         iconTomorrow = value;
        } else if (key =="MIN_TEMP_TOMORROW") {
         minTempTomorrow = value.toInt();
        } else if (key =="SUMMARY_TODAY") {
         summaryTomorrow = value;
        } 

    }
    
  }
  
  
  Serial.println();
  Serial.println("closing connection");    
}

void WeatherClient::setUnits(String units) {
   myUnits = units; 
}

String WeatherClient::getKey(String line) {
  int separatorPosition = line.indexOf("=");
  if (separatorPosition == -1) {
    return "";
  }  
  return line.substring(0, separatorPosition);
}

String WeatherClient::getValue(String line) {
  int separatorPosition = line.indexOf("=");
  if (separatorPosition == -1) {
    return "";
  }  
  return line.substring(separatorPosition + 1);
}


int WeatherClient::getCurrentTemp(void) {
  return currentTemp;
}
int WeatherClient::getCurrentHumidity(void) {
  return currentHumidity;
}
String WeatherClient::getCurrentIcon(void) {
  return currentIcon;
}
String WeatherClient::getCurrentSummary(void) {
  return currentSummary;
}
String WeatherClient::getIconToday(void) {
  return iconToday;
}
int WeatherClient::getMaxTempToday(void) {
  return maxTempToday;
}
int WeatherClient::getMinTempToday(void) {
  return minTempToday;
}
String WeatherClient::getSummaryToday(void) {
  return summaryToday;
}
int WeatherClient::getMaxTempTomorrow(void) {
  return maxTempTomorrow;
}
int WeatherClient::getMinTempTomorrow(void) {
  return minTempTomorrow;
}
String WeatherClient::getIconTomorrow(void) {
  return iconTomorrow;
}
String WeatherClient::getSummaryTomorrow(void) {
  return summaryTomorrow;
}
