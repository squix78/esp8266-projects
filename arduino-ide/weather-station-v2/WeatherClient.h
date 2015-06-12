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

#include <Arduino.h>

class WeatherClient {
  private:
    int currentTemp;
    int currentHumidity;
    String currentIcon;
    String currentSummary;
    String iconToday;
    int maxTempToday;
    int minTempToday;
    String summaryToday;
    int maxTempTomorrow;
    int minTempTomorrow;
    String iconTomorrow;
    String summaryTomorrow;
    String apiKey;
    String myUnits = "auto";
    String myLanguage;
    
    String getValue(String line);
    String getKey(String line);
  
  public:
    void updateWeatherData(String apiKey, double lat, double lon); 
    void setUnits(String units);
    int getCurrentTemp(void);
    int getCurrentHumidity(void);
    String getCurrentIcon(void);
    String getCurrentSummary(void);
    String getIconToday(void);
    int getMaxTempToday(void);
    int getMinTempToday(void);
    String getSummaryToday(void);
    int getMaxTempTomorrow(void);
    int getMinTempTomorrow(void);
    String getIconTomorrow(void);
    String getSummaryTomorrow(void);
    
  
};
