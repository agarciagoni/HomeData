/*  Author: Alejandro Garcia
 *  Starting date: 30/10/2019
 *  
 *  First script to try to put several sensors in the Piccolo Kitchen
 *  Sensors included: 
 *  - Ultrasonic Sensor Y401
 *  - VL53L0X distance sensor
 *  - sound sensor SEN-12642 (x2)
 *  - PIR sensor 
 *  - Buttons (cabinet movements)
 *  
 *  The serial print are set realted to the code in arduino_serial.py where it is written into a csv. Different approaches have to be considered and writen accordingly.
 *  ACTUAL STATUS: ONLY NUMBERS, NOT NAMES. 
 *  SEPARATION BY: /
 */
#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
////   Ultrasonic sensor
  // defines pins numbers
  const int trigPin = 2;
  const int echoPin = 3;
  // defines variables
  long duration;
  int distance;
    
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

//Sound sensor
  int sound_1=A1; //analog input
  int sound_2=A4; //analog input
    //const int gate_in=;
    //const int irq_gate_in=;
  int value_1;
  int value_2;

//PIR x2
  int pir_input_b=8;
  int pir_input_f=6;
  int pir_val_b;
  int stat_b=LOW;
  int cont_mov_b=0;
  int cont_b_total=0;
  int cont_b_pos=0;
  int pir_val_f;
  int stat_f=LOW;
  int cont_mov_f=0;
  int cont_f_total=0;
  int cont_f_pos=0;


void setup() {
////Configure back pins
pinMode(highpin_b, INPUT_PULLUP);
pinMode(medpin_b, INPUT_PULLUP);
pinMode(lowpin_b, INPUT_PULLUP);
////Configure front pins
//pinMode(highpin_f, INPUT_PULLUP);
//pinMode(lowpin_f, INPUT_PULLUP);
////Sound sensors
pinMode(sound_1,INPUT);
pinMode(sound_2,INPUT);
////Distance
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
//PIR
pinMode(pir_input_b,INPUT);
pinMode(pir_input_f,INPUT);
Serial.begin(9600); // Starts the serial communication
 // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
 // Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
 // Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}
void loop() {

//Ultrasonic
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH,500000);
  
  // Calculating the distance
  distance= duration*0.34/2;
////   Prints the distance on the Serial Monitor
//  Serial.print(" Distance sound (mm): ");Serial.print(distance);
//  

//Distance sensor time to travel
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
//  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
//    Serial.print(" Distance light (mm): ");
//    Serial.print(measure.RangeMilliMeter);
//  } else {
//    Serial.print(" out of range ");
//  }

// User Position

  if (distance <= 500 &&measure.RangeMilliMeter >=500){
    //Serial.print("Position: ");
    Serial.print("Left side/");
  }
  else if (measure.RangeMilliMeter <=500 && distance >= 500){
    //Serial.print("Position: "); 
    Serial.print("Right side/");
  }
  else if (measure.RangeMilliMeter <=500 && distance <= 500){
     //Serial.print("Position: "); 
     Serial.print("Center/");
  }
  else{
    // Serial.print("Position: "); 
     Serial.print("None/");    
  }
  delay(300); //Need this delay to not interfere with the analog reading. Look more into this.
  
//Sound
  value_1=analogRead(sound_1);
  value_2=analogRead(sound_2);
 //Serial.print("Sound sensor 1: ");
  Serial.print(value_1);
  if(value_1 <= 10)  {
    Serial.print(" Quiet/");  }
  else if( (value_1 > 10) && ( value_1 <= 30) )  {
    Serial.print(" Moderate/");  }
  else if(value_1 > 30)  {
    Serial.print(" Loud/");  }
 // Serial.print("Sound sensor 2: ");
  Serial.print(value_2);
  if(value_2 <= 10)  {
    Serial.print(" Quiet/");  }
  else if( (value_2 > 10) && ( value_2 <= 30) )  {
    Serial.print(" Moderate/");  }
  else if(value_2 > 30)  {
    Serial.print(" Loud/");  }
    
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

 Serial.print(back_pos);Serial.print("/");
 Serial.print(cont_mov_b);Serial.print("/");
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
  Serial.print(cont_b_pos);Serial.print("/");
  Serial.print(cont_b_total);Serial.print("/");
  
////Front cabinet
  //Serial.print("Front Cabinet: ");
//  if (digitalRead(highpin_f)==1){
    Serial.print("Up/");
//    cont_f_pos=0;
//  }
//  else if (digitalRead(lowpin_f)==1){
//    Serial.println("Down");
//    cont_f_pos=0;
//  }

//PIR sensor front
 Serial.print(cont_mov_f);Serial.print("/");
  pir_val_f=digitalRead(pir_input_f);
    if (pir_val_f==HIGH){
     if (stat_f==LOW){
      cont_f_total=cont_f_total+1;
      cont_f_pos=cont_f_pos+1;
      stat_f=HIGH;
      }
  } else{
    if (stat_f==HIGH){
      stat_f=LOW;
      }
  }
  Serial.print(cont_f_pos);Serial.print("/");
  Serial.println(cont_f_total);
 
  delay(100); 
}
