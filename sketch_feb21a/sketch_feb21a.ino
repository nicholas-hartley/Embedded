//Nicholas Hartley, CWID: 11808942, Lab 4: Intro to analog to digital conversions

/*
  The purpose of this lab was to write an ADC conversion program.
  The program prompts the user to enter 'c' to start a set of conversions. 
  The program has a watchdog timeout of 4 seconds that will perform a board reset
  if the user doesn't enter input within that time frame.
*/

#define A0 inputVoltage

#include <avr/wdt.h>//Watchdog timer library

void setup() {
  Serial.begin(115200);//Open a serial session and set the Baud rate to 115200
  WatchdogSetup();//Calls function to set up the watchdog timer
  wdt_reset();
}

//Setup loop for initializing the watchdog timer
void WatchdogSetup(){
  cli();//Disable interrupts
  wdt_reset();
  
  //Enter Watchdog Configuration mode
  WDTCSR |= B00011000;//Enable configuration without affecting other bits
  // Set Watchdog settings
  WDTCSR = B01101000;//Enable the interupt and set time to 4s
  
  sei();//Re-enable interrupts
}

//Interrupt service for the Watchdog timer
ISR(WDT_vect){
  wdt_reset();
  //Enter Watchdog Configuration mode
  WDTCSR |= B00011000;//Enable configuration without affecting other bits
  // Set Watchdog settings
  WDTCSR = B00000000;//Clear the WDT
}

//Main loop executed for the user
void loop(){
  //Welcome message and WDT start for timeout
  Serial.println("Enter a 'c' to start a set of voltage conversions: ");
  wdt_reset();
  while(!Serial.available()){
    //Wait for user input
  }
  wdt_reset();//Resets the watchdog timer if user input is entered
  
  String userInput = Serial.readString();//Stores user input
  userInput.trim();//Trims Whitespace
  
  updateUI(userInput);
  //Resets the watchdog timer after the UI is updated for next input iteration
  wdt_reset();
}

//Function to update the user through the UI
void updateUI(String input){
    wdt_reset();
    if(input.equals("c")){//If the user entered a 'c' start ADC conversions
      Serial.println("Starting a set of conversions:");
      ADCConversions();
    }
    else{
      Serial.println("Invalid input.");
    }
}

//Function to perform the ADC conversions
void ADCConversions(){
  unsigned long Time;
  int Readings[30];//Array to store the values being read
  for(int i = 0; i < 30; i++){
    Time = micros();
    Readings[i] = analogRead(inputVoltage);
    
  }
}
