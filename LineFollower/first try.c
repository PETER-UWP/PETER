//dynamic line guessing

const int pwfull=255;
const int maxspeed=255;
const int ledscale=4095;

const int lLedpin=14;
const int rLedpin=15;
const int lmot=20;
const int rmot=21;

void setup() {
	pinMode(lLedpin, INPUT);
	pinMode(rLedpin, INPUT);
	pinMode(lmot, OUTPUT);
	pinMode(rmot, OUTPUT);
}

void setRspeed(int s){
	analogWrite(rmot, s);
}
void setLspeed(int s){
	analogWrite(lmot, s);
}

int rightLEDread(){
	return analogRead(rLedpin);
}
int leftLEDread(){
	return analogRead(lLedpin);
}

void loop() {
	setRspeed(maxspeed*(1-leftLEDread()*1.0/ledscale));
	setLspeed(maxspeed*(1-rightLEDread()*1.0/ledscale));
	delay(10);
}