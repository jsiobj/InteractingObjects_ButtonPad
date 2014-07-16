/*
  Library for Sparkfun's Button Pad on Arduino MEGA
  
  For hardware details, see : 
  - https://www.sparkfun.com/products/7835
  - https://www.sparkfun.com/products/8033
  
  For an assembly and connection tutorial see 
  - http://interactingobjects.com/sparkfuns-4x4-button-led-matrix-part-1-soldering/
  - http://interactingobjects.com/sparkfuns-4x4-button-led-matrix-part-2-connecting-to-an-arduino-mega/ 
  - http://interactingobjects.com/sparkfuns-4x4-button-led-matrix-part-3-playing-with-the-pad
  
  Created by Interacting Objects (http://interactingobjects.com), July, 2014.
*/

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
  analogWrite(pinLedRGB[row][0],color[0]);
  analogWrite(pinLedRGB[row][1],color[1]);
  analogWrite(pinLedRGB[row][2],color[2]);
}

//-------------------------------------------------------------------------
// Switching off a row
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixRowDeactivate(byte row) {
  analogWrite(pinLedRGB[row][0],0);
  analogWrite(pinLedRGB[row][1],0);
  analogWrite(pinLedRGB[row][2],0);
}

//-------------------------------------------------------------------------
// Switching on a column
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixColActivate(byte col) {
  digitalWrite(pinLedGnd[col],ON);
}

//-------------------------------------------------------------------------
// Switching off a column
//-------------------------------------------------------------------------
void rgbLedMatrix::matrixColDeactivate(byte col) {
  digitalWrite(pinLedGnd[col],OFF);
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
// Getting surrent locking state
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
void rgbLedMatrix::ledSetState(byte row, byte col, byte color[3]) {
  analogWrite(pinLedRGB[row][0],color[0]);
  analogWrite(pinLedRGB[row][1],color[1]);
  analogWrite(pinLedRGB[row][2],color[2]);
  
  if(color[0]+color[1]+color[2]==0) digitalWrite(pinLedGnd[col],OFF);
  else                              digitalWrite(pinLedGnd[col],ON);
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

//==============================================================================
// Debug helpers... because I had hard time to get all this to work !
//==============================================================================

//-------------------------------------------------------------------------
void rgbLedMatrix::dbgPrintLedColor() {
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
void rgbLedMatrix::dbgPrintLedLockState() {
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