/* 
 RC PulseIn Servos
 By: Matthew Macdonald-Wallace
 Date: 29/07/2015

 HEAVILY BASED ON:

 RC PulseIn Joystick
 By: Nick Poole
 SparkFun Electronics
 Date: 5
 License: CC-BY SA 3.0 - Creative commons share-alike 3.0
 use this code however you'd like, just keep this license and
 attribute. Let me know if you make hugely, awesome, great changes.
 
 Original Article can be found at https://www.sparkfun.com/tutorials/348
 */

#include <Servo.h>

//Servos
Servo foam;
Servo pit;

//Channels
int ch1; // Here's where we'll keep our channel values

//Values
int pulseVal; //The Mapped values from the rudder
int foamVal; // The value for the foam balls Servo
int pitVal; // The value for the Pit Balls Servo
#define threshold 30

//Pinouts
#define RUDDpin 5
#define foamPin 10
#define pitPin 11

void setup() {
Serial.begin(9600);
pinMode(RUDDpin, INPUT); // Set our input pins as such

foam.attach(foamPin); 
pit.attach(pitPin);

foam.write(0);
pit.write(0);

Serial.begin(9600); // Pour a bowl of Serial

}

void loop() {

  //Get pulse
  ch1 = pulseIn(RUDDpin, HIGH, 25000); // Read the pulse width of rudder
  
  Serial.print("\n\nch1: ");
  Serial.print(ch1);
  pulseVal = map(ch1, 1000, 2000, -179, 179); //will get a pulse between -180 and 180
  Serial.print("\nMapped:");
  Serial.print(pulseVal);

  //pulse dissection:
  if(pulseVal < (-1 * threshold)){
    if (foamVal < 175) { // goes from 180 degrees to 0 degrees
      foamVal += 25;
      Serial.print("\nFoam Value increased to:");              // tell servo to go to position in variable 'pos'
      Serial.print(foamVal);              // tell servo to go to position in variable 'pos'  
    }
    if (pitVal > 2) { // goes from 180 degrees to 0 degrees
      pitVal -= 25;
      Serial.print("\nPit Value decreased to:");              // tell servo to go to position in variable 'pos'
      Serial.print(pitVal);              // tell servo to go to position in variable 'pos'  
    }
    Serial.print("\nFoam Value: ");
    Serial.print(foamVal);
  }
  else if(pulseVal > threshold){
    if (foamVal > 2) { // goes from 180 degrees to 0 degrees
      foamVal -= 25;
      Serial.print("\nFoam Value increased to:");              // tell servo to go to position in variable 'pos'
      Serial.print(foamVal);              // tell servo to go to position in variable 'pos'  
    }
    if (pitVal < 175) { // goes from 180 degrees to 0 degrees
      pitVal += 25;
      Serial.print("\nPit Value decreased to:");              // tell servo to go to position in variable 'pos'
      Serial.print(pitVal);              // tell servo to go to position in variable 'pos'  
    }
    if(pitVal >= 175){
      pitVal = 175;
    }
    Serial.print("\nPit Value: ");
    Serial.print(pitVal);
  }
  else{
    if (foamVal > 2) { // goes from 180 degrees to 0 degrees
      foamVal -= 25;
      Serial.print("\nFoam Value decreased to:");              // tell servo to go to position in variable 'pos'
      Serial.print(foamVal);              // tell servo to go to position in variable 'pos'  
    }
    if (pitVal > 2) { // goes from 180 degrees to 0 degrees
      pitVal -= 25;
      Serial.print("\nPit Value decreased to:");              // tell servo to go to position in variable 'pos'
      Serial.print(pitVal);              // tell servo to go to position in variable 'pos'  
    }
  }
  //Put pulses thru servos
  foam.write(foamVal);
  pit.write(pitVal);
  delay(500);
}
