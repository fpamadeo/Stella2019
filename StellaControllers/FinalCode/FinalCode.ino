
#include <LiquidCrystal.h>
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

//Buttons:
#define PB1 38 //UP
#define PB2 39 //DOWN
#define PB3 41 //SELECT
#define TB1 40 //PREV MENU
#define TB2 42 //NEXT MENU
#define TS1 25 //TURN ON SHOOTER
#define numOfInputs  6  // # of buttons
const int inputPins[numOfInputs] = {PB1, PB2, PB3, TB1, TB2, TS1}; // buttons attached on arduino
int inputState[numOfInputs];
int lastInputState[numOfInputs] = {LOW, LOW, LOW, LOW, LOW, LOW};
bool inputFlags[numOfInputs] = {LOW, LOW, LOW, LOW, LOW, LOW};
long lastDebounceTime[numOfInputs] = {0, 0, 0, 0, 0,0};
long debounceDelay = 5;

//LCD SCREEN:
LiquidCrystal lcd(25, 23, 30, 31, 29, 27, 28, 26, 24, 22);
#define MENU_ITEMS 7 //Shooter - Grabber - FoamBalls - ReleasePit - ReleaseFoam - LockAll - UnlockAll
String menu[MENU_ITEMS] = {"Shooter", "Grabber", "Foam Balls", "Release Pit Balls", "Release 1 Foam Ball", "Lock All", "Unlock All"};

//Other global variables:
bool readJS = false; // True when grabber is unlocked
bool selected = false; // True when select is pressed
bool releaseFoam = false; // True when TS1 is ACTIVE and we want rapidfire shooting
int curr = 1; // current menu item 

void setup() {  
  
  //For Testing #TODO# (Comment out after testing)
  Serial.begin(9600);
  
  //Joysticks: //Joystick has no button
  /*pinMode(leftSW_pin, INPUT);
  digitalWrite(leftSW_pin, HIGH);
  pinMode(rightSW_pin, INPUT);
  digitalWrite(rightSW_pin, HIGH);
  */
  
  //LCD Screen:
  lcd.clear();
  lcd.begin(20,4);
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
  
  //Push Buttons/Tactile Switches
  for (int i = 0; i < numOfInputs; i++) { // initialize inputs
    pinMode(inputPins[i], INPUT);
    digitalWrite(inputPins[i], HIGH);      // pull-up 20k
  }
  
  //TESTING: #TODO#
  Serial.print("setup done\n");
  delay(500);
  printScreen();
  lcd.setCursor(1,1);/*
  lcd.write("STELLA 2019         ");
  lcd.write("                    ");
  lcd.write(">                   ");
  lcd.write("                    ");
  */
  lcd.clear();
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

//SETTING UP INPUTS/BUTTONS FLAGS
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
  
  //Serial.println("HERE AT SetInputFLAGS");
}

//CHECKS THE FLAGS SETUP BY THE PREV FUNCTION (Helper Function: inputAction(int*))
void checkFlags(){
  //Serial.println("HERE AT CHECKFLAGS");
  for (int i = 0; i < numOfInputs; i++) {
    //Serial.print("HERE INSIDE CHECKFLAGS ");
    //Serial.println(i);
    if (inputFlags[i] == HIGH) {
      Serial.print("HERE INSIDE CHECKFLAGS = HIGH");
      inputAction(i); // flags for buttons, sets up buttons for use
      Serial.println(curr);
      inputFlags[i] = LOW;
      printScreen(); // last thing done
    }
  }
}

//Helper Function: look at the flags
void inputAction(int input) {
  switch(input){
    case 0: //UP
      
      if(curr == 0){
        curr = MENU_ITEMS;
      }
      else{
        curr -= 1;
      }
      break;
    case 1: //DOWN
      if(curr == MENU_ITEMS - 1){
        curr = 0;
      }
      else{
        curr += 1;
      }
      break;
    case 2: //SELECT
      selected = true;
      break;
    case 3: //PREV
      curr -= 3;
      if(curr < 0){
        curr += MENU_ITEMS;
      }
      break;
    case 4:
      curr += 3;
      if(curr >= MENU_ITEMS){
        curr -= MENU_ITEMS;
      }
      break;
    case 5:
      if(releaseFoam){
        releaseFoam = false;
      }
      else{
        releaseFoam = true;
      }
      break;
    default:
      //*TESTING*
      Serial.print("\nERROR: SWITCH CASE in checkFlags()\n"); //Should not even get here
  }
  
}

//Helper Function: Printing the screen on the LCD:
void printScreen() {
  lcd.clear();
  //Previous:
  lcd.setCursor(0,1);
  Serial.print("\n ");
  if(curr ==0){
    lcd.print(menu[MENU_ITEMS-1]);
    Serial.println(menu[MENU_ITEMS-1]);
  }
  else{
    lcd.print(menu[curr-1]);
    Serial.println(menu[curr-1]);
  }
  lcd.setCursor(0,2);
  lcd.print(">");
  Serial.print(">");
  lcd.print(menu[curr]);
  Serial.println(menu[curr]);
  lcd.setCursor(0,3);
  Serial.print(" ");
  if(curr == MENU_ITEMS-1){
    lcd.print(menu[0]);
    Serial.println(menu[0]);
  } 
  else{
    lcd.print(menu[curr+1]);
    Serial.println(menu[curr+1]);
  }
}

//CURRENT OPTION IS SELECTED:
void checkCurr(){
  if checkCurr(){
    
  }
}

//MAIN:
void loop() {
  // put your main code here, to run repeatedly:
  
  setInputFlags();
  if(readJS){
    joystick();
  }
  checkFlags(); //And Print the screen
  if(selected){
    checkCurr();
  }
  delay(500);
}
