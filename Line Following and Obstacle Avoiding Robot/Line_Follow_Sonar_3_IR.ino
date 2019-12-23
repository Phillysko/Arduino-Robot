/*
  This line following demo uses three of the analog reflective sensors on the Funduino line following sensor
  to follow a line.
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
int irReflectL = 0;
int irReflectM = 0;
int irReflectR = 0;
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
  irReflectL = analogRead(lineLSense);
  irReflectM = analogRead(lineMSense);
  irReflectR = analogRead(lineRSense);
  distance = readPing();
 
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
  delay(300); //*********************************************Adjust these delays**********
  line_spinLeft();
  delay(200);     // timing for robot to turn about 60degrees in order to avoid obscatle.
  line_forward();
  delay(1500);
  line_spinRight();
  delay(300);     // timing for robot to turn about 90 degrees to start moving back towards the line
  line_forward();
  delay(700);
}

void follow_line() {

  if (irReflectL <= thresh && irReflectM <= thresh && irReflectR <= thresh) { // B B B
    line_stop();
    delay(1000);
    line_spinLeft();
    delay(150);

  }
  if (irReflectL <= thresh && irReflectM <= thresh && irReflectR >= thresh) {  // B B W
    line_spinRight();
  }
  if (irReflectL <= thresh && irReflectM >= thresh && irReflectR <= thresh) {  // B W B
    line_spinRight();
  }
  if (irReflectL <= thresh && irReflectM >= thresh && irReflectR >= thresh) { // B W W
    line_slipLeft();
  }
  if (irReflectL >= thresh && irReflectM <= thresh && irReflectR <= thresh) { // W B B
    line_spinLeft();
  }
  if (irReflectL >= thresh && irReflectM <= thresh && irReflectR >= thresh) { // W B W
    line_forward();
  }
  if (irReflectL >= thresh && irReflectM >= thresh && irReflectR <= thresh) { // W W B
    line_slipRight();
    delay(150);
  }
  if (irReflectL >= thresh && irReflectM >= thresh && irReflectR >= thresh) { // W W W
    line_spinLeft();
  }

}

// Sonar Fuctions
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
