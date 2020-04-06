/*  Author: Alejandro Garcia
 *  Starting date: 4/02/2020
 *  
 *  Script to try to  several sensors in the Piccolo Kitchen
 *  Sensors included and prepared for use in the kitchen, with its cables: 
 *  - Ultrasonic Sensor Y401
 *  - Maxbotix Ultrasonic range finder EZ
 *  - VL53L0X distance_sonic sensorp
 *  - Sound sensor x2 (2nd with 100k resistor and no R3).
 *  - PIR sensor x3
 *  - Buttons (cabinet movements): 3 Back cabinet, 2 Front Right, 2 Front Left
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
  const int trigPin = 1;
  const int echoPin = 0;
  NewPing sonar(trigPin, echoPin);   //  An instance of the NewPing class
  // defines variables
  long duration;
  int distance_sonic;
//Sonar
  int pin_sonar = 14;      
  int distance_sonar;
  int pulse_sonar;
//Cabinets
  const int lowpin_b=10;
  const int medpin_b=11;
  const int highpin_b=12;
  int state_h=0;
  int state_m=0;
  int state_l=0;
  String back_pos="LOW";
  //PIR x1
  int pir_input_b=9;
  int pir_val_b;
  int stat_b=LOW;
  int cont_mov_b=0;
  int inter_b_total=0;
  int inter_b_pos=0;

  //Front Right
  const int downpin_fr=6;
  const int uppin_fr=7;
  int state_fr_u=0;
  int state_fr_d=0;
  String front_right_pos="LOW";
  //PIR x1
  int pir_input_fr=5;
  int pir_val_fr;
  int stat_fr=LOW;
  int cont_mov_fr=0;
  int inter_fr_total=0;
  int inter_fr_pos=0;

  //Front Left
  const int downpin_fl=3;
  const int uppin_fl=4;
  int state_fl_u=0;
  int state_fl_d=0;
  String front_left_pos="LOW";
  //PIR x1
  int pir_input_fl=2;
  int pir_val_fl;
  int stat_fl=LOW;
  int cont_mov_fl=0;
  int inter_fl_total=0;
  int inter_fl_pos=0;
  
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
////Configure front righ pins
pinMode(uppin_fr, INPUT_PULLUP);
pinMode(downpin_fr, INPUT_PULLUP);
////Configure front pins
pinMode(downpin_fl, INPUT_PULLUP);
pinMode(downpin_fl, INPUT_PULLUP);
////Sound sensors
pinMode(sound_1,INPUT);
pinMode(sound_2,INPUT);
////distance_sonic
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(pin_sonar, INPUT); // Sets the sonar pin as an Input
//PIR
pinMode(pir_input_b,INPUT);
pinMode(pir_input_fr,INPUT);
pinMode(pir_input_fl,INPUT);

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

//Sonar
pulse_sonar=pulseIn(pin_sonar,HIGH); // read pulse distance
distance_sonar=25.4*pulse_sonar/147; //from the DataSheet 147us/inch., from pulse distance to mm.
////   Prints the distance on the Serial Monitor
Serial.print(distance_sonic);Serial.print(",");
Serial.print(distance_sonar);Serial.print(",");
Serial.print(measure.RangeMilliMeter);Serial.print(",");

// User Position

  if (distance_sonic <= 50 && distance_sonic != 0){ //&&measure.RangeMilliMeter >=500){
    //Serial.print("Position: ");
    UserPos="LS";
  }
  else if (measure.RangeMilliMeter <=500){ //&& distance_sonic >= 50 || distance_sonic==0){
    //Serial.print("Position: "); 
     UserPos="RS";
  }
  else if (distance_sonar<=500){  //(measure.RangeMilliMeter <=500 && distance_sonic!=0 && distance_sonic <= 50){
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
 inter_b_pos=0;
}
else if (state_m==LOW){
  cont_mov_b+=1;
  back_pos="MED";
  inter_b_pos=0;
}
else if(state_l==LOW){
  cont_mov_b+=1;
  back_pos="LOW";
  inter_b_pos=0;
}

 Serial.print(back_pos);Serial.print(",");
 Serial.print(cont_mov_b);Serial.print(",");
 //PIR sensor back
  pir_val_b=digitalRead(pir_input_b);
  if (pir_val_b==HIGH){
     if (stat_b==LOW){
      inter_b_total=inter_b_total+1;
      inter_b_pos=inter_b_pos+1;
      stat_b=HIGH;
      }
  } 
  else{
    if (stat_b==HIGH){
      stat_b=LOW;
      }
  }
  Serial.print(inter_b_pos);Serial.print(",");
  Serial.println(inter_b_total);Serial.print(",");

////Front right cabinet 
  state_fr__u=digitalRead(uppin_fr);
  state_fr_d=digitalRead(downpin_fr); 

 //Serial.print("Back cabinet: ");
if (state_fr_u==LOW){
 cont_mov_fr+=1;
 front_r_pos="UP";
 inter_fr_pos=0;
}
else if (state_fr_d==LOW){
  cont_mov_fr+=1;
  front_r_pos="MED";
  inter_fr_pos=0;
}


 Serial.print(front_r_pos);Serial.print(",");
 Serial.print(cont_mov_fr);Serial.print(",");
 //PIR sensor front r
  pir_val_fr=digitalRead(pir_input_fr);
  if (pir_val_fr==HIGH){
     if (stat_fr==LOW){
      inter_fr_total=inter_fr_total+1;
      inter_fr_pos=inter_fr_pos+1;
      stat_fr=HIGH;
      }
  } 
  else{
    if (stat_fr==HIGH){
      stat_fr=LOW;
      }
  }
  Serial.print(inter_fl_pos);Serial.print(",");
  Serial.println(inter_fl_total);Serial.print(",");

 ////Front left cabinet
  state_fl__u=digitalRead(uppin_fl);
  state_fl_d=digitalRead(downpin_fl); 

 //Serial.print("Back cabinet: ");
if (state_fl_u==LOW){
 cont_mov_fl+=1;
 front_l_pos="UP";
 inter_fl_pos=0;
}
else if (state__fl_d==LOW){
  cont_mov_fl+=1;
  front_l_pos="MED";
  inter_fl_pos=0;
}


 Serial.print(front_l_pos);Serial.print(",");
 Serial.print(cont_mov_fl);Serial.print(",");
 //PIR sensor front r
  pir_val_fl=digitalRead(pir_input_fl);
  if (pir_val_fl==HIGH){
     if (stat_fl==LOW){
      inter_fl_total=inter_fl_total+1;
      inter_fl_pos=inter_fl_pos+1;
      stat_fl=HIGH;
      }
  }
  else{
    if (stat_fl==HIGH){
      stat_fl=LOW;
      }
  }
  Serial.print(inter_fl_pos);Serial.print(",");
  Serial.println(inter_fl_total);//Serial.print(",");

  //NodeMCU COMMUNICATION
  // Here we include all the variables, create a string and send it over serial to the NodeMCU.
  
  String package=(String("Data: ")+String(value_sound1)+','+String(stat1)+','+String(value_sound2)+','+String(stat2)+','+String(distance_sonic)+','+String(distance_sonar)+','+String(measure.RangeMilliMeter)+','+String(UserPos)+','+
                  String(back_pos)+','+String(cont_mov_b)+','+String(inter_b_pos)+','+String(inter_b_total)+','+
                  String(front_right_pos)+','+String(cont_mov_fr)+','+String(inter_fr_pos)+','+String(inter_fr_total)+','+
                  String(front_left_pos)+','+String(cont_mov_fl)+','+String(inter_fl_pos)+','+String(inter_fl_total)+'\n');
  Serial1.print(package);
  //Serial.print(package);
  delay(100); 

}
