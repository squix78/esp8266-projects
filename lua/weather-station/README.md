# Internet Connected Weather Station

This project only requires three parts
* [NodeMCU V1.0 (~$10)](http://s.click.aliexpress.com/e/JYrbmMVfU)
* [OLED Display 128x64, I2C (~$4.50)] (http://www.banggood.com/0_96-Inch-4Pin-White-IIC-I2C-OLED-Display-Module-12864-LED-For-Arduino-p-958196.html?p=0P21061109440201501M)
* [DuPont Connectors (female-female) (~$1.50)](http://www.banggood.com/40-x-10cm-Female-To-Female-Dupont-Jumper-Wires-Cable-p-89717.html?p=0P21061109440201501M)

Then follow these steps:

1. Download this source code from github (either checkout with git or use the "[Download as ZIP](https://github.com/squix78/esp8266-projects/archive/master.zip)" Button
2. Download [ESPlorer](http://esp8266.ru/esplorer/)
3. Open init.lua in ESPlorer and replace SSID and PASSWORD with your Wifi credentials
4. Open weatherStation.lua in ESPlorer and adapt the city for which to receive weather information
5. Register at openweathermap.org to get an API key
6. Use "Safe to ESP" to transfer the file. Use "Safe&Compile" to compile the lua source code into an lc file to safe valuable RAM
7. Use the "Upload..." button in ESPlorer to transfer all the .MONO files to the ESP
8. Connect all the wires as depicted in the schema below
9. Reset the ESP. 

If everything went well you should see the current weather of your city on the tiny OLED display.

Please let me know, if you see room for improvement. Also visit http://blog.squix.ch for more projects.


![The Weather Station Display](http://2.bp.blogspot.com/-W-UZsPvh-4I/VWCjUVTzxqI/AAAAAAAAATc/bmdILYERtGo/s320/display.jpg)
![Connecting the wires](https://github.com/squix78/esp8266-projects/blob/master/weather-station/simple-oled-sketch_bb.png)
