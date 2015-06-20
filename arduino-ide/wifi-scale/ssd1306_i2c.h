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

Credits for parts of this code go to Mike Rankin. Thank you so much for sharing!
*/

#include <Arduino.h>

#define BLACK 0
#define WHITE 1
#define INVERSE 2


class SSD1306 {

private:
   int myI2cAddress;
   int mySda;
   int mySdc;
   uint8_t buffer[128 * 64 / 8];
   bool myIsFontScaling2x2 = false;
   int myFrameState = 0;
   int myFrameTick = 0;
   int myCurrentFrame = 0;
   int myFrameCount = 0;
   int myFrameWaitTicks = 100;
   int myFrameTransitionTicks = 25;
   int myColor = WHITE;
   void (**myFrameCallbacks)(int x, int y);

   
public:
   // Empty constructor
   SSD1306(int i2cAddress, int sda, int sdc);
   void init();
   void resetDisplay(void);
   void reconnect(void);
   void displayOn(void);
   void displayOff(void);
   void clear(void);
   void display(void);
   void setPixel(int x, int y);
   void setChar(int x, int y, unsigned char data);
   void drawString(int x, int y, String text);
   void setFontScale2x2(bool isFontScaling2x2);
   void drawBitmap(int x, int y, int width, int height, const char *bitmap);
   void drawXbm(int x, int y, int width, int height, const char *xbm);
   void sendCommand(unsigned char com);
   void sendInitCommands(void);
   void setColor(int color);
   void drawRect(int x, int y, int width, int height);
   void fillRect(int x, int y, int width, int height);
   
   void setContrast(char contrast);
   void flipScreenVertically();
   
   
   void setFrameCallbacks(int frameCount, void (*frameCallbacks[])(int x, int y));
   void nextFrameTick(void);
   void drawIndicators(int frameCount, int activeFrame);
   void setFrameWaitTicks(int frameWaitTicks);
   void setFrameTransitionTicks(int frameTransitionTicks);
   int getFrameState();
   
   const int FRAME_STATE_FIX = 0;
   const int FRAME_STATE_TRANSITION = 1;

};
