/*  Author: Alejandro Garcia
 *  Date: 30/10/2019
 *  
 *  First script to try to put several sensors in the Piccolo Kitchen
 *  Sensors included: 
 *  - Ultrasonic Sensor Y401
 *  - VL53L0X distance sensor
 *  - sound sensor SEN-12642 (x2)
 *  - PIR sensor 
 *  - Buttons (cabinet movements)
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
//const int lowpin_b=1;
//const int medpin_b=2;
//const int highpin_b=3;
// int state_h=0;
// int state_m=0;
// int state_l=0;
//const int lowpin_f=4;
//const int highpin_f=5;

//Sound sensor
const int sound_1=1; //analog input
const int sound_2=2; //analog input
  //const int gate_in=;
  //const int irq_gate_in=;
int value_1;
int value_2;

//PIR
int pir_input_1=8;
  //int pirState_1=LOW;
int pir_val_1;


void setup() {
////Configure back pins
//pinMode(highpin_b, INPUT_PULLUP);
//pinMode(medpin_b, INPUT_PULLUP);
//pinMode(lowpin_b, INPUT_PULLUP);
////Configure front pins
//pinMode(highpin_f, INPUT_PULLUP);
//pinMode(lowpin_f, INPUT_PULLUP);
////Sound sensors
//pinMode(sound_1,INPUT);
//pinMode(sound_2,INPUT);
//Distance
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input

Serial.begin(9600); // Starts the serial communication
 // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
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
// //Distance sensor
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
//  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
//    Serial.print(" Distance light (mm): ");
//    Serial.print(measure.RangeMilliMeter);
//  } else {
//    Serial.print(" out of range ");
//  }
// Position

  if (distance <= 500 &&measure.RangeMilliMeter >=500){
    Serial.print("Position: ");Serial.print("Left side/");
  }
  else if (measure.RangeMilliMeter <=500 && distance >= 500){
    Serial.print("Position: "); Serial.print("Right side/");
  }
  else if (measure.RangeMilliMeter <=500 && distance <= 500){
     Serial.print("Position: "); Serial.print("Center/");
  }
  else{
     Serial.print("Position: "); Serial.print("None/");    
  }
////Back
//  state_h=digitalRead(highpin_b);
//  state_m=digitalRead(medpin_b); 
//  state_l=digitalRead(lowpin_b);
//if (state_h==LOW){
//  Serial.println("HIGH");
//}
//else if (state_m==LOW){
//  Serial.println("MED");
//}
//else if(state_l==LOW){
//  Serial.println("LOW");
//}
////Front
//  Serial.println("Front Cabinet: ");
//  if (digitalRead(highpin_f)==1){
//    Serial.println("Up");
//  }
//  else if (digitalRead(lowpin_f)==1){
//    Serial.println("Down");
//  }
//Sound
  value_1=analogRead(sound_1);
  value_2=analogRead(sound_2);
  Serial.print(" Sound sensor 1: ");Serial.print(value_1);
  if(value_1 <= 10)  {
    Serial.print(" Quiet./");  }
  else if( (value_1 > 10) && ( value_1 <= 30) )  {
    Serial.print(" Moderate./");  }
  else if(value_1 > 30)  {
    Serial.print(" Loud./");  }
  Serial.print(" Sound sensor 2: ");Serial.print(value_2);
  if(value_2 <= 10)  {
    Serial.print(" Quiet./");  }
  else if( (value_2 > 10) && ( value_2 <= 30) )  {
    Serial.print(" Moderate./");  }
  else if(value_2 > 30)  {
    Serial.print(" Loud./");  }

//PIR sensors
  pir_val_1=digitalRead(pir_input_1);
  Serial.print(" Presence: ");Serial.println(pir_val_1);

  delay(1000); 
}
