//Nicholas Hartley, CWID: 11808942, Lab 3: Watchdog timer reaction Time

#include <avr/wdt.h>//Watchdog timer library

void setup() {
  DDRC = B11111111;//Set all 8 pins on port C as output
  PORTC = B11111110;//Setting the Seven segment display to initial condition
  Serial.begin(9600);//Open a serial session and set the Baud rate to 9600
  wdt_enable(WDTO_4S);
  WatchdogSetup();//Calls function to set up the watchdog timer
  wdt_reset();
  Serial.println("You will have 4 Seconds to enter an integer in the range 0-15."
  "Your reaction time and valid input will be\ndisplayed on the seven segment "
  "display,as well as on the serial monitor.");
}

//Setup loop for initializing the watchdog timer
void WatchdogSetup(){
  cli();
  wdt_reset();
  
  //Enter Watchdog Configuration mode
  WDTCSR |= B00011000;//Enable configuration without affecting other bits
  // Set Watchdog settings
  WDTCSR |= B01000000;//Enable the interupt
  
  sei();
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
  long Time = millis();//Get the time after the user is prompted
  
  while(!Serial.available()){//Wait for user input
    if(WDTCSR == B00000000){//If the interrupt is not enabled signaling a reset
      resetSequence();
    }
  }
  
  Time = millis() - Time;//Find the reaction time
  wdt_reset();//Resets the watchdog timer if user input is entered
  
  String userInput = Serial.readString();//Stores user input
  int number = userInput.toInt();//Attempts to convert input to an integer
  userInput.trim();//Trims Whitespace
  if(number == 0 && !(userInput.equals("0"))){
    //Since toInt returns a 0 upon failure this is to catch invalid input
    number = 16;//Sets value to 16 to trigger default case for invalid input
  }
  
  //Passes user input as an integer and reaction time to the UI handler
  updateUI(number, Time);
  //Resets the watchdog timer after the UI is updated for next input iteration
  wdt_reset();
}

//Function to update the user through the UI
void updateUI(int input, long Time){
    switch(input){//Switch statement to display Hex number to UI
      case 0:// 0
        PORTC = B10000001;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = 0");
        break;
      case 1:// 1
        PORTC = B11001111;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = 1");
        break;
      case 2:// 2
        PORTC = B10010010;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = 2");
        break;
      case 3:// 3
        PORTC = B10000110;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = 3");
        break;
      case 4:// 4
        PORTC = B11001100;
        Serial.print("Reaction Time = ");
        Serial.print(((Time - (Time%1000))/1000)/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = 4");
        break;
      case 5:// 5
        PORTC = B10100100;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = 5");
        break;
      case 6:// 6
        PORTC = B00100000;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = 6");
        break;
      case 7:// 7
        PORTC = B10001111;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = 7");
        break;
      case 8:// 8
        PORTC = B10000000;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = 8");
        break;
      case 9:// 9
        PORTC = B10000100;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = 9");
        break;
      case 10:// A
        PORTC = B10001000;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = A");
        break;
      case 11:// b
        PORTC = B11100000;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = B");
        break;
      case 12:// C
        PORTC = B10110001;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = C");
        break;
      case 13:// d
        PORTC = B11000010;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = D");
        break;
      case 14:// E
        PORTC = B10110000;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = E");
        break;
      case 15:// F
        PORTC = B10111000;
        Serial.print("Reaction Time = ");
        Serial.print((Time - (Time%1000))/1000);//Number of seconds only
        Serial.print(".");
        Serial.println(Time%1000);//Number of milliseconds only
        Serial.println("User input = F");
        break;
      default:// Garbage input
        PORTC = B10110110;
        Serial.println("Invalid input");
        //No reaction time displayed for invalid input
        break;
    }
    Serial.println("\nPlease enter an integer between 0 and 15: ");
}

//Function to manage final operations before a reset
void resetSequence(){
  PORTC = B11111111;//Clear Seven Seg while processing
  PORTC = B01111111;//Turn Decimal Point on
  for(int i = 0; i < 10; i++){
    delay(500);
    PORTC ^= B10000000;
  }
  
 
  asm volatile ("  jmp 0");//Return code to instruction 0 for a soft reset
}
