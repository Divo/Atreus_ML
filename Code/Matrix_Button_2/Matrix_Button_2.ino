
const int no_rows = 5;
const int no_cols = 10;

const int cols[no_cols] = {4, 3, 2, 1, 0, 21, 20, 19, 18, 17};
const int rows[no_rows] = {16, 15, 14, 13, 12};
int ketStates[no_cols][no_rows];

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  Serial.print("Begin");
  // Set col as GND, set to output and low
  //pinMode(col, OUTPUT); 

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
      if (buttonState == LOW) {
        Serial.print(i);
        Serial.print(" ");
        Serial.println(j);
        digitalWrite(11, HIGH);
      } else {
        digitalWrite(11, LOW);
      }

      buttonState = HIGH;
      //digitalWrite(11, LOW);
    }
     //delay(5);

    digitalWrite(cols[i], HIGH);
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

}
