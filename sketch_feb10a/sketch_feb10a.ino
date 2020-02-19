//Nicholas Hartley, CWID: 11808942, Lab 2: Switch and 7-Seg Display

//Pre-processor defines for the pin names
#define External_LED 12
#define Switch 11
#define DecimalPoint 2
#define CPin 3
#define DPin 4
#define EPin 5
#define BPin 6
#define APin 7
#define FPin 8
#define GPin 9

/*
  The purpose of this lab was to write a program that
  could take user input from a switch and display a 
  count of the number of state changes that the 
  switch had experienced. The circuit also needed to
  blink LEDs out of phase on a one second interval.
*/
//Initializes all of the pins to either input or output
void setup() {
  // initialize digital pins as output
  pinMode(LED_BUILTIN, OUTPUT);//Onboard LED/Pin 13
  pinMode(External_LED, OUTPUT);//External LED/Pin 12
  pinMode(Switch, INPUT_PULLUP);//Switch/Enabling Input and Pull-up for Pin 11
  //Seven Segment Display Pins
  pinMode(DecimalPoint, OUTPUT);//Decimal Point
  pinMode(CPin, OUTPUT);//Segment "C"
  pinMode(DPin, OUTPUT);//Segment "D"
  pinMode(EPin, OUTPUT);//Segment "E"
  pinMode(BPin, OUTPUT);//Segment "B"
  pinMode(APin, OUTPUT);//Segment "A"
  pinMode(FPin, OUTPUT);//Segment "F"
  pinMode(GPin, OUTPUT);//Segment "G"
  Serial.begin(9600);//Open a serial session and set the Baud rate to 9600
  Serial.println("Toggle switch \"on\" to make LEDs blink. "
  "To make the LEDs stop toggle switch \"off\".\nThe number of state changes"
  " will be recorded in a continuous decimal count and \na hexadecimal count"
  " will match the seven segment LED display.\n");
}
//Blinks LEDs out of phase with each other for 1 second each
void loop() {
  int count = 0; //How many times the state has been changed
  int state = digitalRead(Switch); //Current state of the input pin
  long last = 0; //Last time a check for the state was performed in ms
  long blinkT = -2000; //Last time a blink cycle was started

  //Changes the Seven Segment display and the serial output
  updateUI(count);

  while(1){ //To avoid reinitializing values

    //Here to prevent debounce and Switch flickering
    if((millis() - last) > 50){//Check to see if 50 ms has passed
      if(state != digitalRead(Switch)){
        count = count + 1; //Update count of state changes
        state = digitalRead(Switch); //Update the current state of the switch
        updateUI(count);
      }
      last = millis(); //Update the last time pin was checked
    }
    //If the LEDs have completed a full cycle and the switch is "ON"
    if(state == LOW && (millis() - blinkT) > 2000){
      blinkT = millis();//Set the last blink time to current time
      digitalWrite(LED_BUILTIN, HIGH);//Turn onboard LED on
      digitalWrite(External_LED, LOW);//Turn External LED off
    }
    //If the LEDs still need to cycle
    else if(digitalRead(LED_BUILTIN) == HIGH && (millis() - blinkT) > 1000){
      digitalWrite(LED_BUILTIN, LOW);//Turn onboard LED off
      digitalWrite(External_LED, HIGH);//Turn External LED on
    }
    else{//If the LEDs have cycled and the switch is "off"
      if((millis()-blinkT) > 2000){
        digitalWrite(External_LED, LOW);//Turn External LED off
      }
    }
  }  
}
//Changes the Seven segment display and prints to the serial monitor
void updateUI(int count){
  //Print statements for serial output
  Serial.print("Count = (decimal) ");
  Serial.print(count);
  Serial.print(" (hex) ");
  //Switch statement to deal with the different 7-Seg states
  switch(count%16){//Modulus since the count rolls over
    case 0:// 0
      digitalWrite(DecimalPoint, HIGH);//Decimal Point off
      digitalWrite(APin, LOW);//Segment "A" on
      digitalWrite(BPin, LOW);//Segment "B" on
      digitalWrite(CPin, LOW);//Segment "C" on
      digitalWrite(DPin, LOW);//Segment "D" on
      digitalWrite(EPin, LOW);//Segment "E" on
      digitalWrite(FPin, LOW);//Segment "F" on
      digitalWrite(GPin, HIGH);//Segment "G" off
      Serial.println("0");
      break;
    case 1:// 1
      digitalWrite(APin, HIGH);//Segment "A" off
      digitalWrite(DPin, HIGH);//Segment "D" off
      digitalWrite(EPin, HIGH);//Segment "E" of
      digitalWrite(FPin, HIGH);//Segment "F" off
      Serial.println("1");
      break;
    case 2:// 2
      digitalWrite(APin, LOW);//Segment "A" on
      digitalWrite(CPin, HIGH);//Segment "C" off
      digitalWrite(DPin, LOW);//Segment "D" on
      digitalWrite(EPin, LOW);//Segment "E" on
      digitalWrite(GPin, LOW);//Segment "G" on
      Serial.println("2");
      break;
    case 3:// 3
      digitalWrite(CPin, LOW);//Segment "C" on
      digitalWrite(EPin, HIGH);//Segment "E" off
      Serial.println("3");
      break;
    case 4:// 4
      digitalWrite(APin, HIGH);//Segment "A" off
      digitalWrite(DPin, HIGH);//Segment "D" off
      digitalWrite(FPin, LOW);//Segment "F" on
      Serial.println("4");
      break;
    case 5:// 5
      digitalWrite(APin, LOW);//Segment "A" on
      digitalWrite(BPin, HIGH);//Segment "B" off
      digitalWrite(DPin, LOW);//Segment "D" on
      Serial.println("5");
      break;
    case 6:// 6
      digitalWrite(DecimalPoint, LOW);//Decimal Point on
      digitalWrite(EPin, LOW);//Segment "E" on
      Serial.println("6");
      break;
    case 7:// 7
      digitalWrite(DecimalPoint, HIGH);//Decimal Point off
      digitalWrite(BPin, LOW);//Segment "B" on
      digitalWrite(DPin, HIGH);//Segment "D" off
      digitalWrite(EPin, HIGH);//Segment "E" off
      digitalWrite(FPin, HIGH);//Segment "F" off
      digitalWrite(GPin, HIGH);//Segment "G" off
      Serial.println("7");
      break;
    case 8:// 8
      digitalWrite(DPin, LOW);//Segment "D" on
      digitalWrite(EPin, LOW);//Segment "E" on
      digitalWrite(FPin, LOW);//Segment "F" on
      digitalWrite(GPin, LOW);//Segment "G" on
      Serial.println("8");
      break;
    case 9:// 9
      digitalWrite(EPin, HIGH);//Segment "E" off
      Serial.println("9");
      break;
    case 10:// A
      digitalWrite(DPin, HIGH);//Segment "D" off
      digitalWrite(EPin, LOW);//Segment "E" on
      Serial.println("A");
      break;
    case 11:// b
      digitalWrite(APin, HIGH);//Segment "A" off
      digitalWrite(BPin, HIGH);//Segment "B" off
      digitalWrite(DPin, LOW);//Segment "D" on
      Serial.println("B");
      break;
    case 12:// C
      digitalWrite(APin, LOW);//Segment "A" on
      digitalWrite(CPin, HIGH);//Segment "C" off
      digitalWrite(GPin, HIGH);//Segment "G" off
      Serial.println("C");
      break;
    case 13:// d
      digitalWrite(APin, HIGH);//Segment "A" off
      digitalWrite(BPin, LOW);//Segment "B" on
      digitalWrite(CPin, LOW);//Segment "C" on
      digitalWrite(FPin, HIGH);//Segment "F" oFF
      digitalWrite(GPin, LOW);//Segment "G" on
      Serial.println("D");
      break;
    case 14:// E
      digitalWrite(APin, LOW);//Segment "A" on
      digitalWrite(BPin, HIGH);//Segment "B" off
      digitalWrite(CPin, HIGH);//Segment "C" off
      digitalWrite(FPin, LOW);//Segment "F" on
      Serial.println("E");
      break;
    case 15:// F
      digitalWrite(DPin, HIGH);//Segment "D" off
      Serial.println("F");
      break;
  }
}
