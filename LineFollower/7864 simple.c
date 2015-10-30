/*--------------------------------------+
 |										|
 | Author: Murray Fordyce				|
 | Lasted Edited: 02015-04-08			|
 |										|
 +--------------------------------------+
 | Description:							|
 |										|
 | This Code Implements A Simple Line	|
 | Following Protocol For Testing		|
 | Helper And Debug Functions Along		|
 | With Basic Implementation			|
 |										|
 +--------------------------------------*/

/*** Global Variables ***/


const int debug=0;
const int testing=0;

/*-- Hardware Constants  --*/
 const int Presed=0;
 const int MaxSpeed=255;
 const int MinSpeed=30; //from internal friction
 const int MaxRevSpeed=180;
 const int OptiMaxRead=4095;

/*--    Pin Constants    --*/
 const int LeftOptiPin=14;
 const int RightOptiPin=15;
 const int LeftMotorForward=22;
 const int RightMotorForward=21;
 const int LeftMotorReverse=23;
 const int RightMotorReverse=20;
 const int CalibrationButton=2;
 const int ModeSwitch=0;

/*-- Calibration Globals --*/
 int LeftHigh=OptiMaxRead;
 int LeftLow=0;
 int RightHigh=OptiMaxRead;
 int RightLow=0;

void setup() {
	pinMode(LeftOptiPin, INPUT);
	pinMode(RightOptiPin, INPUT);
	pinMode(LeftMotorForward, OUTPUT);
	pinMode(RightMotorForward, OUTPUT);
	pinMode(LeftMotorReverse, OUTPUT);
	pinMode(RightMotorReverse, OUTPUT);
	pinMode(CalibrationButton, INPUT);
	//pinMode(ModeSwitch, INPUT);

	if(debug)Serial.begin(9600);
	if(debug)Serial.println("Started");
}

//Serial.print(str);
//Serial.println(str);

   //////////////////////
  //    IO Control    //
 // LE: 02015-03-13  //
//////////////////////

void setRspeed(int s){
	if(s>0){
		analogWrite(RightMotorForward, s);
		analogWrite(RightMotorReverse, 0);
	}else{
		analogWrite(RightMotorForward, 0);
		analogWrite(RightMotorReverse,-s);
	}
}//*/ void setRspeed(int s){}
void setLspeed(int s){
	if(s>0){
		analogWrite(LeftMotorForward, s);
		analogWrite(LeftMotorReverse, 0);
	}else{
		analogWrite(LeftMotorForward, 0);
		analogWrite(LeftMotorReverse,-s);
	}
}//*/ void setLspeed(int s){}

int RightOptiRead(){
	return analogRead(RightOptiPin);
}//*/ int RightOptiRead(){}
int LeftOptiRead(){
	return analogRead(LeftOptiPin);
}//*/ int LeftOptiRead(){}

   //////////////////////////
  //   Scale Management   //
 // LE: 02015-03-13      //
//////////////////////////

int MapOptiMot(int s,int type=0){
	if(debug>1)Serial.println("MapOptiMot");
	if(type==0)
		return s*MaxSpeed/OptiMaxRead;
	if(type==1)
		return s*2*MaxSpeed/OptiMaxRead- MaxSpeed;
	return 0;
}
int ClipMot(int s,int f=0){
	if(debug>1)Serial.println("ClipMot");
	if(s>MaxSpeed)
		return MaxSpeed;
	if(s<-MaxSpeed)
		return -MaxSpeed;
	if(f){
		if(0<s<MinSpeed)
			return MinSpeed;
		if(0<-s<MinSpeed)
			return -MinSpeed;
	}
	return s;
}
int CurveDown(int s,int m=OptiMaxRead){
	//if(debug>1)Serial.println("CurveDown");
	if(debug>1)Serial.print(":         ");
	if(debug>1)Serial.print(m);
	if(debug>1)Serial.print(",");
	if(debug>1)Serial.println(s);
	int v=1;
	if(s<0)v=-1;
	return ((s*s*v)/m);
}

int RightScaleOpti(int s,int c=0,int f=0){
	int t=(MaxSpeed*((s-RightLow)))/(RightHigh-RightLow);
	if(c) t=ClipMot(CurveDown(t,RightHigh-RightLow),f);
	return t;
}//*/ int RightScaleOpti(int s){}
int LeftScaleOpti(int s,int c=0,int f=0){
	int t=(MaxSpeed*((s-LeftLow)))/(LeftHigh-LeftLow);
	if(c) t=ClipMot(CurveDown(t,LeftHigh-LeftLow),f);
	return t;
}//*/ int LeftScaleOpti(int s){}

void CalibrationMode(){
	if(debug)Serial.println("CalibrationMode Start");
	setRspeed(0);
	setLspeed(0);

	delay(20);
	//int heldfor=0,maxtime=1000;
	while(digitalRead(CalibrationButton)==Presed){// && heldfor<maxtime){
		//heldfor++;
		delay(10);
	}
	//if(heldfor>=maxtime-10){
		//if(debug)Serial.println("Button held, stopping");
		//for(;;){}
	//}
	//wait for finger to be taken off //*-or for reset time to be hit

	
	LeftHigh=0;
	LeftLow=OptiMaxRead;
	RightHigh=0;
	RightLow=OptiMaxRead;
	
	int TempLeft,TempRight;
	for(;;){
		delay(10);

		TempLeft=LeftOptiRead();
		if(TempLeft<LeftLow){
			LeftLow=TempLeft;
		}
		if(TempLeft>LeftHigh){
			LeftHigh=TempLeft;
		}

		TempRight=RightOptiRead();
		if(TempRight<RightLow){
			RightLow=TempRight;
		}
		if(TempRight>RightHigh){
			RightHigh=TempRight;
		}

		if(digitalRead(CalibrationButton)==Presed){
			if(debug)Serial.println("CalibrationMode Done");
			delay(10);
			while(digitalRead(CalibrationButton)==Presed)delay(10);
			return;
		}
	}
}//*/ void CalibrationMode(){}

   ////////////////////////
  //   Path Following   //
 // LE: 02015-04-08    //
////////////////////////

void ControlStep(int m=1,int c=0){
	if(m==0){	// mode 0 : speed based on light seen
		setRspeed(RightScaleOpti(RightOptiRead(),c));
		setLspeed(LeftScaleOpti(LeftOptiRead(),c));
	}
	if(m==1){	// mode 1 : speed delta
		int r,l,d,t;
		r=RightScaleOpti(RightOptiRead(),c);
		l=LeftScaleOpti(LeftOptiRead(),c);
		d=r-l;
		//map higher to .5 full+.5 delta
		//map lower to .5 full-1.5 delta
		if(d>0){
			setRspeed(MaxSpeed/2+d/2);
			t=MaxSpeed/2-3*d/2;
			if(t<0)t=t*MaxRevSpeed/MaxSpeed;
			setLspeed(t);
		}else{
			setLspeed(MaxSpeed/2-d/2);
			t=MaxSpeed/2+3*d/2;
			if(t<0)t=t*MaxRevSpeed/MaxSpeed;
			setRspeed(t);
		}
	}
	if(m==2){
		//have percent delta change weighting
	}
}//*/ void ControlStep(){}

   /////////////////////
  //    Main Loop    //
 // LE: 02015-04-08 //
/////////////////////

void loop(){
	if(digitalRead(CalibrationButton)==Presed){
		delay(20);
		if(digitalRead(CalibrationButton)==Presed)
			CalibrationMode();
	}
	if(testing==0){//digitalRead(ModeSwitch)==HIGH){
		ControlStep();
	}else{
		ControlStep(2);
	}
	//setRspeed(0);
	//setLspeed(0);
	delay(5);
}//*/ void loop(){}
