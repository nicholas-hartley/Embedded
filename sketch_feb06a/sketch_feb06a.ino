//Nicholas Hartley, CWID: 11808942, Lab 1: Blink an LED

/*
  The purpose of this lab was to write an led blinking program.
  The led's are supposed to initially start off, then start to
  blink whenever the user specifically enters 'g'. To make the
  leds stop blinking the user has to specifically enter 's'. 
  The leds should maintain whatever state they are in until new
  valid user input is entered.
*/
void setup() {
  // initialize digital pins as output
  pinMode(LED_BUILTIN, OUTPUT);//Onboard LED/Pin 13
  pinMode(12, OUTPUT);//External LED/Pin 12
  Serial.begin(9600);//Open a serial session and set the Baud rate to 9600
  Serial.println("Enter a 'g' to start blinking LEDs "
  "and 's' to stop blinking LEDs.");
}

void loop() {
  int state = 0;

  while(1){ //To avoid reinitializing state to 0
    if(state == 1){//If g is entered
      digitalWrite(LED_BUILTIN, HIGH);   // turn the onboard LED on
      digitalWrite(12, LOW);             // turn the external LED off 
      delay(2000);                       // wait for 2 seconds
      digitalWrite(LED_BUILTIN, LOW);    // turn the onboard LED off
      digitalWrite(12, HIGH);            // turn the external LED on
      delay(1000);                       // wait for a second
    }
    else{//If s is entered
      //Set both pins to off
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(12, LOW);
    }
    
    if(Serial.available() != 0){
      String input = Serial.readString();
      if(input.equals("g\n")){
        state = 1;//Set the LED states to blink
        Serial.println("Enter 's' to stop blinking LEDs");
      }
      else if(input.equals("s\n")){
        state = 0;//Set LED states to off
        Serial.println("Enter 'g' to start blinking LEDs");
      }
      else{
        Serial.println("Invalid input. Please enter 'g' to "
        "blink the LEDs, or 's' to Stop.");
      }
    }
  }  
}
