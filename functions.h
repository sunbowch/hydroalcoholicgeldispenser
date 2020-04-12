/*
// This class is for the Sparfun ultrasonic sensor
// See https://www.sparkfun.com/products/15569 for the datasheet
// Mostly inspired by the code example, but using nonblocking functions

// I have discovered that this sensor need more than 60 mS after power up to
// function. For a battery operation it is long enough to deep-sleep the MCU
// and decrease the power consumption significantly.
*/

extern AccelStepper stepper;                    //uses the same instance of the stepper as declared
                                                // on the main program
class distSensor{
  const byte trigpin,echopin,pwrpin;
  public: distSensor(byte defpin1, byte defpin2, byte defpin3) :
   trigpin(defpin1), echopin (defpin2), pwrpin(defpin3){
  }
  SimpleSleep Sleep;

//------------------------------------------------------------------------------
  void setup(){
    pinMode(trigpin, OUTPUT);
    pinMode(echopin, INPUT);
    pinMode(pwrpin, OUTPUT);
    digitalWrite(pwrpin, LOW);
    digitalWrite(trigpin, LOW);
  }
//------------------------------------------------------------------------------
  void powerOn(){
    digitalWrite(pwrpin, HIGH); // wake up the sensor
    Sleep.deeplyFor(70);        // sleep with pin high as the sensor powers up
  }
//------------------------------------------------------------------------------
  void powerOff(){
    digitalWrite(pwrpin, LOW);
  }

//------------------------------------------------------------------------------
  void ping(){                        //nonblocking function to trig the sensor
    unsigned long deltatime, microtime;

    if (digitalRead(trigpin)==HIGH){
       digitalWrite(trigpin,LOW);
       delayMicroseconds(2);
    }

    digitalWrite(trigpin, HIGH);
    microtime=micros();
//      Serial << ;
    do{
      checkstep();
      deltatime=micros()-microtime;
    }
    while (deltatime<=10);

    digitalWrite(trigpin,LOW);
//    Serial <<"step1 " << deltatime << endl;
  }

//------------------------------------------------------------------------------
  int echo(){                         //nonblocking function to grab the sensor's output
    unsigned long microtime,deltatime;
    long dist;
    ping();

    microtime = micros();
    deltatime = 0;
//    Serial << "step2 ";

    while ((!digitalRead(echopin) && deltatime <= 1000)){
      //delayMicroseconds(10);
      checkstep();
      deltatime=micros()-microtime;
    }

    microtime=micros();
//    Serial << "step3 "<< endl;

    do{
      checkstep();
      deltatime=micros()-microtime;
    }
    while ((digitalRead(echopin) && deltatime <= 1500));

    dist = deltatime/58;

    return dist;
  }

//------------------------------------------------------------------------------
  void checkstep(){                 //if the motor is running, check if a step is scheduled
    if (stepper.distanceToGo()!=0){
      stepper.runSpeed();
    }
  }
};
