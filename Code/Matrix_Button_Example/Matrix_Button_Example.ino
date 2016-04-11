


const int row = 0;     // the number of the pushbutton pin
const int col =  2;      // the number of the LED pin

const int ARRAY_SIZE = 2;
const int rows[ARRAY_SIZE] = {0,1};
const int cols[ARRAY_SIZE] = {2,3};
const String chars[ARRAY_SIZE][ARRAY_SIZE] = {{'a','c'}, {'b','d'}};
int keyStates[ARRAY_SIZE][ARRAY_SIZE];// = {{1, 1}, {1, 1}};


// variables will change:
int buttonState = 1;         // variable for reading the pushbutton status
void setup() {
  Serial.begin(9600);
  Serial.print("Begin");
  Keyboard.begin();
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 2; j++) {
      keyStates[i][j] = 1;
    }
  }
  // Set col as GND, set to output and low
  //pinMode(col, OUTPUT);
  for(int i =0; i <  ARRAY_SIZE; i++) {
    pinMode(cols[i], OUTPUT);
  }

  // Set row to pullup
  //pinMode(row, INPUT_PULLUP);   
  for(int i=0; i < ARRAY_SIZE; i++) {
    pinMode(rows[i], INPUT_PULLUP);
  }

  //led
  pinMode(11, OUTPUT);
}

void loop() {
  //x and y becuase this toolchain is stopid
  for(int x = 0; x < ARRAY_SIZE; x++){
    //Activate column
    digitalWrite(cols[x], LOW);
    
    for(int y = 0; y < ARRAY_SIZE; y++) {
      buttonState = digitalRead(rows[y]);
      if (buttonState == LOW) {
        digitalWrite(11, HIGH);
        if (keyStates[x][y] == HIGH) {
          keyStates[x][y] = LOW;
          Keyboard.print(chars[x][y]);
          delay(30); //Need to debounce properly
        }
      } else if((buttonState == HIGH) && keyStates[x][y] == LOW) {
        keyStates[x][y] = 1;
        delay(30);
      }
      
      buttonState = HIGH;
      digitalWrite(11, LOW);
    }

    //De-activate column
    digitalWrite(cols[x], HIGH);   
  }
}


