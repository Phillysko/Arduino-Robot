 /*
  Philip Skopek
  Ke Ma
  
  ET 387 Lab 3
  
  This line following demo uses three IR sensors along with a Sonar to follow a 
  line and avoid obstacles placed in its path. At the end of the line the robot
  needs to perform a U-turn and return back to the start.
*/

#include <Servo.h>
#include <NewPing.h>

//sensor pins
#define TRIGGER_PIN 6 //analog input 6
#define ECHO_PIN 12 //analog input 12
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //sensor function

Servo servoLeft; // Define left servo
Servo servoRight; // Define right servo


const int lineLSense = A2;
const int lineMSense = A3;
const int lineRSense = A4;
int irL = 0;
int irM = 0;
int irR = 0;
int thresh = 650;

int distance = 100;

void setup() {

  servoLeft.attach(11);
  servoRight.attach(10);

  delay(1000);
  distance = readPing();
  delay(100);
  distance = readPing();

}
void loop() {

  // Read reflective sensors
  irL = analogRead(lineLSense);
  irM = analogRead(lineMSense);
  irR = analogRead(lineRSense);
  distance = readPing(); // Read sonar distance

  if (distance <= 8)
  {
    avoid_obstacle();
  }
  else
  {
    follow_line();
  }


}

void avoid_obstacle() {
  line_stop();
  delay(300); 
  line_spinLeft();
  delay(1000);     // timing for robot to turn about 60degrees in order to avoid obscatle.
  line_forward();
  delay(1500);
  line_spinRight();
  delay(900);     // timing for robot to turn about 90 degrees to start moving back towards the line
  line_forward();
  delay(1800);
  line_spinRight();
  delay(500);
  line_forward();

  while (irL >= thresh) {       // After performing the obstacle avoiding manuever, the robot will travel straight until detecting the line again. We need this while loop
    irL = analogRead(lineLSense); // so that after detecting the line the robot performs the re-cenerting maneuver
    irR = analogRead(lineRSense);
    Serial.println("in the while loop");
  }
  line_backwards();       // This is the re-centering maneuver executed after avoiding the obstacle 
  delay(150):
  line_spinLeft();
  delay(500);
}

}

void follow_line() {

  if (irL <= thresh && irM <= thresh && irR <= thresh) { // B B B
    line_stop();
    delay(1000);
    line_spinLeft();
    delay(150);

  }
  if (irL <= thresh && irM <= thresh && irR >= thresh) {  // B B W
    line_spinRight();
  }
  if (irL <= thresh && irM >= thresh && irR <= thresh) {  // B W B
    line_spinRight();
  }
  if (irL <= thresh && irM >= thresh && irR >= thresh) { // B W W
    line_slipLeft();
  }
  if (irL >= thresh && irM <= thresh && irR <= thresh) { // W B B
    line_spinLeft();
  }
  if (irL >= thresh && irM <= thresh && irR >= thresh) { // W B W
    line_forward();
  }
  if (irL >= thresh && irM >= thresh && irR <= thresh) { // W W B
    line_slipRight();
  }
  if (irL >= thresh && irM >= thresh && irR >= thresh) { // W W W, spin left so that a u-turn is executed at the end of the path
    line_spinLeft();
  }

}

// Sonar Fuctions, returns distace in cm
int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}

// Motion routines for line following
void line_forward() {
  servoLeft.write(180);
  servoRight.write(0);
}
void line_slipRight() {
  servoLeft.write(180);
  servoRight.write(90);
}
void line_slipLeft() {
  servoLeft.write(90);
  servoRight.write(0);
}
void line_spinRight() {
  servoLeft.write(180);
  servoRight.write(180);
}
void line_spinLeft() {
  servoLeft.write(0);
  servoRight.write(0);
}
void line_stop() {
  servoLeft.write(90);
  servoRight.write(90);
}
