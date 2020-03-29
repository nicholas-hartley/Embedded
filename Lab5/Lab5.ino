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
  Serial.begin(9600);//Open a serial session and set the Baud rate to 9600
  Serial.println("\nBoard Reset\n");
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
}

//Main loop executed for the user
void loop(){
  //Welcome message and WDT start for timeout
  Serial.println(" Select a type of conversion to perform: \n Enter ‘a’ for AnalogRead, ‘b’ for Polling, or ‘c’ for Interrupts");
  delay(20);
  wdt_reset();
  while(!Serial.available()){
  }
  wdt_reset();//Resets the watchdog timer if user input is entered

  delay(5);
  String userInput = Serial.readString();//Stores user input
  userInput.trim();//Trims Whitespace
  
  updateUI(userInput);
  //Resets the watchdog timer after the UI is updated for next input iteration
  wdt_reset();
}

//Function to update the user through the UI
void updateUI(String input){
    wdt_reset();
    if(input.equals("a")){//If the user entered an 'a' to start ADC conversions using AnalogRead
      Serial.println(" Starting a set of conversions using AnalogRead: ");
      ADCConversions();
    }
    else if(input.equals("b")){//If the user entered a 'b' to start ADC conversions using Polling
      Serial.println(" Starting a set of conversions using Polling: ");
      PollingADC();
    }
    else if(input.equals("c")){//If the user entered a 'c' to start ADC conversions using Interrupts
      Serial.println(" Starting a set of conversions using Interrupts: ");
      InterruptADC();
    }
    else{
      Serial.println(" Invalid input.");
    }
    Serial.flush();
    Serial.readString();
    delay(20);
}

//Function to perform the ADC conversions using AnalogRead
void ADCConversions(){
  char output[50];//String to print
  unsigned long Time;//Variable to measure the time for each reading
  int Readings[30];//Array to store the times in a smaller data structure
  int Digital; //Variable for the digital voltage value
  
  wdt_reset();//Reset to be safe
  
  for(int i = 0; i < 30; i++){
    delay(200);
    Time = micros();//Time ADC conversion starts
    Digital = analogRead(inputVoltage);
    Time = micros() - Time;//Difference between when the conversion starts and ends
    Readings[i] = Time;
    sprintf(output, " #%02d:   digital value = %03x     Time = %03d usecs", i+1, Digital, Readings[i]);
    Serial.println(output);
  }
  wdt_reset();//Reset to be safe
  float sum = 0;//Find total conversion time
  for(int i = 0; i < 30; i++){
    sum += Readings[i];
  }
  Serial.print("\n average conversion time = ");//Average the total conversion time
  Serial.println(sum/30);
}

//Function to perform the ADC conversions using Polling
void PollingADC(){
  char output[50];//String to print
  unsigned long Time;//Variable to measure the time for each reading
  int Readings[30];//Array to store the times in a smaller data structure
  int Digital; //Variable for the digital voltage value

  ADCSRA |= B11100000;
  
  for(int i = 0; i < 30; i++){
    delay(200);
    ADCSRA |= B01000000;
    Time = micros();//Time ADC conversion starts
    while(ADCSRA == (ADCSRA & B11101111)){//While ADIF == 0, which signals conversion still happening
    }
    Time = micros() - Time;//Difference between when the conversion starts and ends
    Readings[i] = Time;
    sprintf(output, " #%02d:   digital value = %03x     Time = %03d usecs", i+1, Digital, Readings[i]);
    Serial.println(output);
    ADCSRA &= B11110111;
  }
  float sum = 0;
  for(int i = 0; i < 30; i++){
    sum += Readings[i];
  }
  Serial.print("\n average conversion time = ");
  Serial.println(sum/30);
  
  Serial.println(ADCL);
  Serial.println(ADCH);
  ADCSRA |= B10000000;
  wdt_reset();//Reset to be safe
}

//Function to perform the ADC conversions using Interrupts
void InterruptADC(){
  
}
