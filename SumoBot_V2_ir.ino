/* SUMO with ATTACK function
  robot drives forward looking for opponent while making sure it backs away then turns when it goes over a white line.
  If either left or right IR top distance sensors sees an object infront the head towards it and faster if both sensors
  are triggered at the same time while still making sure it backs away if it sees a while line.
*/


#include <Servo.h>

Servo amotor; // create servo object to control a servo
Servo bmotor; // create servo object to control a servo
bool lInfrared, rInfrared; //top collision avoidance sensors
int led = 13;
int sensorValue = 0; // variable to store value to for floor IR sensor calibration
int Lfloorsensor = A2; //definition of pins used by floor IR sensors
int Mfloorsensor = A3;
int Rfloorsensor = A4;
int Button = 2; //button to start robot
int lInfraredPin = 3; // top collision detection sensors
int rInfraredPin = 8;
long result = 0;
int White = 900; //white line value - change depending on calibration value on Serial Monitor
int aspeed = 1500; // variable to store the servo position or speed
int bspeed = 1500; // variable to store the servo position or speed

void setup()                    // run once, when the sketch starts
{
  Serial.begin(9600); //start Serial Monitor output
  pinMode(13, OUTPUT); //LED pin is an output
  amotor.attach(10); // right servo connect to pin 10
  bmotor.attach(11); // left servo connect to pin 11
  digitalWrite(led, HIGH); //show led on before robot starts

  pinMode(Button, INPUT_PULLUP);  //simple button connect https://www.arduino.cc/en/uploads/Tutorial/inputPullupButton.png
  // just connect ground and signal
  while (digitalRead(Button) == 1) { // wait until button is pushed
  }
  digitalWrite(led, LOW); //turn led off after robot starts
 tone(4,440,500); // play sound before starting
  delay(500);
}


//---------------------------FUNCTIONS---------------------------//
// RCtime is meant to convert digital signals to analog values 
// we use it here, but is not necessary because we already have an analog to digital converter on the
// line following sensors, but leave anyway
long RCtime(int sensPin) {
  long result = 0;
  pinMode(sensPin, OUTPUT);       // make pin OUTPUT
  digitalWrite(sensPin, HIGH);    // make pin HIGH to discharge capacitor
  delay(1);                       // wait a  ms to make sure cap is discharged

  pinMode(sensPin, INPUT);        // turn pin into an input and time till pin goes low
  digitalWrite(sensPin, LOW);     // turn pullups off or it won't work
  while (digitalRead(sensPin)) {  // wait for pin to go low
    result++;
  }

  return result;                   // report results
}

void blinkLED() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);               // wait for 0.2 seconds
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
}

void goForward(int i) {
  aspeed = 1400; //set speed variable
  bspeed = 1600; //set speed variable
  amotor.writeMicroseconds(aspeed); // tell servo to move as indicated by  variable 'aspeed'
  bmotor.writeMicroseconds(bspeed); // tell servo to move as indicated by  variable 'bspeed'
  delay(i); //time for the servo to move

}

void goBack(int i) {
  aspeed = 1600; //set speed variable
  bspeed = 1400; //set speed variable
  amotor.writeMicroseconds(aspeed); // tell servo to move as indicated by  variable 'aspeed'
  bmotor.writeMicroseconds(bspeed); // tell servo to move as indicated by  variable 'bspeed'
  delay(i); //time for the servo to move

}

void spinRight(int i) {
  aspeed = 2000; //set speed variable
  bspeed = 2000; //set speed variable
  amotor.writeMicroseconds(aspeed); // tell servo to move as indicated by  variable 'aspeed'
  bmotor.writeMicroseconds(bspeed); // tell servo to move as indicated by  variable 'bspeed'
  delay(i); //time for the servo to move

}

void spinLeft(int i) {
  aspeed = 1000; //set speed variable
  bspeed = 1000; //set speed variable
  amotor.writeMicroseconds(aspeed); // tell servo to move as indicated by  variable 'aspeed'
  bmotor.writeMicroseconds(bspeed); // tell servo to move as indicated by  variable 'bspeed'
  delay(i); //time for the servo to move

}

void attack() {
  aspeed = 1000; //set speed variable
  bspeed = 2000; //set speed variable
  amotor.writeMicroseconds(aspeed); // tell servo to move as indicated by  variable 'aspeed'
  bmotor.writeMicroseconds(bspeed); // tell servo to move as indicated by  variable 'bspeed'

  while (lInfrared && rInfrared) {
    lInfrared = !digitalRead(lInfraredPin);
    rInfrared = !digitalRead(rInfraredPin);
  }
}

//---------------------------END OF FUNCTIONS---------------------------//

void loop()                     // this part of the code runs continuously
{
  sensorValue = analogRead(Mfloorsensor); //read the IR floor sensor in the middle and put it in variable sensorValue
  Serial.println(sensorValue); //print the value on Serial Monitor to edit our White variable value
Sumo:
  if (RCtime(Lfloorsensor) > White) {
    goBack(900);
    spinRight(750);

  }
  else if (RCtime(Rfloorsensor) > White) {
    goBack(900);
    spinLeft(750);
  }

  else if (RCtime(Mfloorsensor) > White) {
    goBack(1200);
    //  spinLeft(750);
  }

  else goForward(100);

  // check iR sensors to see opponent

  lInfrared = !digitalRead(lInfraredPin);
  rInfrared = !digitalRead(rInfraredPin);

  if (lInfrared) {
    if ( rInfrared) {
      attack();
    }
    else
    {
      spinLeft(100);
    }
  }
  else if (rInfrared) {
    spinRight(100);
  }
}
