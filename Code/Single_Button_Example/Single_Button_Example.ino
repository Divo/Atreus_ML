
const int row = 13;     // the number of the pushbutton pin
const int col =  21;      // the number of the LED pin


// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  Serial.print("Begin");
  // Set col as GND, set to output and low
  pinMode(col, OUTPUT);  

  // Set row to pullup
  pinMode(row, INPUT_PULLUP);   

  //led
  pinMode(11, OUTPUT);
}

void loop(){
  digitalWrite(col, LOW);  
  // read the state of the pushbutton value:
  buttonState = digitalRead(row);

  Serial.println(buttonState, DEC);

  if (buttonState == HIGH) {
    digitalWrite(11, LOW);
  } else {
    digitalWrite(11, HIGH);
  }

}
