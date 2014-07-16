/*
  Sparkfun's Button Pad library
  Build for Arduino Mega
  Created by js at Interacting Objects, July, 2014.
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

class rgbLedMatrix {

  public:

    // Matrix constructor
    rgbLedMatrix(byte userPinLedRGB[ROWS][3], byte userPinLedGnd[COLS], byte numRows, byte numCols);
    
    // Matrix basic functions
    void matrixLedToggleState(byte row, byte col, byte color[3]); // Toogling on/off (need to specify the color) 
    byte matrixLedGetState(byte row,byte col);                    // Getting led current state (ON/OFF)
    byte* matrixLedGetColor(byte row,byte col);                   // Getting led current color
    void matrixLedSetState(byte row, byte col, byte color[3]);    // Setting led color
    void matrixLedSetRandom(byte row, byte col, byte min=0, byte max=255);  // Setting a random color
    void matrixLedSetAll(byte color[3]);                          // Setting all leds to the very same color
    void matrixLedSetAll(byte colors[ROWS][COLS][3]);             // Setting all leds to specific colors
    void matrixLedSetAllRandom();                                 // Setting all leds to random colors

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
    void ledTestAll(byte* color);
    
    // Debugging helpers
    void dbgPrintLedColor();
    void dbgPrintLedLockState();
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