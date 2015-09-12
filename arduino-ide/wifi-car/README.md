# WiFi Car

This sketch uses the NodeMCU V1.0 motorshield to control a smart car. You can use the inclination of your smart device to steer the two engines forwards, backwards or in any desired direction.

See more here: http://blog.squix.ch/2015/09/esp8266-nodemcu-motor-shield-review.html

## Setup Instructions

To make this work you'll have to adapt ssid and password according to your access point in the ino file. Then you'll have to find out the IP of your NodeMCU and open http://<myip> in your smartphone browser. This will load the index.html from the sketch which in turns loads the smartcar.js from my server. If you want to change the javascript you'll have to host the javascript file on your own server
