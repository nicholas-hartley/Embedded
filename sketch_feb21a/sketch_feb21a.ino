//Nicholas Hartley, CWID: 11808942, Lab 4: Intro to analog to digital conversions

/*
  The purpose of this lab was to write an ADC conversion program.
  The program prompts the user to enter 'c' to start a set of conversions. 
  The program has a watchdog timeout of 4 seconds that will perform a board reset
  if the user doesn't enter input within that time frame.
*/

#define inputVoltage A0

#include <avr/wdt.h>//Watchdog timer library

void setup() {
  Serial.begin(9600);//Open a serial session and set the Baud rate to 115200
  wdt_enable(WDTO_4S);
  WatchdogSetup();//Calls function to set up the watchdog timer
  wdt_reset();
  Serial.flush();
  Serial.println("\nBoard Reset");
  //Welcome message and WDT start for timeout
  Serial.println("\nEnter a 'c' to start a set of voltage conversions: ");
  wdt_reset();
}

//Setup loop for initializing the watchdog timer
void WatchdogSetup(){
  cli();//Disable interrupts
  wdt_reset();
  
  //Enter Watchdog Configuration mode
  WDTCSR |= B00011000;//Enable configuration without affecting other bits
  // Set Watchdog settings
  WDTCSR &= B01101000;//Enable the interupt and set time to 4s
  
  sei();//Re-enable interrupts
}

//Interrupt service for the Watchdog timer
ISR(WDT_vect){
  wdt_reset();
  //Enter Watchdog Configuration mode
  WDTCSR |= B00011000;//Enable configuration without affecting other bits
  // Set Watchdog settings
  WDTCSR &= B01111111;//Clear the interrupt flag
}

//Main loop executed for the user
void loop(){
  while(!Serial.available()){}//Wait for serial input

  //Read user input assuming '\n' is terminating character
  updateUI(Serial.readStringUntil('\n'));
  //Resets the watchdog timer after the UI is updated for next input iteration
  wdt_reset();
}

//Function to update the user through the UI
void updateUI(String input){
  wdt_reset();
  if(input.equals("c")){//If the user entered a 'c' start ADC conversions
    Serial.println("\nStarting a set of conversions:");
    ADCConversions();//Start ADC conversions
  }
  else{
    Serial.println("Invalid input.");
  }
  //Welcome message and WDT start for timeout
  if(Serial.available()){
  Serial.readString();//Clear user input entered during conversions
  }
  Serial.println("\nEnter a 'c' to start a set of voltage conversions: ");
  wdt_reset();
}

//Function to perform the ADC conversions
void ADCConversions(){
  char output[50];//String to print
  unsigned long Time;//Variable to measure the time for each reading
  int Readings[30];//Array to store the times in a smaller data structure
  int Digital; //Variable for the digital voltage value
  for(int i = 0; i < 30; i++){
    Time = micros();//Time ADC conversion starts
    Digital = analogRead(inputVoltage);//Read the input voltage
    Time = micros() - Time;//Difference betweene when the conversion starts and ends
    Readings[i] = Time;//Populate the array for current conversion time
    //Format and print output to user
    sprintf(output, "#%02d:   digital value = %03x     Time = %d usecs",
            i+1, Digital, Readings[i]);
    Serial.println(output);
    delay(500);
    wdt_reset();
  }
  wdt_reset();//Reset to be safe
  float sum = 0;
  for(int i = 0; i < 30; i++){
    sum += Readings[i];
  }
  //Print out average time. Sidenote sprintf doesn't have native float support
  Serial.print("\naverage conversion time = ");
  Serial.print(sum/30);
  Serial.println(" usecs");
}
