/*  Author: Alejandro Garcia
 *  Starting date: 19/11/2019
 *  
 *  First script to try to put several sensors in the Piccolo Kitchen
 *  Sensors included and prepared for use in the kitchen, with its cables: 
 *  - Ultrasonic Sensor Y401
 *  - VL53L0X distance_sonic sensor
 *  - Sound sensor x1
 *  - PIR sensor x1
 *  - Buttons x5 ( 3 backside,2 upper cabinet left)
 *  - 
 *  
 *  The serial print are set realted to the code in arduino_serial.py where it is written into a csv. Different approaches have to be considered and writen accordingly.
 *  ACTUAL STATUS: ONLY NUMBERS, NOT NAMES. 
 *  SEPARATION BY: ","
 */
 #include "Adafruit_VL53L0X.h"
 #include <NewPing.h>
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

////   Ultrasonic sensor
  // defines pins numbers
  const int trigPin = 3;
  const int echoPin = 2;
  NewPing sonar(trigPin, echoPin);   //  An instance of the NewPing class
  // defines variables
  long duration;
  int distance_sonic;
    
//Cabinets
  const int lowpin_b=13;  //Backside pins
  const int medpin_b=12;
  const int highpin_b=11;
  int state_h=0;
  int state_m=0;
  int state_l=0;
  String back_pos="LOW";
  const int lowpin_f=8;   //Front-Left pins
  const int highpin_f=9;
  String front_pos="UP";


  //PIR x1
  int pir_input_b=10;
  int pir_val_b;
  int stat_b=LOW;
  int cont_mov_b=0;
  int cont_b_total=0;
  int cont_b_pos=0;

  int pir_input_f=7;
  int pir_val_f;
  int stat_f=LOW;
  int cont_mov_f=0;
  int cont_f_total=0;
  int cont_f_pos=0;
  
//Sound sensor
  int sound_1=A1; //analog input
  int value_sound;
void setup() {
////Configure back pins
pinMode(highpin_b, INPUT_PULLUP);
pinMode(medpin_b, INPUT_PULLUP);
pinMode(lowpin_b, INPUT_PULLUP);
//Configure front pins
pinMode(highpin_f, INPUT_PULLUP);
pinMode(lowpin_f, INPUT_PULLUP);
////Sound sensors
pinMode(sound_1,INPUT);
////distance_sonic
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
}

void loop() {
//Sound
  value_sound=analogRead(sound_1);
  Serial.print(value_sound); Serial.print(",");
  //Serial.print("Status: ");
  if(value_sound <= 10)
  {
    Serial.print("Quiet,");
  }
  else if( (value_sound > 10) && ( value_sound <= 30) )
  {
    Serial.print("Moderate,");
  }
  else if(value_sound > 30)
  {
    Serial.print("Loud,");
  }
//Distance sensor time to travel
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

//Ultrasonic
 distance_sonic = sonar.ping_cm();           //  Ask NewPing to trigger, fetch echo and calculate distance in cm
////   Prints the distance on the Serial Monitor
Serial.print(distance_sonic);Serial.print(",");
Serial.print(measure.RangeMilliMeter);Serial.print(",");
//  


// User Position

  if (distance_sonic <= 50 && distance_sonic != 0 &&measure.RangeMilliMeter >=500){
    //Serial.print("Position: ");
    Serial.print("Left,");
  }
  else if (measure.RangeMilliMeter <=500 && (distance_sonic >= 50 || distance_sonic==0)){
    //Serial.print("Position: "); 
    Serial.print("Right,");
  }
  else if (measure.RangeMilliMeter <=500 && distance_sonic!=0 && distance_sonic <= 50){
     //Serial.print("Position: "); 
     Serial.print("Center,");
  }
  else{
    // Serial.print("Position: "); 
     Serial.print("None,");    
  }

  

    
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
 Serial.print(cont_b_total);Serial.print(",");
 Serial.print(cont_b_pos);Serial.print(",");
 
//Front cabinet
  //Serial.print("Front Cabinet: ");
  if (digitalRead(highpin_f)==1){
    Serial.print("Up,");
    cont_f_pos=0;
  }
  else if (digitalRead(lowpin_f)==1){
    Serial.println("Down,");
    cont_f_pos=0;
  }

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
 //PIR sensor front
 Serial.print(cont_mov_f);Serial.print(",");
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
  Serial.print(cont_f_total);Serial.print(",");
  Serial.println(cont_f_pos);
   
  delay(200); 

}
