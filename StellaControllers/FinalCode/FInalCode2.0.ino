//BUTTONS WORK, RF INCLUDED, NEED TO ADD FUNCTIONS FOR TESTING WILL DO SOON, ALL VARIABLES SHOULD BE INCLUDED (at least joysticks)
//Changes in printScreen, string for screen, Check current not added (plan to put that in InputAction, maybe)
//POSSIBLE PROBLEM when adding CheckCurr, also with void print screen function from previous code. 
#include <SPI.h>  
#include "RF24.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(25, 23, 30, 31, 29, 27, 28, 26, 24, 22);

//RF24:
typedef struct package
{
  int   state = 0; //State of the selected option: 0 = OFF; 1 = ON; 2 = SINGLE SHOT 
  int   key = 0; //"Hash key" of the selected option
  double xAxis = 0.0; //yAxis value from the joysticks
  double yAxis = 0.0; //xAxis value from the joysticks
  bool  locked = true; //True until we want to lock
} pkg; 
byte addresses[][6] = {"0"};
pkg toSend;
RF24 controller (7,8);

//Joysticks:
// Left Analog Stick
//const int leftSW_pin = 2; // digital pin connected to Left switch output //0 if pressed and 1 if not
#define leftX_pin A2   // analog pin connected to Left X output //Left: 0; Mid:~492; Right:~1008
#define leftY_pin A3   // analog pin connected to Left Y output //down: 0; Mid:~503; Up:~1006
// Right Analog Stick
//const int rightSW_pin = 3;// digital pin connected to Right switch output //0 if pressed and 1 if not
#define rightX_pin A0  // analog pin connected to Right X output //Left: 0; Mid:~492; Right:~1010 
#define rightY_pin A1  // analog pin connected to Right Y output //down: 0; Mid:~510; Up:~1019
//Left values
double leftLR = 0;   //value representing x axis
double leftUD = 0;   //value representing y axis
//right values
double rightLR = 0;   //value representing x axis
double rightUD = 0;   //value representing y axis

//#TODO# values of x and y:
#define leftX 507.0
#define leftY 523.00
#define rightX 507
#define rightY 500

// INITIAL VALUES
int j = 0;


//Making User move cursor first before selecting options for neatness and consistancy
boolean selectFIRST = 0;


const int numOfScreens = 4; // total available of screens (subject to change)
int currentScreen = 0; // sets what current screen should be; based on array values
String screens[numOfScreens][4] = {{"Shooter","Grabber","Foam Balls","Release Foam Balls"},{"Release 1 Foam Ball","Lock all","Unlock all"}}; //array of 0,1,2,3


// BUTTONS
//Buttons:
const int numOfInputs = 6;
const int inputPins[numOfInputs] = {38, 39, 41, 40, 42, 25}; // buttons attached on arduino
int inputState[numOfInputs];
int lastInputState[numOfInputs] = {LOW, LOW, LOW, LOW, LOW, LOW};
bool inputFlags[numOfInputs] = {LOW, LOW, LOW, LOW, LOW, LOW};
long lastDebounceTime[numOfInputs] = {0, 0, 0, 0, 0,0};
long debounceDelay = 5;

//SETUP (Redundant I know)
void setup() {

//RF24
controller.begin();
controller.setChannel(115);
controller.setPALevel(RF24_PA_MAX);
controller.setDataRate( RF24_250KBPS ) ; 
controller.openWritingPipe( addresses[0]);
delay(1000);

lcd.clear();

  //Push Buttons/Tactile Switches
  for (int i = 0; i < numOfInputs; i++) { // initialize inputs
    pinMode(inputPins[i], INPUT);
    digitalWrite(inputPins[i], HIGH);      // pull-up 20k
  }
  lcd.begin(20, 4); // lcd screen of 16 charc and 2 lines
  //Initial Screen:
  lcd.setCursor(1,0);
  lcd.print(".-.._.._..  .  .-.");
  Serial.println(".-.._.._..  .  .-.");
  lcd.setCursor(1,1);      
  lcd.print("`-. | |- |  |  |-|");       
  Serial.println("`-. | |- |  |  |-|");       
  lcd.setCursor(1,2);
  lcd.print("`-' ' `-'`-'`-'` '");        
  Serial.println("`-' ' `-'`-'`-'` '");        
  lcd.setCursor(8,3);
  lcd.print("2019       ");
  Serial.println("       2019       ");
  
  //TESTING: #TODO#
  /*
  Serial.print("setup done\n");
  delay(500);
  
  lcd.setCursor(1,1);/*
  lcd.write("STELLA 2019         ");
  lcd.write("                    ");
  lcd.write(">                   ");
  lcd.write("                    ");
  */
}

void joystick(){
  //Left Joystick
  leftLR = (analogRead(leftX_pin) - (leftX))/5.0;
  leftUD = (analogRead(leftY_pin) - (rightX))/5.0;
  
  //Right Joystick
  rightLR = (analogRead(rightX_pin) - (leftY))/5.0;
  rightUD = (analogRead(rightY_pin) - (rightY))/5.0;

  //Cancelling negative Noise:
  if(leftLR < -100){
    leftLR = -100;
  }
  if(rightLR < -100){
    rightLR = -100;
  }
  if(leftUD < -100){
    leftUD = -100;
  }
  if(rightUD < -100){
    rightUD = -100;
  }

  //Canceling positive noise:
  if(leftLR > 100){
    leftLR = 100;
  }
  if(leftUD > 100){
    leftUD = 100;
  }
  if(rightLR > 100){
    rightLR = 100;
  }
  if(rightUD > 100){
    rightUD = 100;
  }

  //Cancelling idle noise:
  if(leftLR < 5.0 && leftLR > -5.0){
    leftLR = 0.0;
  }
  if(leftUD < 5.0 && leftUD > -5.0){
    leftUD = 0.0;
  }
  if(rightLR < 5.0 && rightLR > -5.0){
    rightLR = 0.0;
  }
  if(rightUD < 5.0 && rightUD > -5.0){
    rightUD = 0.0;
  }
}

//RUN
void loop() {
  setInputFlags();
  resolveInputFlags();

}

//SETTING UP INPUTS/BUTTONS
void setInputFlags() { // checking for inputs of buttons
  for (int i = 0; i < numOfInputs; i++) {
    int reading = digitalRead(inputPins[i]);
    if (reading != lastInputState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != inputState[i]) {
        inputState[i] = reading;
        if (inputState[i] == LOW) { // buttons start out LOW not pressed!
          inputFlags[i] = HIGH;
        }
      }
    }
    lastInputState[i] = reading;
  }
}


//CHECKING AND PRINTING
void resolveInputFlags() {
  for (int i = 0; i < numOfInputs; i++) {
    if (inputFlags[i] == HIGH) {
      inputAction(i); // flags for buttons, sets up buttons for use
      Serial.println(currentScreen);
      inputFlags[i] = LOW;
      printScreen(); // last thing done
    }
  }
}



//BUTTON OPERATIONS
void inputAction(int input) {
  if (input == 0) { // change arrow cursor "up"
    if(currentScreen <= 1){ // Should ONLY work in array 0 to 1 as this is the menu!
      selectFIRST = 1;
    if (j == 0) {
      j = 3;
      lcd.clear();
      lcd.setCursor(0,j);
      lcd.print(">");
      if (currentScreen == 0) {
          currentScreen = numOfScreens-3;
      }
          else if (currentScreen != 0){
            currentScreen--;
          }
      
    } else {
      j--;
      lcd.clear();
      lcd.setCursor(0,j);
      lcd.print(">");
     
    }
  }
} 
  
  else if (input == 1) { // change arrow cursor "down"
   if(currentScreen <= 1){ // should ONLY work in array 0 to 1 as those are the current menu options
    selectFIRST = 1;
    if (j == 3) {
      j = 0;
      lcd.clear();
      lcd.setCursor(0,j);
      lcd.print(">");
      if (currentScreen == numOfScreens-3) {
          currentScreen = 0;
      }
          else if(currentScreen != numOfScreens-3){
            currentScreen++;
            
          }
    } 
    else {
      j++;
      lcd.clear();
      lcd.setCursor(0,j);
      lcd.print(">");
     
     
    }
  }
} 
  
  
  else if (input == 2) {
    if (selectFIRST == 1){
    if (currentScreen == 0) {
      if( j == 0){ // locks Motors while unlocking arm controllability
        lcd.clear();
        currentScreen = 3; // message arm SHOULD be able to be moved (add check?)
         
      }
      if(j == 1){ // locks arm movement unlocks motors
        lcd.clear();
        currentScreen = 2; // message motor SHOULD be running (add check?)
        
      }
   } 
 }
} 
  else if (input == 3) { // Restart; lock everything with reset or keep as is?
    if(selectFIRST == 1){
  if(2 <= currentScreen <= 3){
    j = 0;
    currentScreen = 0;
    lcd.clear();
    lcd.setCursor(0,j);
    lcd.print(">");
    
  }  
  }
  }
}




//PRINT
void printScreen() {
  //lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(1,1);
  lcd.print(screens[currentScreen][1]);
  lcd.setCursor(1,2);
  lcd.print(screens[currentScreen][2]);
  lcd.setCursor(1,3);
  lcd.print(screens[currentScreen][3]);
}
