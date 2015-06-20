#include <Wire.h>
#include "HX711.h"
#include "ssd1306_i2c.h"

// HX711.DOUT	- pin #A1
// HX711.PD_SCK	- pin #A0

HX711 scale(12, 14);		// parameter "gain" is ommited; the default value 128 is used by the library
SSD1306 display(0x3c, 2, 0);

void setup() {
  ESP.wdtDisable();
  
  Serial.begin(115200);
  Serial.println("HX711 Demo");
  
  display.init();
  display.clear();
  display.display();
  
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());			// print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));  	// print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided 
						// by the SCALE parameter (not set yet)  

  scale.set_scale(2158.45f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  //scale.set_scale();
  scale.tare();				        // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 
						// by the SCALE parameter set with set_scale

  Serial.println("Readings:");
}

void loop() {
  //Serial.print("one reading:\t");
  Serial.println(scale.get_units(), 1);
  display.clear();
  display.setFontScale2x2(true);
  display.drawString(10,10, String(scale.get_units(), 1) + "g");
  display.display();
  
  //Serial.print("\t| average:\t");
  //Serial.println(scale.get_units(10), 1);

  //scale.power_down();			        // put the ADC in sleep mode
  delay(100);
  //scale.power_up();
}
