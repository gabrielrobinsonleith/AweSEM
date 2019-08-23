

Table table;
import processing.serial.*;
Serial myPort; //creates a software serial port on which you will listen to Arduino
String fileName;
String path;
String extension = ".csv";
 int xPos = 0; 

void setup()
{
  String portName = Serial.list()[3]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 115200);
  
  table = new Table();

  table.addColumn("x");
  table.addColumn("y");
  table.addColumn("intensity");
  
  size(600, 600);
  background(255);
  


}




void draw()
{ 
  String val;
    val = myPort.readStringUntil('\n'); //The newline separator separates each Arduino loop. We will parse the data by each newline separator.
    if (val!= null) { //We have a reading! Record it.
      val = trim(val); //gets rid of any whitespace or Unicode nonbreakable space
     println(val); //Optional, useful for debugging. If you see this, you know data is being sent. Delete if you like.

      float sensorVal[] = float(split(val, ',')); //parses the packet from Arduino and places the valeus into the sensorVals array. I am assuming floats. Change the data type to match the datatype coming from Arduino.
      TableRow newRow = table.addRow(); //add a row for this new reading
      

      newRow.setFloat("x", sensorVal[0]);//record a unique identifier (the row’s index)
      newRow.setFloat("y", sensorVal[1]);
      newRow.setFloat("intensity",sensorVal[2]);
      
        int inByte = int(sensorVal[2]); 
        
         
         noStroke();
         fill(inByte);
         rect(map(sensorVal[0],0,40,0,600),map(sensorVal[1],0,40,0,600),600/40,600/40);
         if(sensorVal[0] == 255){
           noStroke();
           fill(255,0,0);
           rect(map(sensorVal[1],0,40,0,600),map(sensorVal[2],0,40,0,600),600/40,600/40);
        }  
}

}
