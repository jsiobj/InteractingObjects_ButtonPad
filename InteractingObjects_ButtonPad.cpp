/*
  Library for Sparkfun's Button Pad on Arduino MEGA
  
  For hardware details, see : 
  - https://www.sparkfun.com/products/7835
  - https://www.sparkfun.com/products/8033
  
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
 
*/
#define DEBUG

#include "debug.h"
#include "InteractingObjects_ButtonPad.h"

//==============================================================================
// Constructor : Essentially setting the pins
//==============================================================================
rgbLedMatrix::rgbLedMatrix(byte userPinLedRGB[ROWS][3], byte userPinLedGnd[COLS], byte numRows, byte numCols) {
  
  for(byte row=0;row<ROWS;row++)
    for(byte i=0; i<3; i++)  
      pinLedRGB[row][i]=userPinLedRGB[row][i];
  
  for(byte col=0;col<COLS;col++) pinLedGnd[col]=userPinLedGnd[col];
     
  for(int row=0;row<numRows;row++) {
    pinMode(pinLedGnd[row],OUTPUT);
    matrixRowDeactivate(row);
    for(int col=0;col<numCols;col++) {
      pinMode(pinLedRGB[row][col],OUTPUT);
      matrixColDeactivate(col);
    }
  }

}

//==============================================================================
// "Matrix" mode functions
//==============================================================================

//-------------------------------------------------------------------------
// Setting colors for one row (turning in on)
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixRowActivate(byte row,byte color[3]) {
  analogWrite(pinLedRGB[ROW2ADDR(row)][0],color[0]);
  analogWrite(pinLedRGB[ROW2ADDR(row)][1],color[1]);
  analogWrite(pinLedRGB[ROW2ADDR(row)][2],color[2]);
}

//-------------------------------------------------------------------------
// Switching off a row
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixRowDeactivate(byte row) {
  analogWrite(pinLedRGB[ROW2ADDR(row)][0],0);
  analogWrite(pinLedRGB[ROW2ADDR(row)][1],0);
  analogWrite(pinLedRGB[ROW2ADDR(row)][2],0);
}

//-------------------------------------------------------------------------
// Switching on a column
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixColActivate(byte col) {
  digitalWrite(pinLedGnd[COL2ADDR(col)],ON);
}

//-------------------------------------------------------------------------
// Switching off a column
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixColDeactivate(byte col) {
  digitalWrite(pinLedGnd[COL2ADDR(col)],OFF);
}

//-------------------------------------------------------------------------
// Toggling lock state
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedToggleLock(byte row,byte col) {
  ledLockState[row][col]=!ledLockState[row][col];
}

//-------------------------------------------------------------------------
// Locking led state
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedLock(byte row,byte col) {
  ledLockState[row][col]=ON;
}

//-------------------------------------------------------------------------
// Unlocking led state
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedUnlock(byte row,byte col) {
  ledLockState[row][col]=OFF;
}

//-------------------------------------------------------------------------
// Getting current locking state
//-------------------------------------------------------------------------
byte rgbLedMatrix::matrixLedGetLockState(byte row, byte col) {
  return ledLockState[row][col];
}

//-------------------------------------------------------------------------
// Getting led current state (ON/OFF)
//-------------------------------------------------------------------------
uint8_t rgbLedMatrix::matrixLedGetState(byte row,byte col) {
  if(ledColor[row][col][0]+ledColor[row][col][1]+ledColor[row][col][2]==0)
    return OFF;
  else
    return ON;
}

uint8_t rgbLedMatrix::matrixLedGetState(byte key) {
  byte row=KEY2ROW(key),col=KEY2COL(key);
  
  if(ledColor[row][col][0]+ledColor[row][col][1]+ledColor[row][col][2]==0)
    return OFF;
  else
    return ON;
}

//-------------------------------------------------------------------------
// Getting led current color
//-------------------------------------------------------------------------
byte* rgbLedMatrix::matrixLedGetColor(byte row,byte col) {
  return ledColor[row][col];
}

//-------------------------------------------------------------------------
// Toogling led on / off
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedToggleState(byte row, byte col, byte color[3]) {

  if(ledColor[row][col][0]+ledColor[row][col][1]+ledColor[row][col][2]==0) {
    matrixLedSetState(row,col,color);
  }
  else {
    ledColor[row][col][0]=0;
    ledColor[row][col][1]=0;
    ledColor[row][col][2]=0;
  }

}

//-------------------------------------------------------------------------
// Setting led color
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedSetState(byte row, byte col, byte color[3]) {
  ledColor[row][col][0]=color[0];
  ledColor[row][col][1]=color[1];
  ledColor[row][col][2]=color[2];
}

void rgbLedMatrix::matrixLedSetState(byte key, byte color[3]) {
  matrixLedSetState(KEY2ROW(key), KEY2COL(key), color);
}

//-------------------------------------------------------------------------
// Setting led off
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedSetOff(byte row, byte col) {
  ledColor[row][col][0]=0;
  ledColor[row][col][1]=0;
  ledColor[row][col][2]=0;
}

void rgbLedMatrix::matrixLedSetOff(byte key) {
  matrixLedSetOff(KEY2ROW(key), KEY2COL(key));
}

//-------------------------------------------------------------------------
// Setting led to random color
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedSetRandom(byte row, byte col, byte min, byte max) {
  ledColor[row][col][0]=random(min,max);
  ledColor[row][col][1]=random(min,max);
  ledColor[row][col][2]=random(min,max);
}

//-------------------------------------------------------------------------
// Setting all led to a common color
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedSetAll(byte color[3]) {
  for(byte col=0;col<COLS;col++) 
    for(byte row=0;row<ROWS;row++) 
      matrixLedSetState(row,col,color);
}

//-------------------------------------------------------------------------
// Setting all leds to specific colors
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedSetAll(byte colors[ROWS][COLS][3]) {
  for(byte col=0;col<COLS;col++) 
    for(byte row=0;row<ROWS;row++) 
      matrixLedSetState(row,col,colors[row][col]);
}

//-------------------------------------------------------------------------
// Setting all leds to random colors
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedSetAllRandom() {
  for(byte col=0;col<COLS;col++) 
    for(byte row=0;row<ROWS;row++) 
      matrixLedSetRandom(row,col);
}

//-------------------------------------------------------------------------
// Setting all leds off
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedSetAllOff() {
  for(byte col=0;col<COLS;col++) 
    for(byte row=0;row<ROWS;row++) 
      matrixLedSetOff(row,col);
}

//-------------------------------------------------------------------------
// Effectively displaying colors
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedRefresh(int interval) {

  for(byte col=0;col<COLS;col++) {
    
    // Setting each color for each row
    for(byte row=0;row<ROWS;row++) {
      matrixRowActivate(row,matrixLedGetColor(row,col));
    }
    
    // Then activating the column...
    matrixColActivate(col);
    
    // ... and deactivating it after a short while
    delayMicroseconds(interval);
    matrixColDeactivate(col);
  }

}

//-------------------------------------------------------------------------
void rgbLedMatrix::matrixLedRefreshDemo(int interval) {

  for(byte col=0;col<COLS;col++) {
    
    // Setting each color for each row
    for(byte row=0;row<ROWS;row++) {
      matrixRowActivate(row,matrixLedGetColor(row,col));
    }
    
    // Then activating the column...
    matrixColActivate(col);
    
    // ... and deactivating it after a short while
    delay(interval);
    matrixColDeactivate(col);
  }

}

//==============================================================================
// Individual led mode
//==============================================================================

//-------------------------------------------------------------------------
void rgbLedMatrix::ledSetState(byte row, byte col, byte color[3]) {

  analogWrite(pinLedRGB[ROW2ADDR(row)][0],color[0]);
  analogWrite(pinLedRGB[ROW2ADDR(row)][1],color[1]);
  analogWrite(pinLedRGB[ROW2ADDR(row)][2],color[2]);
  
  if(color[0]+color[1]+color[2]==0) digitalWrite(pinLedGnd[COL2ADDR(col)],OFF);
  else                              digitalWrite(pinLedGnd[COL2ADDR(col)],ON);
}

//-------------------------------------------------------------------------
void rgbLedMatrix::ledSetRandom(byte row, byte col, byte min, byte max) {
  ledColor[row][col][0]=random(min,max);
  ledColor[row][col][1]=random(min,max);
  ledColor[row][col][2]=random(min,max);
}

//-------------------------------------------------------------------------
void rgbLedMatrix::ledSetOff(byte row, byte col) {
  byte off[]={0,0,0};
  ledSetState(row,col,off);  
}

//-------------------------------------------------------------------------
void rgbLedMatrix::ledTestAll(byte* color) {
  int period=50; // in ms
  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledSetState(row,col,color);
      delay(period);
      ledSetOff(row,col);     
    }
  }
}

//-------------------------------------------------------------------------
// Setting all leds off
//-------------------------------------------------------------------------
void rgbLedMatrix::ledSetAllOff() {
  for(byte col=0;col<COLS;col++) 
    for(byte row=0;row<ROWS;row++) 
      ledSetOff(row,col);
}

//-------------------------------------------------------------------------
void rgbLedMatrix::ledTestMatrix(int period) {

  byte color_red[]={255,0,0};
  byte color_green[]={0,255,0};
  byte color_blue[]={0,0,255};

  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledSetOff(row,col);
    }
  }
  
  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledSetState(row,col,color_red);
      delay(period);
      ledSetOff(row,col);     
      ledSetState(row,col,color_green);
      delay(period);
      ledSetOff(row,col);     
      ledSetState(row,col,color_blue);
      delay(period);
      ledSetOff(row,col);     
    }
  }
  
  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledSetState(row,col,color_red);
    }
  }
  delay(1000);
  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledSetState(row,col,color_green);
    }
  }
  delay(1000);
  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledSetState(row,col,color_blue);
    }
  }
  delay(1000);
  for(int row=0;row<ROWS;row++) {
    for(int col=0;col<COLS;col++) {
      ledSetOff(row,col);     
    }
  }
  
}

//==============================================================================
// Debug helpers... because I had hard time to get all this to work !
//==============================================================================

//-------------------------------------------------------------------------
void rgbLedMatrix::dbgMatrixPrintLedColor() {
  for(byte row=0;row<ROWS;row++) {
    for(byte col=0;col<COLS;col++) {
      Serial.print(ledColor[row][col][0]); Serial.print(";");
      Serial.print(ledColor[row][col][1]); Serial.print(";");
      Serial.print(ledColor[row][col][2]); Serial.print("    ");
    }
    Serial.println();
  }
}

//-------------------------------------------------------------------------
void rgbLedMatrix::dbgMatrixPrintLedLockState() {
  for(byte row=0;row<ROWS;row++) {
    for(byte col=0;col<COLS;col++) {
      Serial.print(ledLockState[row][col]); Serial.print("    ");
    }
    Serial.println();
  }
}

//-------------------------------------------------------------------------
void rgbLedMatrix::dbgPrintColor(byte color[3]) {

  Serial.print("Color: ");
  Serial.print(color[0]);Serial.print(",");
  Serial.print(color[1]);Serial.print(",");
  Serial.print(color[2]);Serial.print(",");
  Serial.println();

}