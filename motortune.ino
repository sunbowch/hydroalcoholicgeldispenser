#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Streaming.h>

/**/
#define stepPin 6
#define dirPin 5
#define motorInterfaceType 1
#define enablePin 7
#define minSpeed 100
#define maxSpeed 2000
unsigned long time,time1;

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void sequence(){
  //stepper.enableOutputs();
  stepper.setMaxSpeed(450);
  stepper.runToNewPosition(600);
  stepper.runToNewPosition(400);
  stepper.setCurrentPosition(0);
  //stepper.disableOutputs();
}

void stepperSetup(){
  // Set the maximum speed in steps per second:
  stepper.setMaxSpeed(600);
  stepper.setAcceleration(90000);
  stepper.setEnablePin(enablePin);
  stepper.enableOutputs();
  stepper.moveTo(600);
}

void setup() {

  stepperSetup();
  pinMode(13, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, INPUT);
//  pinMode(5, OUTPUT);
//  digitalWrite(5,HIGH);
  digitalWrite(13,HIGH);
  digitalWrite(A1,LOW);
  Serial.begin(9600);
  time = millis();

}

void loop() {
  long dist;
  int i;
  int spd0,spdc;
  if (millis()-time >=500){
    time=millis();
    int spd=analogRead(A1);
    if (spd != spd0){
      spd0=spd;
      spdc = map(spd0,0,1023,minSpeed,maxSpeed);
      stepper.setMaxSpeed(spdc);
    }
  }
  do{
    stepper.run();
  }
  while (stepper.distanceToGo()!=0);
  if (stepper.currentPosition()==600){
    //stepper.moveTo(0);
    stepper.runToNewPosition(0);
  }
  else{
    //stepper.moveTo(600);
    stepper.runToNewPosition(600);

  }
  Serial << spdc << endl;
}
