/*=====================================================================================

  This is an example which makes uses of InteractingObjects_RGBLedMatrix library

  For an assembly and connection tutorial see 
  - http://interactingobjects.com/sparkfuns-4x4-button-led-matrix-part-1-soldering/
  - http://interactingobjects.com/sparkfuns-4x4-button-led-matrix-part-2-connecting-to-an-arduino-mega/ 
  - http://interactingobjects.com/sparkfuns-4x4-button-led-matrix-part-3-playing-with-the-pad
  
  WARNING : As of now, in the tutorial, I use PWM pins 2 to 13. But I swapped pins
            4 a 13 respectively with pin 44 & 45.
        
            Q:Why didn't I used pin 4 & 13 and 44 & 45 instead ? 
            
            A:Because I needed to increase PWM frequency to make the matrix works.
              To do this, you need to change Timers config. Pins 4 and 13 use timer 0.
              But... as it is used by Arduino's core timing functions,  you don't want 
              to mess with it. Do you ?

  ======================================================================================*/

#include <InteractingObjects_ButtonPad.h>
#include <Keypad.h>

// Define pin used for buttons
byte pinBtn[ROWS]       = {22,23,24,25}; // Buttons rows
byte pinBtnGnd[COLS]    = {26,27,28,29}; // Buttons cols

// Define pins used for RGB leds
byte customPinLedRGB[ROWS][3] = {{2,3,44},{5,6,7},{8,9,10},{11,12,45}}; // RGB pins
byte customPinLedGnd[COLS]    = {30,31,32,33};                          // GND pins

// "Standard" colors                        
#define COLOR_RED     0
#define COLOR_GREEN   1
#define COLOR_BLUE    2
#define COLOR_YELLOW  3
#define COLOR_MAGENTA 4
#define COLOR_CYAN    5
#define COLOR_WHITE   6
#define COLOR_OFF     7

byte standardColors[][3] =  { 
  {255,0  ,0  }, 
  {0  ,255,0  }, 
  {0  ,0  ,255},
  {255,255,0  },
  {255,0  ,255},
  {0  ,255,255},
  {255,255,255},
  {0  ,0  ,0  }
};

// For keypad
char hexaKeys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};

// Creating keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), pinBtn, pinBtnGnd, ROWS, COLS); 

// Creating led matrix
rgbLedMatrix ledMatrix = rgbLedMatrix(customPinLedRGB,customPinLedGnd,ROWS,COLS);

//-------------------------------------------------------------------------
// What to do when a key is pressed
//-------------------------------------------------------------------------
void onKeyPressed(char keyCode) {
  byte row=keyCode/4;
  byte col=keyCode%4;
  
  ledMatrix.matrixLedSetRandom(row,col); // Switch the led on with a randow color or changing the color if on  
  ledMatrix.matrixLedUnlock(row,col);    // ... and "unlock" the led, i.e. when button is release, switch it off
}

//-------------------------------------------------------------------------
// What to do when a key is held
//-------------------------------------------------------------------------
void onKeyHold(int keyCode) {
  byte row=keyCode/4;
  byte col=keyCode%4;

  // When a button is held, "locking" the color (i.e. don't switch led off when key is released
  if(ledMatrix.matrixLedGetState(row,col)==ON) ledMatrix.matrixLedLock(row,col);  
}

//-------------------------------------------------------------------------
// What to do when a key is released
//-------------------------------------------------------------------------
void onKeyReleased(int keyCode) {
  byte row=keyCode/4;
  byte col=keyCode%4;

  // When the key is released, if not locked (i.e. key was not hold) and led is ON, switching it OFF
  if(ledMatrix.matrixLedGetLockState(row,col)==OFF && ledMatrix.matrixLedGetState(row,col)==ON)
    ledMatrix.matrixLedSetState(row,col,standardColors[COLOR_OFF]);

}

//-------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  setPWMFreq();     // Setting a higher frequency for PWM
  
  // Just testing leds
  ledMatrix.ledTestAll(standardColors[COLOR_RED]);
  ledMatrix.ledTestAll(standardColors[COLOR_GREEN]);
  ledMatrix.ledTestAll(standardColors[COLOR_BLUE]);
  ledMatrix.ledTestAll(standardColors[COLOR_WHITE]);
  
  // Switching on all leds with random colors
  ledMatrix.matrixLedSetAllRandom();

}

//-------------------------------------------------------------------------
void loop() {
  
  // At least one key state changed
  if (customKeypad.getKeys()){

    for (int i=0; i<LIST_MAX; i++)  {  // Go through 10 keys max
        if ( customKeypad.key[i].stateChanged ) { // Check if state changed
          switch (customKeypad.key[i].kstate) {         // Report active key state : IDLE, PRESSED, HOLD, or RELEASED

                case PRESSED:
                  onKeyPressed(customKeypad.key[i].kcode);
                  break;
                
                case HOLD:
                  onKeyHold(customKeypad.key[i].kcode);
                  break;
                
                case RELEASED:
                  onKeyReleased(customKeypad.key[i].kcode);
                  break;
                
                case IDLE:
                  // do nothing !
                  break;
          }
        }
    }
  }

  // refreshing the led matrix (all we did up until now is updating the status "logically")
  ledMatrix.matrixLedRefresh(500);
}
