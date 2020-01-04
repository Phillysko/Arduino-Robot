/*

  SumoBot with Sonar and IR Floor sensors

The code is designed fo the sumobot to operate on a black circular arena with
a white edge.
The robot spins if sonar does not detect anything
and then goes full speed forward if sonar does detect
an object.
IR sensors are used on the front and back of robot 
to detect outer white edge and perform evasive maneuvers


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
Servo servoSonar;

const int FrontMSensor = A3; //Front linke tracking sensor mounted on bottom
const int RearSensor = 3;  // Digital IR Sensor Module attached to back of robot
int irFront = 0;
int irRear = 0;
byte button = 1; // Assigns button to pin 1
int distance;



void setup()
{
  Serial.begin(9600);
  servoLeft.attach(11);
  servoRight.attach(10);
  servoSonar.attach(9);
  pinMode(button, INPUT_PULLUP); //Assigns button as input
  pinMode(RearSensor, INPUT); 
  servoSonar.write(82);
  delay(50);

  while (digitalRead(button) == HIGH) {} // Awaits button press
  delay(500);

}
void loop()
{

  int irFront = analogRead(FrontMSensor);
  int irRear = digitalRead(RearSensor);  // 1 = black , 0 = white
  distance = readPing();

  /*
    // For sensor testing and troubleshooting
    Serial.print(distance);
    Serial.println("cm");
    Serial.print(irFront);
    Serial.println(" Front");
    Serial.print(irRear);
    Serial.println(" Rear");
  */

  delay(50);
  spin(); // start rotete if (distance > 30)
  while (distance < 30  ) { // sonar detects opponent 
    goForward();
    distance = readPing();
    irFront = analogRead(FrontMSensor);
    irRear = analogRead(RearSensor);
    
   //exit loop to recheck sonar distance and check IR values to make sure robot is not at the white edge 
    if ( irFront < 650 || irRear == 1 ) {   d 
      break;
    }
    delay(10);
  }


  if (irFront >= 650 )  // > 650 means white line
  {
    Stop();
    delay (50);
    goBackward();
    delay(500);
  }

  if (irRear == 0 )  // if rear sensor sees white
  {
    Stop();
    delay (50);
    goForward();
    delay(500);
  }

  /* ----------- debugging ----------------
    Serial.print(ultrasonic.Ranging(CM));
    Serial.println("cm");
    Serial.println("IR front :");
    Serial.println(irFront);
    Serial.println("IR back :");
    Serial.println(irRear);
  */

} //--------Functions-----------
  

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}

void goForward() {
  servoLeft.write(0);
  servoRight.write(180);
}
void goBackward() {
  servoLeft.write(180);
  servoRight.write(0);
}
void spin() {
  servoLeft.write(0);
  servoRight.write(0);
}
void Stop() {
  servoLeft.write(90);
  servoRight.write(90);
}
