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

#define potpin A6  // analog pin used to connect the potentiometer
#define ampPin A15
#define voltPin A14
#define forcePin A15
#define revPinInterrupt 2 // For teensy 3.1.  Interrupt # is pin #
#define sigOut 10   // signal to esc/servo


#define Imultiplier 
#define Iconst
#define Vmultiplier 452

#define MaxThrott 50


/*
29613 / 4 counts == 16.41v
7403 

452 counts / volt
*/


///////////////////////////////////
// libraries
#include <Servo.h> // Allows control of the esc


///////////////////////////////////// 
Servo myservo;  // create servo object to control a servo




//////////////////////////////////////
// variables used in the program
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the input string is complete
volatile unsigned int revolve = 0; // variable to count pulses
int throttle;    // variable to read the value from the analog pin 
volatile int rpti = 10.0;  // variable to store revolutions per timer interval defined by SCANSPEEED
byte wattgoal = 0; // variable to store the goal wattage.
int rpm = 15; /// variable to store the rpm with some inbuilt debugging info.
int scanspeed = 500;  // scan interval in milliseconds for rpm (computes rpm from agragate revolution count in this interval)
int mode = 0; // mode. 0 is stop, 1 is potentiometer controlled, 2 is computer throttle controlled, 3 is computer wattage controlled
int number = 0;
int updateFrequency = 40;
int analogThrottle;
char letter;

int amp; // for PID loop
int volt;



const char sendData = 'A';
const char stopp = 'X';
const char startt = 'S';
const char setthrott = 'T';
const char setwatt = 'W';
const char setInterval = 'U';
const char setUpdate = 'F';
const char printSettings = 'V';
const char setP = 'P';
const char setI = 'I';
const char setD = 'D';


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
  //analogReadResolution(16); // comment this out if using a chip with a 10 bit adc. for the teensy leave this here.
  myservo.attach(sigOut);  // attaches the servo on pin 9 to the servo object 
  //rpmSetup(scanspeed);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH); // turn on the led to let me know the program is working
  inputString.reserve(10); // reserve space for recieved messages
 
} 
 
///////////////////////////////// 
void loop() {
 
  
  ///////////////////////
  // data input
  ///////////////////////
  
  if (stringComplete) {
    
      //Serial.print("I Recieved: ");
      //Serial.print(inputString);
      
    letter = inputString.charAt(0); // pull out first charecter
    inputString = inputString.substring(1); // cut off first charecter
    number = inputString.toInt(); // convert the rest of the string to an integer.
    
    inputString = ""; // clear the string
    stringComplete = false;  // reset to default value so we don't accidentally trigger
    
     // troubleshooting
     //Serial.print("Letter = "); Serial.print(letter); Serial.print("  Number = "); Serial.println(number);
     
    
       switch(letter){
       
       case sendData:
       ///////////////////////////////// 
       // serial printing stuff
       
        Serial.print(analogThrottle); //"position" of throttle
//        Serial.print(",");
//        Serial.print(analogRead(ampPin)); // output of amperage sensor
//        Serial.print(",");
//        Serial.print(analogRead(voltPin)); // output of voltage divider
//        Serial.print(",");
//        Serial.print(analogRead(forcePin)); // output of force sensor opamp
//        Serial.print(",");
//        Serial.print(rpti * 60 * ( 1000 / scanspeed ));
//        //Serial.print(revolve);
//        Serial.print(",");
//        //Serial.print("000"); // placeholder for later?
//        Serial.println("");       
       break;
         
         
       case stopp: // purposely missspelled. stops motor
       mode = 0;
       myservo.write(0);
       throttle = 0;
       wattgoal = 0;
       //Serial.println("Stopped");
       break;  
     
       case startt:
         //Serial.println("Started");
         if (number >= 0 && number <= 3){
           throttle = 0;
           mode = number;
//Serial.print("mode = "); Serial.println (mode);
         }else{
           //Serial.print ("mode not in range");
         }
       break;
     
       case setthrott:
         if(number >=0 || number <= 100){ throttle = number; 
         //Serial.print("Throttle set to: ");  Serial.println(number);
         }
       break;
       
       case setInterval:
         scanspeed = number;
         //rpmSetup(scanspeed);
       break;
       
       case setUpdate:
         updateFrequency = number;
       break;
       
       default:
         Serial.print("bad command");
       break;
       }
        
        
      number = 0;
  }
 
  
  
///////////////////////
// data output routines
///////////////////////
 
  // servo control stuff
   analogThrottle = map(analogRead(potpin), 0, 65536, 0, 100);     // scale 16 bit adc input to a precentage
  
  if(mode == 1){
    throttle = analogThrottle;
  }
  
  if(mode != 0){ 
      myservo.write(map(throttle, 0, 100, 20, 180)); // sets the throttle from 0-100% 
    } else {
      myservo.write(0); // set throttle to 0 if we don't actively want it on. This one's for safety.
    }
  

  
  //delay(updateFrequency);                           // don't send data quite as fast as you can.
  
  
  
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
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}
