//Nicholas Hartley, CWID: 11808942, Labs 5&6: Intro to Polling, Port Manipulation, and Interrupt-Driven Systems

/*
  The purpose of this lab was to compare the speed of three different methods for getting values from the ADC.
  The program prompts the user to enter 'a', 'b', or 'c' to pick which kind of conversion is being done.
  The program has a watchdog timeout of 4 seconds that will perform a board resetif the user doesn't enter 
  input within that time frame.
*/

#define inputVoltage A0

#include <avr/wdt.h>//Watchdog timer library

volatile int Reading;//ADC reading for the ADC interrupt

void setup() {
  Serial.begin(9600);//Open a serial session and set the Baud rate to 115200
  cli();
  ADCSRA = B10001111;//Turn on the ADC/Interrupt and set the prescale to the only guaranteed speed
  ADCSRB = B00000000;//Pretty sure this is the default, but just in case analog read decides to get funky
  ADMUX = B01000000;//Set the reference voltage
  sei();
  wdt_enable(WDTO_4S);
  WatchdogSetup();//Calls function to set up the watchdog timer
  wdt_reset();
  Serial.flush();
  Serial.println("\n Board Reset");
  //Welcome message and WDT start for timeout
  Serial.println("\n Select a type of conversion to perform: \n enter ‘a’ for AnalogRead, ‘b’ for polling, or ‘c’ for interrupts\n");
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

ISR(ADC_vect){
  cli();//Disable interrupts
  
  Reading = ADCL | (ADCH << 8);//Get the ADC reading

  ADCSRA &= B10101111;//Clear the interrupt flag and start conversion bit
  
  sei();//Enable interrupts again
}

//Main loop executed for the user
void loop(){
  while(!Serial.available()){}//Wait for serial input

  //Read user input assuming '\n' is terminating character
  updateUI(Serial.readStringUntil('\n'));
  
  //Welcome message and WDT start for timeout
  if(Serial.available()){
  Serial.readString();//Clear user input entered during conversions
  }
  Serial.println("\n Select a type of conversion to perform: \n enter ‘a’ for AnalogRead, ‘b’ for polling, or ‘c’ for interrupts\n");
  wdt_reset();
}

//Function to update the user through the UI
void updateUI(String input){
  wdt_reset();
  float sum;
  if(input.equals("a")){//If the user entered an 'a' start ADC conversions with AnalogRead
    Serial.println("\n Starting a set of conversions using AnalogRead:");
    sum = ADCConversions();//Start ADC conversions using AnalogRead
  }
  else if(input.equals("b")){//If the user entered a 'b' start ADC conversions with Polling
    Serial.println("\n Starting a set of conversions using Polling:");
    sum = PollingADC();//Start ADC conversions using Polling
  }
  else if(input.equals("c")){//If the user entered a 'c' start ADC conversions with Interrupts
    Serial.println("\n Starting a set of conversions using Interrupts:");
    sum = InterruptADC();//Start ADC conversions using Interrupts
  }
  else{
    Serial.println(" Invalid input.");
    wdt_reset();
    return;
  }

  //Print out the average time for any conversion
  Serial.print(sum/30);
  Serial.println(" usecs");
}

//Function to perform the ADC conversions
int ADCConversions(){
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
    sprintf(output, " #%02d:   digital value = %03X     Time = %d usecs",
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
  wdt_reset();
  Serial.print("\n average AnalogRead conversion time = ");
  return sum;
}

//Function to perform the ADC conversions using Polling
int PollingADC(){
  char output[50];//String to print
  unsigned long Time;//Variable to measure the time for each reading
  int Readings[30];//Array to store the times in a smaller data structure
  int Digital; //Variable for the digital voltage value
  
  for(int i = 0; i < 30; i++){
    ADCSRA |= B01000000;
    Time = micros();//Time ADC conversion starts
    while(true){//While ADIF == 0, which signals conversion still happening
      if((ADCSRA | B10111111) != B11111111){//Check the ADCSC bit, since it gets set to 0 when the conversion is finished
        Digital = ADCL | (ADCH << 8);
        Time = micros() - Time;//Difference between when the conversion starts and ends
        break;
      }
    }
    Readings[i] = Time;
    sprintf(output, " #%02d:   digital value = %03X     Time = %03d usecs", i+1, Digital, Readings[i]);
    Serial.println(output);
    ADCSRA &= B11101111;
    delay(500);
    wdt_reset();
  }
  float sum = 0;
  for(int i = 0; i < 30; i++){
    sum += Readings[i];
  }
  wdt_reset();
  Serial.print("\n average Polling conversion time = ");
  return sum;
}

//Function to perform the ADC conversions using Interrupts
int InterruptADC(){
  char output[50];//String to print
  unsigned long Time;//Variable to measure the time for each reading
  int Readings[30];//Array to store the times in a smaller data structure

  for(int i = 0; i < 30; i++){
    Time = micros();//Time ADC conversion starts
    cli();//Gotta add this to be safe, since the ADC is an interrupt
    ADCSRA |= B01000000;
    sei();
    while(true){//The interrupt hasn't fired yet
      if((ADCSRA | B10111111) != B11111111){//If the Start conversion is back to 0 then the interrupt has finished
        break;
      }
    }
    Time = micros() - Time;
    Readings[i] = Time;
    sprintf(output, " #%02d:   digital value = %03X     Time = %03d usecs", i+1, Reading, Readings[i]);
    Serial.println(output);
    delay(500);
    wdt_reset();
  }
  float sum = 0;
  for(int i = 0; i < 30; i++){
    sum += Readings[i];
  }
  wdt_reset();
  Serial.print("\n average Interrupt conversion time = ");
  return sum;
}
