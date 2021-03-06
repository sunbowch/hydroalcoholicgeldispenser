/*

Simple software to operate a contactless dispenser.

Gives a single dose of liquid when activated if something (a hand?) is within reach

Pumps some liquid back after distribution to avoid any dripping

Stops immediately if the hand is removed.

Can be used with any arduino board.

Parts needed are only an arduino, a standard stepper driver, a capacitor, a
sparkfun distance sensor, a stepper motor and and optionnal fuse.

The stepper runs a 3D printed peristaltic pump that gives the dose.

The dose can be ajusted by a trimmer, value read only during setup to spare power after.

The same trimmer is used to tune the motor speed parameters with motortune.ino

*/
#include <SimpleSleep.h>          //library to deep-sleep the mcu
#include <AccelStepper.h>         //non-blocking stepper control library
#include "functions.h"            //separate file to simplify the code

#define motorInterfaceType 1          // see library definition, 1 = standard stepper driver
#define dirPin 5                      // direction pin of the stepper driver
#define stepPin 6                     // step pin
#define enablePin 7                   // power pin of the driver
#define trigPin 3                     // trigger pin of the sensor, ask for a measure
#define echoPin 4                     // input pin of the sensor data, pulse length proportional to distance
#define onPin 2                       // VCC pin of the sensor
#define lowpotPin A0                  // Output pin next to trimpotPin, fixed to LOW
#define trimpotPin A1                 // Pin for sensing the trimpot value used to tune the quantity.
#define highpotPin A2
#define stepperspeed 700              // Tune these parameters for your stepper and
#define stepperacceleration 18000     // power supply to get smooth operation
#define maxstepsfordose 1000           // tune for the max desired quantity of liquid
#define minstepsfordose 400           // tune for the min desired quantity of liquid
#define backsteps 50                  // tune to avoid dripping
#define minimumdistance 2            // cm to trigger the delivery
#define maximumdistance 15            // cm to trigger the delivery
#define idletime 300

int i;                              // counts the loops when the hand is within reach
int stepsfordose;

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
distSensor sensor(trigPin,echoPin,onPin);
SimpleSleep Sleep;

//------------------------------------------------------------------------------
void setup() {

  sensor.setup();
  stepperSetup();
  stepsfordose=quantitySetup();
}

//------------------------------------------------------------------------------
void loop() {

  int dist;

  sensor.powerOn();           // wakes up the sensor
  dist=sensor.echo();         // get the measured distance

  if (dist>=minimumdistance && dist <= maximumdistance){
    i++;                      //one way to do something only if the hand stays 2 cycles
    switch (i){               //and to distribute only one dose even if the hand stays longer
      case 2:
        squirt();
      break;
    }
  }
  else{
    i=0;                      //cycle count reset only when nothing is within reach
    sensor.powerOff();
  }
  Sleep.deeplyFor(idletime);         // leaves only a watchdog timer on So that the loop runs only every idletime miliseconds
}

//------------------------------------------------------------------------------
void squirt(){

  long dist = sensor.echo();
  long position;
  stepper.enableOutputs();                //powers on the stepper driver
  stepper.setMaxSpeed(stepperspeed);
  stepper.moveTo(stepsfordose);           //define the movement to do
  unsigned long time= millis();

  do{
    stepper.run();              //moves the motor if a step is scheduled.
    if (millis()-time >= 100){  //check position every 1/10 seconds
      dist = sensor.echo();
      time=millis();
    }
  }
  while (stepper.distanceToGo()!=0 && dist < maximumdistance );      //runs until the dose is supplied or hand is removed

  if (dist>=maximumdistance) i=0;                                   //resets the counter if the hand is removed

  position = stepper.currentPosition()-backsteps;
  stepper.runToNewPosition(position);
  stepper.setCurrentPosition(0);                            //resets the stepper position for next run
  stepper.disableOutputs();                                 //powers off the stepper driver
}

//------------------------------------------------------------------------------
void stepperSetup(){

  stepper.setMaxSpeed(stepperspeed);
  stepper.setAcceleration(stepperacceleration);
  stepper.setEnablePin(enablePin);
  stepper.disableOutputs();                                 //powers off the stepper driver
}

//------------------------------------------------------------------------------
int quantitySetup(){                          //reads the potentionmeter value and puts all the pins LOW after that to spare power

  int potvalue;

  pinMode(lowpotPin, OUTPUT);
  pinMode(trimpotPin, INPUT);
  pinMode (highpotPin, OUTPUT);
  digitalWrite(lowpotPin,LOW);
  digitalWrite(highpotPin, HIGH);
  delayMicroseconds(20);
  potvalue = analogRead(trimpotPin);
  digitalWrite(highpotPin,LOW);
  potvalue = map(potvalue,0,1023,minstepsfordose,maxstepsfordose);
  for (i=8;i<=20;i++){
    pinMode(i,OUTPUT);
  }
  return potvalue;
}
