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
#define LEFT_WEEL_PIN 10   // signal to esc/servo left
#define RIGHT_WEEL_PIN 11   // signal to esc/servo right
#define Imultiplier
#define Iconst
#define Vmultiplier 452
#define MaxSpeed 50
//#define SERIALECHO
#define ZEROSPEED 95   // zero is between 92 and 98

	///////////////
 // libraries //
///////////////
#include <Servo.h> // Allows control of the esc

	/////////////////////
 // Global Varibles //
/////////////////////

// create Left and Right servo
Servo myservoL;
Servo myservoR;

// variables used in the program
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the input string is complete
int throttle;    // variable to read the value from the analog pin

#define DEBUG 0

int commandArg = 0;
char letter;

//WatchDog parameters
#define WATCHDOG_TIME 200
int watchdog = WATCHDOG_TIME;  // ms until watchdog triggers. is reset each time serial is recieved.

//Controll Codes
const char STOP_CHAR = 'X';
//const char START_CHAR = 'S';
const char LEFT_CHAR = 'L';
const char RIGHT_CHAR = 'R';
//const char DATA_CHAR = 'A';
//const char WATT_CHAR = 'W';
//const char SET_INTERVAL_CHAR = 'U';
//const char SET_UPDATE_CHAR = 'F';
//const char PRINT_SETTINGS_CHAR = 'V';

void setup(){
	Serial.begin(9600);
	//analogReadResolution(16); // commented as using a chip with a 10 bit
	myservoL.attach(LEFT_WEEL_PIN);  // attaches the servo on pin 9 to the servo object adc. for the teensy leave this here.
	myservoR.attach(RIGHT_WEEL_PIN);  // attaches the servo on pin 9 to the servo object

	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH); // turn on the led to let me know the program is working
	inputString.reserve(10); // reserve space for recieved messages

	delay(4000);

	if(DEBUG)
		Serial.print("Setup Complete.");
}

void parseInput(){
	if(DEBUG){
		Serial.print("I Recieved: ");
		Serial.print(inputString);
	}

	letter = inputString.charAt(0);
	inputString = inputString.substring(1);
	commandArg = inputString.toInt();

	inputString = "";
	stringComplete = false;

	if(DEBUG){
		Serial.print("Letter = ");
		Serial.print(letter);
		Serial.print("  ComandArg = ");
		Serial.println(commandArg);
	}

	switch(letter){

	case STOP_CHAR:
		watchdog = 0;
		if(DEBUG)
			Serial.println("Stopped.");
		break;

	case LEFT_CHAR:
		myservoL.write(commandArg);
		watchdog = WATCHDOG_TIME;
		break;

	case RIGHT_CHAR:
		myservoR.write(commandArg);
		watchdog = WATCHDOG_TIME;
		break;

		//case START_CHAR:
		//if(DEBUG)
		//	Serial.println("Started");
		//if (commandArg >= 0 && commandArg <= 3){
		//	throttle = 0;
		//	mode = commandArg;
		//	if(debug)
		//		Serial.print("mode = "); Serial.println (mode);
		//}else
		//	if(DEBUG)
		//		Serial.print ("mode not in range");
		// break;

	default:
		Serial.print("Unknown Command.");
		break;
	}

	commandArg = 0;
}

void readSerialData(){
	while (Serial.available()){
		char inChar = (char)Serial.read();
		if(DEBUG>2)
			Serial.print(inChar);
		if (inChar == 10||inChar == 13)
			stringComplete = true;
		else
			inputString += inChar;
	}
}

void loop(){
	readSerialData();

	if(DEBUG>1){
		Serial.print("string complete: ");
		Serial.println(stringComplete);
	}

	if (stringComplete)
		parseInput(inputString);

	if(watchdog <= 0){
		myservoL.write(ZEROSPEED);
		myservoR.write(ZEROSPEED);
	}

	watchdog--;
	delay(10);
}
