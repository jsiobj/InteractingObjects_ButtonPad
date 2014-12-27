/*=====================================================================================

  This is an example which makes uses of InteractingObjects_RGBLedMatrix library
  It tests all leds in "matrix mode", i.e. all leds on with different colors, 
  by setting all to white, then turning all led, one by one to red, green and blue.

  For an assembly and connection tutorial see 
  - http://interactingobjects.com/sparkfuns-4x4-button-led-matrix-part-1-soldering/
  - http://interactingobjects.com/sparkfuns-4x4-button-led-matrix-part-2-connecting-to-an-arduino-mega/ 
  - http://interactingobjects.com/sparkfuns-4x4-button-led-matrix-part-3-playing-with-the-pad
  
  The MIT License (MIT)
  
  Copyright (c) 2014 Interacting Objects (http://interactingobjects.com)
  
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

  ======================================================================================*/

#include <InteractingObjects_ButtonPad.h>  // Button pad / led management library
#include <Keypad.h>           // For keypad management


char hexaKeys[ROWS][COLS] = {
  {'3', '2', '1', '0'},
  {'7', '6', '5', '4'},
  {'B', 'A', '9', '8'},
  {'F', 'E', 'D', 'C'}
};

byte pinBtn[ROWS]       = {22,23,24,25}; // Buttons rows
byte pinBtnGnd[COLS]    = {26,28,30,32}; // Buttons cols
byte customPinLedRGB[ROWS][3] = {{11,10,9},{8,7,6},{5,3,2},{44,45,46}}; // LEDs rows
byte customPinLedGnd[COLS]    = {27,29,31,33}; // LEDs cols

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), pinBtn, pinBtnGnd, ROWS, COLS);    // Keypad
rgbLedMatrix ledMatrix = rgbLedMatrix(customPinLedRGB, customPinLedGnd, ROWS, COLS);  // LED matrix

int interval=1000;

void setup() {
  Serial.begin(115200);
}

void loop() {
  
  byte colorWhite[3]={255,255,255};
  byte colorRed[3]={255,0,0};
  byte colorGreen[3]={0,255,0};
  byte colorBlue[3]={0,0,255};
  
  long int now;
  
  ledMatrix.matrixLedSetAll(colorWhite);
  now=millis();
  while(millis()-now<interval) ledMatrix.matrixLedRefresh();

  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledMatrix.matrixLedSetState(row,col,colorRed);
      now=millis();
      while(millis()-now<interval) ledMatrix.matrixLedRefresh();
    }
  }
  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledMatrix.matrixLedSetState(row,col,colorGreen);
      now=millis();
      while(millis()-now<interval) ledMatrix.matrixLedRefresh();
    }
  }
  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledMatrix.matrixLedSetState(row,col,colorBlue);
      now=millis();
      while(millis()-now<interval) ledMatrix.matrixLedRefresh();
    }
  }
  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledMatrix.matrixLedSetOff(row,col);
      now=millis();
      while(millis()-now<interval) ledMatrix.matrixLedRefresh();
    }
  }

}
