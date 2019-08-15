/*
//  Rhis is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
//   It won't work with v1.x motor shields! Only for the v2's with built in PWM
//   control

//   For use with the Adafruit Motor Shield v2
//   ---->	http://www.adafruit.com/products/1438
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

int revs=0;
int size;
String valStr;
int val;


byte byteRead;

const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(400, 1);


void recvWithEndMarker() {
 static byte ndx = 0;
 char endMarker = '\n';
 char rc;
 
 // if (Serial.available() > 0) {
           while (Serial.available() > 0 && newData == false) {
 rc = Serial.read();

 if (rc != endMarker) {
 receivedChars[ndx] = rc;
 ndx++;
 if (ndx >= numChars) {
 ndx = numChars - 1;
 }
 }
 else {
 receivedChars[ndx] = '\0'; // terminate the string
 ndx = 0;
 newData = true;
 }
 }
}

void showNewData() {
 if (newData == true) {
   size = sizeof(receivedChars)/sizeof(receivedChars[0]);
   char speed[size-1];

  for(int i = 1; i<size; i++){
    speed[i-1] = receivedChars[i];
  }

  valStr = String(speed);
  val = valStr.toInt();

   if (receivedChars[0] == 's'){
    Serial.println("Speed: ");
    Serial.println(val);
   }
   else if (receivedChars[0] == 'm'){ 
     Serial.println("Move: ");
     Serial.println(val);
   }
   else{
     Serial.println("Error");
   }

 newData = false;
 }
}

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps


  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz

  myMotor->setSpeed(0);  // 10 rpm

   Serial.println("<Arduino is ready>");
}




void loop() {
 recvWithEndMarker();
 showNewData();
}
