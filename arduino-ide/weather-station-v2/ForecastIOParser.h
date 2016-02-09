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

#pragma once

#include "JsonListener.h"

struct weatherData{
  String temperatureMin;
  String temperatureMax;
  String humidity;
  String temperature;
  String icon;
  String summary;
  };

class ForecastIO: public JsonListener {
  public:
    ForecastIO();
    void updateWeatherData( String apiKey, double lat, double lon);

  public:
    virtual void whitespace(char c);
    virtual void startDocument();
    virtual void key(String key);
    virtual void value(String value);
    virtual void endArray();
    virtual void endObject();
    virtual void endDocument();
    virtual void startArray();
    virtual void startObject();

  struct weatherData current, today, tomorrow;
      
  private:
    String apiKey;
    String token;
    String object;
    int arraynum;
};


