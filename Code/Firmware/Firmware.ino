
//https://www.pjrc.com/teensy/td_keyboard.html for key codes
//Also keylayouts.h in Teensy libs installed into Arduino env
#include "keylayouts.h"

const int no_rows = 5;
const int no_cols = 10;


const int LAYER_0 = 0xFF;
const int LAYER_1 = 0xFE;
const int LAYER_2 = 0xFD;

const unsigned int KEY_FN = 0xFC;
const unsigned int SFT_MSK = 0x80;

const int LED_PIN = 11;

unsigned int layer_0[no_rows][no_cols] = {{KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P},
                                  {KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON},
                                  {KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH},
                                  {KEY_ESC, KEY_TAB, MODIFIERKEY_CTRL, MODIFIERKEY_SHIFT, KEY_SPACE, KEY_BACKSPACE, KEY_FN, KEY_MINUS, KEY_QUOTE, MODIFIERKEY_ALT},
                                  {0, 0, 0, 0,MODIFIERKEY_GUI , KEY_ENTER, 0, 0, 0, 0}};

unsigned int layer_1[no_rows][no_cols] = {{(KEY_1 | SFT_MSK) , KEY_UP_ARROW, (KEY_2 | SFT_MSK), (KEY_LEFT_BRACE | SFT_MSK) , (KEY_RIGHT_BRACE | SFT_MSK), KEY_PAGE_UP, KEY_7, KEY_8, KEY_9, (KEY_8 | SFT_MSK) },
                                  {KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW, KEYPAD_ASTERIX, (KEY_4 | SFT_MSK), KEY_PAGE_DOWN, KEY_4, KEY_5, KEY_6, KEYPAD_PLUS},
                                  {KEY_LEFT_BRACE, KEY_RIGHT_BRACE, (KEY_9 | SFT_MSK), (KEY_0 | SFT_MSK), (KEY_7 | SFT_MSK), KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_BACKSLASH},
                                  {LAYER_2, KEY_INSERT, MODIFIERKEY_CTRL, MODIFIERKEY_SHIFT, KEY_SPACE, KEY_BACKSPACE, KEY_FN, KEY_PERIOD, KEY_0, KEY_EQUAL},
                                  {0, 0, 0, 0, MODIFIERKEY_GUI, KEY_ENTER, 0, 0, 0, 0}};

unsigned int layer_2[no_rows][no_cols] = {{KEY_INSERT, KEY_HOME, KEY_UP_ARROW, KEY_END, KEY_PAGE_UP, KEY_UP_ARROW, KEY_F7, KEY_F8, KEY_F9, KEY_F10},
                                  {KEY_DELETE, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW, KEY_PAGE_DOWN, KEY_DOWN_ARROW, KEY_F4, KEY_F5, KEY_F6, KEY_F11},
                                  {0, 0, 0, 0, 0, 0, KEY_F1, KEY_F2, KEY_F3, KEY_F12},
                                  {0, 0, MODIFIERKEY_CTRL, KEY_LEFT_SHIFT, KEY_SPACE, KEY_BACKSPACE, LAYER_0, 0, 0, MODIFIERKEY_ALT},
                                  {0, 0, 0, 0, MODIFIERKEY_GUI, KEY_ENTER, 0, 0, 0, 0}};
                                  

unsigned int (*activeLayer)[no_cols] = layer_0;

int shift_state, ctrl_state, alt_state, gui_state;

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


//This is me half assing a bug where these 4 keys are initaly "pressed" because of lazy code in scan_matrix (the release bit)
  shift_state = 1;
  ctrl_state = 1;
  alt_state = 1;
  gui_state = 1;

  //initKeyStates();
  //initDebounceArray();
  //initLastKeyStates(); //These don't work. Don't care for now

  for(int i = 0; i < no_cols; i++) {
    pinMode(cols[i], OUTPUT);
  }

  for (int i = 0; i < no_rows; i++) {
    pinMode(rows[i], INPUT_PULLUP);
  }

  //led
  pinMode(LED_PIN, OUTPUT);
}

void loop(){

  unsigned int  pressedKey = scan_matrix();
  
  //Serial.println(pressedKey, BIN);

  //Holding FN key activates layer 1
  if (pressedKey == KEY_FN) {
    if (activeLayer == layer_0) {
      activeLayer = layer_1; 
      digitalWrite(LED_PIN, HIGH);
    } else {
      activeLayer = layer_0;
      digitalWrite(LED_PIN, LOW);
    }

  }

  shift_state = updateModifier(MODIFIERKEY_SHIFT, &pressedKey, shift_state);
  
  ctrl_state  = updateModifier(MODIFIERKEY_CTRL, &pressedKey, ctrl_state);

  alt_state   = updateModifier(MODIFIERKEY_ALT, &pressedKey, alt_state);
  
  gui_state   = updateModifier(MODIFIERKEY_GUI, &pressedKey, gui_state);


  Keyboard.set_modifier(shift_state | ctrl_state | alt_state | gui_state);
  
  if ( (pressedKey & SFT_MSK) ) {
    Serial.println("Shift mask detected");
    Keyboard.set_modifier( MODIFIERKEY_SHIFT );
    pressedKey = pressedKey ^ SFT_MSK;
  }

  if (pressedKey != 0) {
    Serial.println(pressedKey);
    Keyboard.set_key1(pressedKey);
  }
  
  Keyboard.send_now();

  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();

}


int updateModifier(unsigned int modifier, unsigned int * pressedKey, int state) {
  if (*pressedKey == modifier) {
    *pressedKey = 0;
    if (state == 0) {
      return modifier;
    }else{
      return 0;
    }
  }

  return state;
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
          unsigned int key = activeLayer[j][i];
          //Key has been pressed
          if (buttonState == LOW) {
             //eeSerial.print(i);
             //Serial.print(" ");
             //Serial.println(j);
             return key;
             //Keyboard.write(layer_0[j][i]);
          } else {
            //release modifers here? If it's a special key being released send it like a normal event.
            if (key == MODIFIERKEY_SHIFT || key == MODIFIERKEY_CTRL || key == MODIFIERKEY_ALT || key == MODIFIERKEY_GUI) {
              return key;
            }
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
