
const int no_rows = 5;
const int no_cols = 10;

//// ~ should be fn
char dvorak_layer_1[no_rows][no_cols] = {{'\'', ',', '.', 'p', 'y', 'f', 'g', 'c', 'r', 'l'},
                       {'a', 'o', 'e', 'u', 'i', 'd', 'h', 't', 'n', 's'},
                       {';', 'q', 'j', 'k', 'x', 'b', 'm', 'w', 'v', 'z'},
                       {KEY_ESC, KEY_TAB, KEY_LEFT_GUI, KEY_LEFT_SHIFT, KEY_BACKSPACE, ' ', '~', '-', '"', 13},
                       {' ', ' ', ' ', ' ', KEY_LEFT_CTRL, KEY_LEFT_ALT, ' ', ' ', ' ', ' '}}; 

unsigned long debounceDelay = 25;

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

          if (buttonState == LOW) {
             Serial.print(i);
             Serial.print(" ");
             Serial.println(j);
             Keyboard.write(dvorak_layer_1[j][i]);
          }
        }
      }
      lastKeyStates[i][j] = buttonState;
    }
    digitalWrite(cols[i], HIGH);
  }

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


