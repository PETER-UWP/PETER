/*Program created by marshall scholz as a rc airplane motor preformance test stand controller / datalogger on the UW platteville SAE Aero team.

TODO:
inplement gcode esque control of motor
inplement pid current control
implement estop & start / stop button
make timing more flexable
add output for enabled warning light

Codes to implement
- start (enable throttle)
- stop  (disable throttle / motor)
- set throttle (and scale to precentage)
- set target current
- set target wattage
- change update interval
- change rpm calculation interval
- update pid loop constants - saved across powerups
- readout pid loop constants over serial
- zero inputs? (current, thrust?)


*/


// configuration of pins/etc

//#define potpin A6  // analog pin used to connect the potentiometer
//#define ampPin A15
//#define voltPin A14
//#define forcePin A15
//#define revPinInterrupt 2 // For teensy 3.1.  Interrupt # is pin #
#define sigOutL 10   // signal to esc/servo left
#define sigOutR 11   // signal to esc/servo right


#define Imultiplier 
#define Iconst
#define Vmultiplier 452

#define MaxSpeed 50

//#define SERIALECHO


#define ZEROSPEED 95   // zero is between 92 and 98



///////////////////////////////////
// libraries
#include <Servo.h> // Allows control of the esc


///////////////////////////////////// 
Servo myservoL;  // create servo object to control a servo
Servo myservoR;  // create servo object to control a servo




//////////////////////////////////////
// variables used in the program
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the input string is complete
int throttle;    // variable to read the value from the analog pin 

int mode = 0; // mode. 0 is stop, 1 is potentiometer controlled, 2 is computer throttle controlled, 3 is computer wattage controlled
int number = 0;
int updateFrequency = 40;
int analogThrottle;
char letter;

int amp; // for PID loop
int volt;

#define WATCHSET 200

int watchdog = WATCHSET;  // ms until watchdog triggers. is reset each time serial is recieved.






const char stopp = 'X';
const char startt = 'S';
const char setSpeedL = 'L';
const char setSpeedR = 'R';

//const char sendData = 'A';
//const char setwatt = 'W';
//const char setInterval = 'U';
//const char setUpdate = 'F';
//const char printSettings = 'V';



// parse the letter part of the gcode. commands follow
      // [S{mode}]start (enable motor) will start motor if throttle is zero. 
      //    modes: 
      //    - 0 is stop
      //    - 1 is potentiometer controlled
      //    - 2 is computer throttle controlled
      //    - 3 is wattage controlled
      //
      // [X000]stop (disable motor) will set throttle to zero.
      // [T{value}] set throttle
      // [W{value}] set wattage goal
      // [U{value}] update interval in milliseconds. default == 500ms
      //
      // [V{value}] print out current variables. (pid, wattage goal, update interval, etc)
      //
      // pid stuff. not stored across power cyles.
      // [P{value}]
      // [I{value}]
      // [D{value}]


 
 
///////////////////////////////// 
void setup() 
{ 
  
  
  Serial.begin(9600);
  //analogReadResolution(16); // comment this out if using a chip with a 10 bit 
  myservoL.attach(sigOutL);  // attaches the servo on pin 9 to the servo object adc. for the teensy leave this here.
  myservoR.attach(sigOutR);  // attaches the servo on pin 9 to the servo object 
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH); // turn on the led to let me know the program is working
  inputString.reserve(10); // reserve space for recieved messages
  
  delay(4000);
  
  Serial.print("ALL set!");
 
} 
 
///////////////////////////////// 
void loop() {
  serialEvent();
  
  ///////////////////////
  // data input
  ///////////////////////
  
//  Serial.print("string complete: ");
//  Serial.println(stringComplete);
  
  if (stringComplete) {
    
//     Serial.print("I Recieved: ");
//     Serial.print(inputString);
      
    letter = inputString.charAt(0); // pull out first charecter
    inputString = inputString.substring(1); // cut off first charecter
    number = inputString.toInt(); // convert the rest of the string to an integer.
    
    inputString = ""; // clear the string
    stringComplete = false;  // reset to default value so we don't accidentally trigger
    
     // troubleshooting
     //Serial.print("Letter = "); Serial.print(letter); Serial.print("  Number = "); Serial.println(number);
     
    
       switch(letter){   
            
          case stopp: // purposely missspelled. stops motor
             watchdog = 0;
             Serial.println("Stopped");
          break;  
          
          case setSpeedL: // purposely missspelled. stops motor
             myservoL.write(number);
             watchdog = WATCHSET;
          break;  
          
          case setSpeedR: // purposely missspelled. stops motor
             myservoR.write(number);
             watchdog = WATCHSET;
          break;  
        
   //       case startt:
   //         //Serial.println("Started");
   //         if (number >= 0 && number <= 3){
   //           throttle = 0;
   //           mode = number;
   //        //Serial.print("mode = "); Serial.println (mode);
   //         }else{
   //           //Serial.print ("mode not in range");
   //         }
   //       break;
        
          default:
            Serial.print("bad command");
       break;
       }
        
        
      number = 0;
  }
 
  
  
///////////////////////
// data output routines
///////////////////////

  if(watchdog <= 0){ 
      myservoL.write(ZEROSPEED);
      myservoR.write(ZEROSPEED);   
  }
  
  watchdog--;
  delay(10);       
} 





/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    
#ifdef SERIALECHO 
Serial.print(inChar);  //echo serial commands if enabled
#endif
    
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}
