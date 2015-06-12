# esp8266-Weather Station V2

This is a total make over of the earlier weather station. Instead of building my code
on the NodeMCU lua firmware I decided to switch to the Arduino IDE based development setup.
The reason for this was the constant problems with heap memory running out and other instabilities.
When I finally found code that demonstrated the use of my beloved SSD1306 based OLED 128x64 displays
the final piece was in place.

I tried to separate the building blocks of the code into different classes, so they can be reused
easiley for other purposes. There is a class for the SSD1306 display and one for talking to the weather API
of forecast.io. 

## SSD1306

This class interfaces with the display over the I2C bus. Please be aware that I set the bus speed relatively 
high, in order to have faster refreshes of the display. It can happen that for some reason this value is 
too high for your setup. Then you'd have to experiment with different values. This class is based on code from @mikerankin. Thank you a lot, Mike for deciphering the datasheets!

## WeatherClient

I'm using the forecast.io API to retrieve localized weather information. The API returns huge JSON objects
which I thought would be hard to handle on the ESP8266. For this reason I wrote a little php script which 
selects interesting information and transforms it into key=value pairs, separated by a new-line charachter.
This can be handled very memory efficiently on the ESP8266. This step costed some self-convincing, since 
I'm usually a big fan of well structured data. But you have to think differently on embedded platforms

If you find any bugs or code with room for improvement please let me know. C/C++ is not my mother tongue...
The best way to tell me about improvments are pull requests (which is a github feature...).

The weather icons are from http://www.alessioatzeni.com/meteocons/
Alessio is accepting donations for his work

See a video here: http://blog.squix.ch/2015/06/esp8266-weather-station-v2-code.html
