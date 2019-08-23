
// control the AweSEM prototypev2 motors

// use m and s to type commands into Serial

// EX: to move it by 30 units up type m30 (m-30 for downwards)
// speed is default 10rpm, to change type s30 to set speed to 30rpm


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
    myMotor->setSpeed(val);

   }  
   else if (receivedChars[0] == 'm'){ 
     Serial.println("Move: ");
     Serial.println(val);
    
    if(val>0){
      myMotor->step(val,FORWARD,DOUBLE);
    }
    else{
      myMotor->step(abs(val),BACKWARD,DOUBLE);
    }

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

  myMotor->setSpeed(50);  // 10 rpm

   Serial.println("<Arduino is ready>");
}




void loop() {
 recvWithEndMarker();
 showNewData();
}
