/**The MIT License (MIT)
ESP8622 Weather Display v3
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

*/

#include "ForecastIOParser.h"
#include "JsonStreamingParser.h"
#include "JsonListener.h"

JsonStreamingParser parser;
ForecastIO forecast;

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

ForecastIO::ForecastIO(){
  parser.setListener(&forecast);
  }

void ForecastIO::updateWeatherData( String apiKey, double lat, double lon) {
  HTTPClient http;
  Serial.println("Connecting to server: ");
  
  // We now create a URI for the request
  String url="https://api.forecast.io/forecast/" + apiKey + "/" + String(lat)+"," + String(lon);
  
  http.begin( url );
  Serial.print("Requesting URL: ");
  Serial.println(url);
  int httpCode = http.GET();
  if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      // get lenght of document (is -1 when Server sends no Content-Length header)
      int len = http.getSize();
      // create buffer for read
      char buff[128] = { 0 };
      // get tcp stream
      WiFiClient * stream = http.getStreamPtr();
      // read all data from server
      Serial.println("Start parsing...");
      while(http.connected() && (len > 0 || len == -1)) {
        // get available data size
        size_t size = stream->available();
        if(size) {
          // read up to 128 byte
          int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

          for(int i=0;i<c;i++)
            parser.parse(buff[i]); 

          if(len > 0)
            len -= c;
          }
        delay(1);
        }
      /*
      Serial.println();
      Serial.println( "Current");
      Serial.println( listener.current.temperature );
      Serial.println( listener.current.humidity );
      Serial.println( listener.current.icon );
      Serial.println( listener.current.summary );
      Serial.println( "Today");
      Serial.println( listener.today.temperatureMin );
      Serial.println( listener.today.temperatureMax );
      Serial.println( listener.today.icon );
      Serial.println( listener.today.summary );
      Serial.println( "Tomorrow");
      Serial.println( listener.tomorrow.temperatureMin );
      Serial.println( listener.tomorrow.temperatureMax );
      Serial.println( listener.tomorrow.icon );
      Serial.println( listener.tomorrow.summary );
      */
      Serial.println();
      Serial.print("[HTTP] connection closed or file end.\n");
      }
    }
  else
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    
  http.end();
  
  Serial.println();
  Serial.println("closing connection");    
}



void ForecastIO::whitespace(char c) {
  //Serial.println("whitespace");
}

void ForecastIO::startDocument() {
  //Serial.println("start document");
}

void ForecastIO::key(String key) {
  //Serial.println("key: " + key);
  token=key;
}

void ForecastIO::value(String value){
  //Serial.println("value: " + value);
  struct weatherData *a;
  a=NULL;
  if(object=="currently")
    a=&current;
  else if(object=="daily" && arraynum==1)
    a=&today;
  else if(object=="daily" && arraynum==2)
    a=&tomorrow;

  if(a!=NULL){
    if(token=="temperatureMax")
      a->temperatureMax=value;
    else if(token=="temperatureMin")
      a->temperatureMin=value;
    else if(token=="temperature")
      a->temperature=value;
    else if(token=="icon")
      a->icon=value;
    else if(token=="summary")
      a->summary=value;
    else if(token=="humidity")
      a->humidity=value;
    }
}

void ForecastIO::endArray() {
  //Serial.println("end array. ");
}

void ForecastIO::endObject() {
  //Serial.println("end object. ");
}

void ForecastIO::endDocument() {
  //Serial.println("end document. ");
}

void ForecastIO::startArray() {
   //Serial.println("start array. ");
   arraynum=0;
}

void ForecastIO::startObject() {
   //Serial.println("start object. ");
   arraynum++;
   if( token == "currently" || token == "daily" )
      object=token;
   }


