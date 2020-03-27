//
// This class is for the Sparfun ultrasonic sensor
// See https://www.sparkfun.com/products/15569 for the datasheet
// Mostly inspired by the code example

// I have discovered that this sensor need more than 60 mS after power up to
// function. For a battery operation it is long enough to deep-sleep the MCU
// and decrease the power consumption significantly.

class distSensor{
  const byte trigpin,echopin,pwrpin;
  public: distSensor(byte defpin1, byte defpin2, byte defpin3) : trigpin(defpin1), echopin (defpin2), pwrpin(defpin3){
  }
  SimpleSleep Sleep;
  void setup(){
    pinMode(trigpin, OUTPUT);
    pinMode(echopin, INPUT);
    pinMode(pwrpin, OUTPUT);
    digitalWrite(pwrpin, LOW);
  }
  void powerOn(){
    digitalWrite(pwrpin, HIGH); // wake up the sensor
    Sleep.deeplyFor(70);        // time needed after power up to be operational
  }

  void powerOff(){
    digitalWrite(pwrpin, LOW);
  }

  void ping(){
    digitalWrite(trigpin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin, LOW);
  }

  long echo(){
    long duration, distance;
    ping();
    duration = pulseIn(echopin, HIGH);
    distance = (duration/2) / 29.1;     // to get distance in cm
    return distance;
  }

};
