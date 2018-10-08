/*
 Short Description:
 Takes the analog (x- and y-axises) and digital (switch) data and converting it to a -100 to 100 for analog, 
 and 1/0 for the digital. 

 For representation: this outputs it to the serial monitor and to LEDs
 */

// Arduino pin numbers
// Left Analog Stick
const int leftSW_pin = 2;  // digital pin connected to Left switch output //0 if pressed and 1 if not
const int leftX_pin = A0;  // analog pin connected to Left X output //Left: 0; Mid:~492; Right:~1008
const int leftY_pin = A1;  // analog pin connected to Left Y output //down: 0; Mid:~503; Up:~1006
// Right Analog Stick
const int rightSW_pin = 3; // digital pin connected to Right switch output //0 if pressed and 1 if not
const int rightX_pin = A2; // analog pin connected to Right X output //Left: 0; Mid:~492; Right:~1010 
const int rightY_pin = A3; // analog pin connected to Right Y output //down: 0; Mid:~510; Up:~1019
// LED pin numbers
const int upL = 11;        //LED representing pos y axis
const int leftL = 10;      //LED representing neg x axis
const int downL = 9;       //LED representing neg y axis
const int rightL = 8;      //LED representing pos x axis

const int upR = 7;         //LED representing pos y axis
const int leftR = 6;       //LED representing neg x axis
const int downR = 5;       //LED representing neg y axis
const int rightR = 4;      //LED representing pos x axis


//(To test) values of x and y:
const int leftX = 492;
const int leftY = 503;
const int rightX = 492;
const int rightY = 510;

void setup() {
  pinMode(leftSW_pin, INPUT);
  digitalWrite(leftSW_pin, HIGH);
  pinMode(rightSW_pin, INPUT);
  digitalWrite(rightSW_pin, HIGH);
  Serial.begin(9600);
}

void loop() {
  //Left Joystick
  int switchL = digitalRead(leftSW_pin);
  int xaxL = (analogRead(leftX_pin) - (leftX))/5;
  int yaxL = (analogRead(leftY_pin) - (rightX))/5;
  
  //Right Joystick
  int switchR = digitalRead(rightSW_pin);
  int xaxR = (analogRead(rightX_pin) - (leftY))/5;
  int yaxR = (analogRead(rightY_pin) - (rightY))/5;

  //Cancelling negative Noise:
  if(xaxL < -100){
    xaxL = -100;
  }
  if(xaxR < -100){
    xaxR = -100;
  }
  if(yaxL < -100){
    yaxL = -100;
  }
  if(yaxR < -100){
    yaxR = -100;
  }

  //Canceling positive noise:
  if(xaxL > 100){
    xaxL = 100;
  }
  if(xaxR > 100){
    xaxR = 100;
  }
  if(yaxL > 100){
    yaxL = 100;
  }
  if(yaxR > 100){
    yaxR = 100;
  }

  //Cancelling idle noise:
  if(xaxL > 2 || xaxL < -2){
    xaxL = 0;
  }
  if(xaxR > 2 || xaxR < -2){
    xaxR = 0;
  }
  if(yaxL > 2 || yaxL < -2){
    yaxL = 0;
  }
  if(yaxR > 2 || yaxR < -2){
    yaxR = 0;
  }

  //Powering Up LEDs
  //X-axises - Powers up the corresponding LEDs by how hard you move the joysticks
  if (xaxL < 0){
    analogWrite(leftL, xaxL * -2.55);  
  }
  else{
    analogWrite(rightL, xaxL * 2.55);  
  }
  if (xaxR < 0){
    analogWrite(leftR, xaxR * -2.55);  
  }
  else{
    analogWrite(rightR, xaxR * 2.55);  
  }

  //Y-axises - Powers up the corresponding LEDs by how hard you move the joysticks
  if (yaxL < 0){
    analogWrite(downL, yaxL * -2.55);  
  }
  else{
    analogWrite(upL, yaxL * 2.55);  
  }
  if (yaxR < 0){
    analogWrite(downR, yaxR * -2.55);  
  }
  else{
    analogWrite(upR, yaxR * 2.55);  
  }

  //Switches - Powers up all (L/R) the LEDs when pressed
  if(switchL == 0){
    analogWrite(downL, 255);
    analogWrite(upL, 255);
    analogWrite(leftL, 255);
    analogWrite(rightL, 255);
  }
  
  if(switchR == 0){
    analogWrite(downR, 255);
    analogWrite(upR, 255);
    analogWrite(leftR, 255);
    analogWrite(rightR, 255);
  }
  /*
  //Serial Monitor printing (for debug/testing)
  Serial.print("Left Switch:  ");
  Serial.print(switchL);
  Serial.print("\n");
  Serial.print("Right Switch:  ");
  Serial.print(switchR);
  Serial.print("\n");
  
  Serial.print("Left X-axis: ");
  Serial.print(xaxL);
  Serial.print("\n");
  Serial.print("Right X-axis: ");
  Serial.print(xaxR);
  Serial.print("\n");
  
  Serial.print("Left Y-axis: ");
  Serial.println(yaxL);
  Serial.print("\n");
  Serial.print("Right Y-axis: ");
  Serial.println(yaxR);
  Serial.print("\n\n");
  */
  delay(750);
}
