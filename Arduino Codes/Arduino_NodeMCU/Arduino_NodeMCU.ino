/*  Author: Alejandro Garcia
 *  Starting date: 19/11/2019
 *  
 *  First script to try to put several sensors in the Piccolo Kitchen
 *  Sensors included and prepared for use in the kitchen, with its cables: 
 *  - Ultrasonic Sensor Y401
 *  - VL53L0X distance_sonic sensor
 *  - Sound sensor x2 (2nd with 100k resistor and no R3).
 *  - PIR sensor x1
 *  - Buttons (cabinet movements)
 *  
 *  The serial print are set realted to the code in arduino_serial.py where it is written into a csv. Different approaches have to be considered and writen accordingly.
 *  ACTUAL STATUS: ONLY NUMBERS, NOT NAMES. 
 *  SEPARATION BY: /
 */
 #include "Adafruit_VL53L0X.h"
 #include <NewPing.h>
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

////   Ultrasonic sensor
  // defines pins numbers
  const int trigPin = 0;
  const int echoPin = 1;
  NewPing sonar(trigPin, echoPin);   //  An instance of the NewPing class
  // defines variables
  long duration;
  int distance_sonic;
    
//Cabinets
  const int lowpin_b=10;
  const int medpin_b=11;
  const int highpin_b=12;
  int state_h=0;
  int state_m=0;
  int state_l=0;
  String back_pos="LOW";
  //const int lowpin_f=4;
  //const int highpin_f=5;
  //PIR x1
  int pir_input_b=9;
  int pir_val_b;
  int stat_b=LOW;
  int cont_mov_b=0;
  int cont_b_total=0;
  int cont_b_pos=0;
//Sound sensor
  int sound_1=A1; //analog input
  int value_sound1;
  String stat1="Q";
  int sound_2=A3; //analog input
  int value_sound2;
  String stat2="Q";

//Position
String UserPos="Na";
void setup() {
////Configure back pins
pinMode(highpin_b, INPUT_PULLUP);
pinMode(medpin_b, INPUT_PULLUP);
pinMode(lowpin_b, INPUT_PULLUP);
////Sound sensors
pinMode(sound_1,INPUT);
pinMode(sound_2,INPUT);
////distance_sonic
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
//PIR
pinMode(pir_input_b,INPUT);


Serial.begin(9600); // Starts the serial communication
Serial1.begin(9600);
 // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
 // Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
}

void loop() {
//Sound
  value_sound1=analogRead(sound_1);
  Serial.print(value_sound1); Serial.print(",");
  Serial.print("Status 1: ");
  if(value_sound1 <= 10)
  {
    stat1="Q";
  }
  else if( (value_sound1 > 10) && ( value_sound1 <= 30) )
  {
    stat1="M";
  }
  else if(value_sound1 > 30)
  {
    stat1="L";
  }
  Serial.print(stat1);Serial.print(',');
  value_sound2=analogRead(sound_1);
  Serial.print(value_sound2); Serial.print(",");
  Serial.print("Status 2: ");
  if(value_sound2 <= 10)
  {
    stat2="Q";
  }
  else if( (value_sound2 > 10) && ( value_sound2 <= 30) )
  {
    stat2="M";
  }
  else if(value_sound2 > 30)
  {
    stat2="L";
  }
  Serial.print(stat2);Serial.print(',');
//Distance sensor time to travel
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
//  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
//    Serial.print(" Distance light (mm): ");
//    Serial.print(measure.RangeMilliMeter);
//  } else {
//    Serial.print(" out of range ");
//  }
//Ultrasonic
 distance_sonic = sonar.ping_cm();           //  Ask NewPing to trigger, fetch echo and calculate distance in cm
////   Prints the distance on the Serial Monitor
Serial.print(distance_sonic);Serial.print(",");
Serial.print(measure.RangeMilliMeter);Serial.print(",");
//  


// User Position

  if (distance_sonic <= 50 && distance_sonic != 0 &&measure.RangeMilliMeter >=500){
    //Serial.print("Position: ");
    UserPos="LS";
  }
  else if (measure.RangeMilliMeter <=500 && distance_sonic >= 50 || distance_sonic==0){
    //Serial.print("Position: "); 
     UserPos="RS";
  }
  else if (measure.RangeMilliMeter <=500 && distance_sonic!=0 && distance_sonic <= 50){
     //Serial.print("Position: "); 
      UserPos="CE";
  }
  else{
    // Serial.print("Position: "); 
      UserPos="Na";    
  }
  UserPos="Na";
  Serial.print(UserPos);Serial.print(",");
  //delay(300); //Need this delay to not interfere with the analog reading. Look more into this.
  

    
////Back cabinet 
  state_h=digitalRead(highpin_b);
  state_m=digitalRead(medpin_b); 
  state_l=digitalRead(lowpin_b);
 //Serial.print("Back cabinet: ");
if (state_h==LOW){
 cont_mov_b+=1;
 back_pos="HIGH";
 cont_b_pos=0;
}
else if (state_m==LOW){
  cont_mov_b+=1;
  back_pos="MED";
  cont_b_pos=0;
}
else if(state_l==LOW){
  cont_mov_b+=1;
  back_pos="LOW";
  cont_b_pos=0;
}

 Serial.print(back_pos);Serial.print(",");
 Serial.print(cont_mov_b);Serial.print(",");
 //PIR sensor back
  pir_val_b=digitalRead(pir_input_b);
  if (pir_val_b==HIGH){
     if (stat_b==LOW){
      cont_b_total=cont_b_total+1;
      cont_b_pos=cont_b_pos+1;
      stat_b=HIGH;
      }
  } else{
    if (stat_b==HIGH){
      stat_b=LOW;
      }
  }
  Serial.print(cont_b_pos);Serial.print(",");
  Serial.println(cont_b_total);//Serial.print(",");
  
  // Here we include all the variables, create a string and send it over serial to the NodeMCU.
  String package=(String("Data: ")+String(value_sound1)+','+String(stat1)+','+String(value_sound2)+','+String(stat2)+','+
                   String(distance_sonic)+','+String(measure.RangeMilliMeter)+','+String(UserPos)+','+
                   String(back_pos)+','+String(cont_mov_b)+','+String(cont_b_pos)+','+String(cont_b_total)+','+
                   String("-")+','+String(0)+','+String(0)+','+String(0)+','+
                   String("-")+','+String(0)+','+String(0)+','+String(0)+'\n');
  Serial1.print(package);
  //Serial.print(package);
  delay(100); 

}
