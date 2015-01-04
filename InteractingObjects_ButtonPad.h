/*
  Sparkfun's Button Pad library
  Build for Arduino Mega
 
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

*/

#ifndef buttonPad_h
#define buttonPad_h

#include "Arduino.h"

// Led statuses
#define ON 0x0
#define OFF 0x1

// Button statuses
#define LOCKED 0x0
#define UNLOCKED 0x1

const byte ROWS=4;
const byte COLS=4;

// Translate key number (0 to 15) to x/y address
#define KEY2COL(key) COLS-(key%COLS)-1
#define KEY2ROW(key) key/ROWS

// Convert x/y (0/0 is top left, 3/3 is bottom right) 
// to physical address (0/0) is top left, 3/3 is bottom right
#define COL2ADDR(col) COLS-col-1 
#define ROW2ADDR(row) row

class rgbLedMatrix {

  public:

    // Matrix constructor
    rgbLedMatrix(byte userPinLedRGB[ROWS][3], byte userPinLedGnd[COLS], byte numRows, byte numCols);
    
    // Matrix basic functions
    void matrixLedToggleState(byte row, byte col, byte color[3]); // Toogling on/off (need to specify the color) 
    byte matrixLedGetState(byte row,byte col);                    // Getting led current state (ON/OFF)
    byte matrixLedGetState(byte key);                             // Getting led current state (ON/OFF)
    byte* matrixLedGetColor(byte row,byte col);                   // Getting led current color
    void matrixLedSetState(byte row, byte col, byte color[3]);    // Setting led color
    void matrixLedSetState(byte key, byte color[3]);              // Setting led color
    void matrixLedSetOff(byte row, byte col);
    void matrixLedSetOff(byte key);
    void matrixLedSetRandom(byte row, byte col, byte min=0, byte max=255);  // Setting a random color
    void matrixLedSetAll(byte color[3]);                          // Setting all leds to the very same color
    void matrixLedSetAll(byte colors[ROWS][COLS][3]);             // Setting all leds to specific colors
    void matrixLedSetAllRandom();                                 // Setting all leds to random colors
    void matrixLedSetAllOff();


    // Matrix led "locking" function
    void matrixLedToggleLock(byte row,byte col);   // Toggle lonck state (LOCK/UNLOCK)
    void matrixLedLock(byte row,byte col);         // Locking led
    void matrixLedUnlock(byte row,byte col);       // Unlocking led
    byte matrixLedGetLockState(byte row,byte col); // Getting lock status
    
    // Effectively displaying colors
    void matrixLedRefresh(int interval = 130);
    void matrixLedRefreshDemo(int interval=1);

    // Individual leds function (1 led at a time)
    void ledSetState(byte row, byte col, byte color[3]);
    void ledSetRandom(byte row, byte col, byte min=0, byte max=255);
    void ledSetOff(byte row, byte col);
    void ledSetAllOff();
    void ledTestAll(byte* color);
    void ledTestMatrix(int period = 200);
    
    // Debugging helpers
    void dbgMatrixPrintLedColor();
    void dbgMatrixPrintLedLockState();
    void dbgPrintColor(byte color[3]);

  private:
    
    byte pinLedRGB[ROWS][3];
    byte pinLedGnd[COLS];

    byte ledColor[ROWS][COLS][3];
    byte ledLockState[ROWS][COLS];
    
    void matrixRowActivate(byte row,byte color[3]);
    void matrixRowDeactivate(byte row);
    void matrixColActivate(byte col);
    void matrixColDeactivate(byte col);  
};

#endif