/*
 Short Description:
 Takes the analog (x- and y-axises) and digital (switch) data and converting it to a -100 to 100 for analog, 
 and 1/0 for the digital. 

 For representation: this outputs it to the serial monitor and to LEDs
 */

// Arduino pin numbers
// Left Analog Stick
const int leftSW_pin = 2;  // digital pin connected to Left switch output //0 if pressed and 1 if not
const int leftX_pin = A2;  // analog pin connected to Left X output //Left: 0; Mid:~492; Right:~1008
const int leftY_pin = A3;  // analog pin connected to Left Y output //down: 0; Mid:~503; Up:~1006
// Right Analog Stick
const int rightSW_pin = 3; // digital pin connected to Right switch output //0 if pressed and 1 if not
const int rightX_pin = A0; // analog pin connected to Right X output //Left: 0; Mid:~492; Right:~1010 
const int rightY_pin = A1; // analog pin connected to Right Y output //down: 0; Mid:~510; Up:~1019

// LED pin numbers
const int up = 6;        //LED representing neg x axis
const int left = 9;          //LED representing pos y axis
const int right = 10;       //LED representing neg y axis
const int down = 11;      //LED representing pos x axis

// LED powerValue
double powerL = 0;        //LED representing neg x axis
double powerU = 0;          //LED representing pos y axis
double powerD = 0;       //LED representing neg y axis
double powerR = 0;      //LED representing pos x axis

//To select which Joystick to look at:
int whichStick = 0; //0 if LJ or 1 if RJ

//(To test) values of x and y:
const double leftX = 507.0;
const double leftY = 523.00;
const double rightX = 507;
const double rightY = 500;

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
  double xaxL = (analogRead(leftX_pin) - (leftX))/5.0;
  double yaxL = (analogRead(leftY_pin) - (rightX))/5.0;
  
  //Right Joystick
  double switchR = digitalRead(rightSW_pin);
  double xaxR = (analogRead(rightX_pin) - (leftY))/5.0;
  double yaxR = (analogRead(rightY_pin) - (rightY))/5.0;

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
  if(xaxL < 5.0 && xaxL > -5.0){
    xaxL = 0.0;
  }
  if(xaxR < 5.0 && xaxR > -5.0){
    xaxR = 0.0;
  }
  if(yaxL < 5.0 && yaxL > -5.0){
    yaxL = 0.0;
  }
  if(yaxR < 5.0 && yaxR > -5.0){
    yaxR = 0.0;
  }

  //Switches - Selects which (L/R) the LEDs when pressed (L have precedence)
  if(switchL == 0){
    whichStick = 0;
  analogWrite(left, 255);
  analogWrite(right, 255);
  analogWrite(up, 255);
  analogWrite(down, 255);
  delay(500);
  }
  
  if(switchR == 0){
    whichStick = 1;
  analogWrite(left, 255);
  analogWrite(right, 255);
  analogWrite(up, 255);
  analogWrite(down, 255);
  delay(500);
  }
  
  //Powering Up LEDs
  //If looking at Left Joystick 
  if(whichStick == 0){
    //When Joystick is idle, decrement the power
    if(xaxL < 0.1 && xaxL > -0.1 ){
      powerL -= 2.5;
      powerR -= 2.5;
    }
    if(yaxL < 0.1 && yaxL > -0.1){
      powerD -= 2.5;
      powerU -= 2.5;
    }
    //x-axis
    if(xaxL < 0){
      powerL += xaxL * -0.05;
    }
    else{
      powerR += xaxL * 0.05;
    }
    //y-axis
    if(yaxL < 0){
      powerD += yaxL * -0.05;
    }
    else{
      powerU += yaxL * 0.05;
    }
  }
  
  //If Right Joystick is being look at
  else{
    //When Joystick is idle, decrement the power 
    if(xaxR < 0.1 && xaxR > -0.1){
      powerL -= 2.5;
      powerR -= 2.5;
    }
    if(yaxR < 0.1 && yaxR > -0.1){
      powerU -= 2.5;
      powerD -= 2.5;
    }
    //x-axis
    if(xaxR < 0){
      powerL += xaxR * -0.05;
    }
    else{
      powerR += xaxR * 0.05;
    }
    //y-axis
    if(yaxR < 0){
      powerD += yaxR * -0.05;
    }
    else{
      powerU += yaxR * 0.05;
    }
  }

  //Handling overflow:
  if(powerU > 255.0){
    powerU = 255.0;
  }
  if(powerD > 255.0){
    powerD = 255.0;
  }
  if(powerL > 255.0){
    powerL = 255.0;
  }
  if(powerR > 255.0){
    powerR = 255.0;
  }
  if(powerU < 0){
    powerU = 0;
  }
  if(powerD < 0){
    powerD = 0;
  }  
  if(powerL < 0){
    powerL = 0;
  }  
  if(powerR < 0){
    powerR = 0;
  }

  analogWrite(left, powerL);
  analogWrite(right, powerR);
  analogWrite(up, powerU);
  analogWrite(down, powerD);
  
  //Serial Monitor printing (for debug/testing)
  /*Serial.print("Left Switch:  ");
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

  Serial.print("\n\nPower L: ");
  Serial.print(powerL);
  Serial.print("\nPower R: ");
  Serial.print(powerR);
  Serial.print("\nPower D: ");
  Serial.print(powerD);
  Serial.print("\nPower U: ");
  Serial.print(powerU);
  delay(750);
}
