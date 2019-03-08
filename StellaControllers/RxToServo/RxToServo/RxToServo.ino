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
Servo lr;
Servo ud;

//Channels
int ch1; // Here's where we'll keep our channel values
int ch2;
int ch3;

//Values
int lrval; // The value for the "Left/Right" Servo
int udval; // The value for the "Up/Down" Servo

//Pinouts

void setup() {
Serial.begin(9600);
pinMode(5, INPUT); // Set our input pins as such
pinMode(6, INPUT);
pinMode(7, INPUT);

lr.attach(10); 
ud.attach(11);

Serial.begin(9600); // Pour a bowl of Serial

}

void loop() {

  //Get pulse
  ch1 = pulseIn(5, HIGH, 25000); // Read the pulse width of 
  ch2 = pulseIn(6, HIGH, 25000); // each channel
  ch3 = pulseIn(7, HIGH, 25000);

Serial.print("ch1\n");
Serial.print(ch1);
lrval = map(ch1, 1000, 2000, 0, 179);
//Put pulse thru servo
lr.write(lrval);
udval = map(ch3, 1000, 2000, 0, 179);
ud.write(udval);
}
