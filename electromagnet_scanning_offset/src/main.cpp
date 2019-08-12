#include <Arduino.h>

//This code will scan the electromagnets in a raster pattern, starting from the botom left corner,
//moving right to the max value set, and then moving down a row and starting again from the left


// it will scan through a set number of times and record the voltage read from the photodiode, 
//wherever the max voltage occurs, it will then send out a dc analog offset to the electromagnets

//if the switch is ever turned off, it will wait for the switch to be turned back on and begin again
//the scanning process

//pinout for the microcontroller
const int led_pin = 2;
const int switch_pin = 3;
const int x_pin = A21;
const int y_pin = A22;
const int photodiode = A9;

//max and min analog values to be sent to electromagnets
int max_v = 40;
int min_v = 0;

//this is the amount of scans to take before setting an offset
int samples = 1;

//this array will hold the photodiode values
//size of array is = max_v*max_v;
int voltage[1600];

// this creates gobal variable for final offset 
int x_offset = 0;
int y_offset = 0;

//this function will find the max value in an array and then can return the modular index
//this is currently the part of the code not working
//I belive the logic is correct just do not know the syntax
void getMax(){
int l = (sizeof(voltage)/sizeof(voltage[0]));

int max_val = 0;
int max_val_index = 0;

for(int i = 0; i < l; i++){
  if(voltage[i] > max_val){
  max_val = voltage[i];
  max_val_index = i;
} 
  Serial.print("maximum value: ");
  Serial.println(max_val);
  Serial.print("maximum value index: ");
  Serial.println(max_val_index);
  Serial.println();
}
x_offset = (max_val_index % max_v);
y_offset = (((max_val_index)-(max_val_index % max_v))/max_v);

//  Serial.print("x offset: ");
//  Serial.println(x_offset);
//  Serial.print("y offset: ");
//  Serial.println(y_offset);
}

//this function scans the magnets in a ~raster pattern and records
//photodiode voltages
void scan_magnets(){
  Serial.println("scanning");
//  starting in the top left corner for scanning
  analogWrite(x_pin, min_v);
  analogWrite(y_pin, min_v);

//  iterates through full scan a set number of times
//the value for samples is set in the constants way above
  for(int iteration = 0; iteration < samples; iteration++){
//    iteratates through y values
  for(int y = 0; y < max_v; y++){
//    iterates thorugh x values
    for(int x = 0; x < max_v; x++){

//      records photodiode valuesv


// //the following code is to test out the code, uncomment to ensure max is reached manually
      //  if (y*max_v+x == 204){
      //    Serial.println("reached 750");

      //    Serial.print("y: ");
      //    Serial.println(y);

      //    Serial.print("x:  ");
      //    Serial.println(x);

      //    Serial.println("taking an image....");
      //    delay(2000);

      //    voltage[y*max_v+x] = analogRead(photodiode);
      //    Serial.println("done");
      //    delay(2000);
         
      //  }

      //  else{
       voltage[y*max_v+x] = analogRead(photodiode);
      //  }


       Serial.print(voltage[y*max_v+x]);
       Serial.print("<-voltage mod_index->");
       Serial.println(y*max_v+x);
       


//      increments x scan position
     analogWrite(x_pin, min_v+x);
     delay(10);
    }
//      increments y scan position
    analogWrite(y_pin,min_v+y);
   }
  }
//calls get max functino
 

//turns on led when everything is done
  digitalWrite(led_pin, 1);
}

void setup() {
  digitalWrite(led_pin, 0);
  pinMode(switch_pin,INPUT_PULLDOWN);
  pinMode(led_pin,OUTPUT);
  pinMode(photodiode,INPUT);
  Serial.begin(115200);



//  will only begin when switch is turned on
while(digitalRead(switch_pin) == 0){
  Serial.println("Waiting");
  delay(300);
}

  scan_magnets();

  getMax();
   
  Serial.println("done scan");

  
//  sending dc analog offset to magnets
  analogWrite(x_pin, x_offset);
  analogWrite(y_pin, y_offset); 
  
  Serial.println("constant dc output set: ");
  Serial.print("x offset: ");
  Serial.println(x_offset);
  Serial.print("y offset: ");
  Serial.println(y_offset);
  
}

void loop() {


  //  will reset to scanning mode when switch is turned off
  if (digitalRead(switch_pin) == 0) {

    //    waiting for switch to turn back on
    digitalWrite(led_pin, 0);
    while (digitalRead(switch_pin) == 0) {
      Serial.print("Waiting");
      delay(100);
    }
    scan_magnets();

    getMax();
   
    Serial.println("done scan");

  
  //  sending dc analog offset to magnets
    analogWrite(x_pin, x_offset);
    analogWrite(y_pin, y_offset); 
    
    Serial.println("constant dc output set: ");
    Serial.print("x offset: ");
    Serial.println(x_offset);
    Serial.print("y offset: ");
    Serial.println(y_offset);
  
  }


}

