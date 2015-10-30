const int LOpt=14;
const int ROpt=15;
const int LMotFor=22;
const int RMotFor=21;
const int LMotRev=23;
const int RMotRev=20;
const int CalButton=2;

int LeftHigh=4095;
int LeftLow=0;
int RightHigh=4095;
int RightLow=0;

void setup() {
	pinMode(LOpt, INPUT);
	pinMode(ROpt, INPUT);
	pinMode(LMotFor, OUTPUT);
	pinMode(RMotFor, OUTPUT);
	pinMode(LMotRev, OUTPUT);
	pinMode(RMotRev, OUTPUT);
	pinMode(CalButton, INPUT);

}

void setRspeed(int s){
	if(s>0){
		analogWrite(RMotFor, s);
		analogWrite(RMotRev, 0);
	}else{
		analogWrite(RMotFor, 0);
		analogWrite(RMotRev,-s);
	}
}
void setLspeed(int s){
	if(s>0){
		analogWrite(LMotFor, s);
		analogWrite(LMotRev, 0);
	}else{
		analogWrite(LMotFor, 0);
		analogWrite(LMotRev,-s);
	}
}

void CalibrationMode(){
	setRspeed(0);
	setLspeed(0);

	delay(20);
	while(digitalRead(CalButton)==0)delay(10);

	LeftHigh=0;
	LeftLow=4095;
	RightHigh=0;
	RightLow=4095;

	int tl,tr;
	for(;;){
		delay(10);

		tl=analogRead(LOpt);
		if(tl<LeftLow)
			LeftLow=tl;
		if(tl>LeftHigh)
			LeftHigh=tl;

		tr=analogRead(ROpt);
		if(tr<RightLow)
			RightLow=tr;
		if(tr>RightHigh)
			RightHigh=tr;

		if(digitalRead(CalButton)==0){
			delay(10);
			while(digitalRead(CalButton)==0)delay(10);
			return;
		}
	}
}

void loop(){
	if(digitalRead(CalButton)==0)CalibrationMode();

	int d,t;

	d=(255*((analogRead(ROpt)-RightLow)))/(RightHigh-RightLow)
	 -(255*((analogRead(LOpt)-LeftLow)))/(LeftHigh-LeftLow);

	if(d>0){
		setRspeed(128+d/2);
		t=128-3*d/2;
		if(t<0)t=t*180/255;
		setLspeed(t);
	}else{
		setLspeed(128-d/2);
		t=128+3*d/2;
		if(t<0)t=t*180/255;
		setRspeed(t);
	}

	delay(5);
}
