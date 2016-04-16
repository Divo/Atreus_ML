
//https://www.pjrc.com/teensy/td_keyboard.html for key codes
//Also keylayouts.h in Teensy libs installed into Arduino env
#include "keylayouts.h"

const int no_rows = 5;
const int no_cols = 10;


const int LAYER_0 = 0xFF;
const int LAYER_1 = 0xFE;
const int LAYER_2 = 0xFD;

const int KEY_FN = 0xFC;

unsigned int layer_0[no_rows][no_cols] = {{KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P},
                                  {KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON},
                                  {KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH},
                                  {KEY_ESC, KEY_TAB, KEY_LEFT_GUI, KEY_LEFT_SHIFT, KEY_BACKSPACE, KEY_SPACE, KEY_FN, KEY_MINUS, KEY_SLASH, KEY_ENTER},
                                  {0, 0, 0, 0, KEY_LEFT_CTRL, KEY_LEFT_ALT, 0, 0, 0, 0}};

unsigned int layer_1[no_rows][no_cols] = {{ASCII_21, ASCII_40, KEY_UP_ARROW, ASCII_7B, ASCII_7D, KEY_PAGE_UP, KEY_7, KEY_8, KEY_9, ASCII_2A},
                                  {KEYPAD_ASTERIX, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW, ASCII_24, KEY_PAGE_DOWN, KEY_4, KEY_5, KEY_6, KEYPAD_PLUS},
                                  {KEY_LEFT_BRACE, KEY_RIGHT_BRACE, ASCII_28, ASCII_29, ASCII_26, KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_BACKSLASH},
                                  {LAYER_2, KEY_INSERT, KEY_LEFT_GUI, KEY_LEFT_SHIFT, KEY_BACKSPACE, KEY_SPACE, KEY_FN, KEY_PERIOD, KEY_0, KEY_ENTER},
                                  {0, 0, 0, 0, KEY_LEFT_CTRL, KEY_LEFT_ALT, 0, 0, 0, 0}};

unsigned int layer_2[no_rows][no_cols] = {{KEY_INSERT, KEY_HOME, KEY_UP_ARROW, KEY_END, KEY_PAGE_UP, KEY_UP_ARROW, KEY_F7, KEY_F8, KEY_F9, KEY_F10},
                                  {KEY_DELETE, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW, KEY_PAGE_DOWN, KEY_DOWN_ARROW, KEY_F4, KEY_F5, KEY_F6, KEY_F11},
                                  {0, 0, 0, 0, 0, 0, KEY_F1, KEY_F2, KEY_F3, KEY_F12},
                                  {0, 0, KEY_LEFT_GUI, KEY_LEFT_SHIFT, KEY_BACKSPACE, ' ', LAYER_0, 0, 0, 0},
                                  {' ', ' ', ' ', ' ', KEY_LEFT_CTRL, KEY_LEFT_ALT, ' ', ' ', ' ', ' '}};



unsigned long debounceDelay = 5;

const int cols[no_cols] = {4, 3, 2, 1, 0, 21, 20, 19, 18, 17};
const int rows[no_rows] = {16, 15, 14, 13, 12};
int lastKeyStates[no_cols][no_rows];
int keyStates[no_cols][no_rows];


unsigned long debounceArray[no_cols][no_rows];

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  Serial.print("Begin");
  // Set col as GND, set to output and low
  //pinMode(col, OUTPUT);

  //initKeyStates();
  //initDebounceArray();
  //initLastKeyStates();

  for(int i = 0; i < no_cols; i++) {
    pinMode(cols[i], OUTPUT);
  }

  // Set row to pullup
  //pinMode(row, INPUT_PULLUP);
  for (int i = 0; i < no_rows; i++) {
    pinMode(rows[i], INPUT_PULLUP);
  }

  //led
  pinMode(11, OUTPUT);
}

void loop(){

  unsigned int  pressedKey = scan_matrix();

  if (pressedKey == 0) {
    return;
  }

  Serial.println(pressedKey);

  if (pressedKey == KEY_LEFT_SHIFT) {

  }else if (pressedKey == KEY_FN) {

  }else if (pressedKey == KEY_LEFT_CTRL) {

  }else if (pressedKey == KEY_LEFT_ALT) {

  }/*else if (pressedKey >= 0x80 && pressedKey <= 0xCD) {
    Keyboard.press(pressedKey);
    Keyboard.release(pressedKey);
  }*/else{
    Keyboard.set_key1(pressedKey);
    Keyboard.send_now();
    Keyboard.set_key1(0);
    Keyboard.send_now();
  }

}


unsigned int scan_matrix() {
  for (int i = 0; i < no_cols; i++) {
    //Activate column
    digitalWrite(cols[i], LOW);

    for (int j = 0; j < no_rows; j++) {
      buttonState = digitalRead(rows[j]);

      //If switch state has changed
      if (buttonState != lastKeyStates[i][j]) {
        //reset the debounce timer
        debounceArray[i][j] = millis();
      }

      //Wait for button to settle for debounceDelay time
      if ((millis() - debounceArray[i][j]) > debounceDelay) {
        //If the button state has changed.
        if (buttonState != keyStates[i][j]) {
          keyStates[i][j] = buttonState;

          //Key has been pressed
          if (buttonState == LOW) {
             Serial.print(i);
             Serial.print(" ");
             Serial.println(j);
             return layer_0[j][i];
             //Keyboard.write(layer_0[j][i]);
          } else {
            //release modifers here?
          }
        }
      }
      lastKeyStates[i][j] = buttonState;
    }
    digitalWrite(cols[i], HIGH);
  }

  return 0;
}

void initKeyStates() {
  for(int i = 0; i < no_cols; i++) {
    for(int j = 0; i < no_rows; j++) {
        keyStates[i][j] = 1;
     }
   }
}

void initLastKeyStates() {
  for(int i = 0; i < no_cols; i++) {
    for(int j = 0; i < no_rows; j++) {
        lastKeyStates[i][j] = 1;
     }
   }
}

void initDebounceArray() {
  for(int i = 0; i < no_cols; i++) {
    for(int j = 0; i < no_rows; j++) {
        debounceArray[i][j] = 1;
     }
   }
}

//  digitalWrite(col, LOW);
//  // read the state of the pushbutton value:
//  buttonState = digitalRead(row);
//
//  Serial.println(buttonState, DEC);
//
//  if (buttonState == HIGH) {
//    digitalWrite(11, LOW);
//  } else {
//    digitalWrite(11, HIGH);
//  }
