/*

Simple software to operate a contactless dispenser.

Gives a single dose of liquid when activated if something (a hand?) is within reach

Pumps some liquid back after distribution to avoid any dripping

Stops immediately if the hand is removed.

Can be used with any arduino board.

Parts needed are only an arduino, a standard stepper driver, a capacitor, a
sparkfun distance sensor, a stepper motor and and optionnal fuse.

The stepper runs a 3D printed peristaltic pump that gives the dose.

*/
#include <SimpleSleep.h>
#include <AccelStepper.h>
#include "functions.h"

#define motorInterfaceType 1
#define dirPin 5
#define stepPin 6
#define enablePin 7
#define trigPin 3
#define echoPin 4
#define onPin 2                       // VCC pin of the sensor
#define stepperspeed 500              // Tune these parameters for your stepper and
#define stepperacceleration 15000     // power supply
#define stepsfordose 600              // tune for the desired quantity
#define backsteps 50                  // tune to avoid dripping
#define minimumdistance 5             // to make the system react
#define maximumdistance 25            // to make the system react

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
distSensor sensor(trigPin,echoPin,onPin);
SimpleSleep Sleep;
int i;

void setup() {

  sensor.setup();
  stepperSetup();
}

void loop() {

  long dist;
  unsigned long runtime = millis();

  sensor.powerOn();
  dist=sensor.echo();

  if (dist>=minimumdistance && dist <= maximumdistance){
    i++;                      //one way to do something only if the hand stays 2 cycles
    switch (i){               //and to distribute only one dose even if the hand stays longer
      case 2:
        squirt();
      break;
    }
  }
  else{
    i=0;
    sensor.powerOff();
  }
  Sleep.deeplyFor(400);         // So that the loop runs only every 0.4 seconds
}

void squirt(){

  long dist = sensor.echo();
  long position;
  stepper.enableOutputs();      //powers on the stepper driver
  stepper.setMaxSpeed(stepperspeed);
  stepper.moveTo(stepsfordose);
  unsigned long time= millis();

  do{
    stepper.run();
    if (millis()-time >= 100){  //check position every 1/10 seconds
      dist = sensor.echo();
      time=millis();
    }
  }
  while (stepper.distanceToGo()!=0 && dist <maximumdistance );

  if (dist>=maximumdistance) i=0;

  position = stepper.currentPosition()-backsteps;
  stepper.runToNewPosition(position);
  stepper.setCurrentPosition(0);
  stepper.disableOutputs();     //powers off the stepper driver
}

void stepperSetup(){

  stepper.setMaxSpeed(stepperspeed);
  stepper.setAcceleration(stepperacceleration);
  stepper.setEnablePin(enablePin);
  stepper.disableOutputs();        //powers off the stepper driver
}
