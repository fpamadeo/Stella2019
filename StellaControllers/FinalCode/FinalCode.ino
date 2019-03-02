#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#include <SPI.h>  
#include <LiquidCrystal.h>

#define MECHS 4 //Number of Mechanisms: 0 = Shooter, 1 = Grabber, 2 = Foam Balls, 3 = Pit Balls

//LCD SCREEN:
LiquidCrystal lcd(25, 23, 30, 31, 29, 27, 28, 26, 24, 22);
#define MENU_ITEMS 10 //Shooter - Grabber - FoamBalls - ShooterOnOff - Grab -  ReleasePit - ReleaseOneFoam - ReleaseAllFoam - LockAll - UnlockAll
String screens[MENU_ITEMS] = {"LOCK Shooter", "LOCK Grabber", "LOCK Foam Balls", "Turn on Shooter", "Grab now", "Release Pit Balls", "Release 1 Foam Ball", "Release Foam Balls", "Lock All", "Unlock All"};

//RF24:
  int   key = 0; //"Hash key" of the selected option; 
                 //KEYS: Shooter - Grabber - PitBalls - FoamBalls - All

byte addresses[][6] = {("0")};
RF24 controller (7,8);


//Joysticks:
//Unneeded as of now, copy from the previous versions if needed

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
long debounceDelay = 10;

//Other global variables:
bool readJS = false; // True when grabber is unlocked
bool selected = false; // True when select is pressed
bool releaseFoam = false; // True when TS1 is ACTIVE and we want rapidfire shooting
int curr = 1; // current menu item 

//Making User move cursor first before selecting options for neatness and consistancy
boolean selectFIRST = 0;

//SCREEN
//OUTPUTS WORDS
int currentScreen = 1; // sets what current screen should be; based on array values
const int choiceLimit = 9;
int upScreen = 0; // top option "screen moves"
int downScreen = 0; //bottom option "screen moves"


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
}

//MAIN:
void loop() {
  // put your main code here, to run repeatedly:
  
  setInputFlags();
  resolveInputFlags();
  
  controller.write(key, sizeof(key));//Sending package to robot
  delay(10);
}

//Helper Functions:
//Refer to prev JS comment
//void joystick(){
//}
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
    if(currentScreen <= 9){ // Should ONLY work in array 0 to 1 as this is the menu!
      selectFIRST = 1;   
        if (currentScreen <= 0) {
            currentScreen = choiceLimit;
        }
            else if (currentScreen > 0){
              currentScreen--;  
            } 
      }
   } 
  
  else if (input == 1) { // change arrow cursor "down"
   if(currentScreen <= 9){ // should ONLY work in array 0 to 1 as those are the current menu options
    selectFIRST = 1;
    
      
      if (currentScreen >= choiceLimit) {
          currentScreen = 0;
                              }
          else if(currentScreen < choiceLimit){
           
            currentScreen++;
            
          }
     
    
  }
} 
  
  
  else if (input == 2) { // DONT PUSH THIS YET 
    if (selectFIRST == 1){
      
      }
   } 
 
  else if (input == 3) { // Restart; lock everything with reset or keep as is?
    if(selectFIRST == 1){
  
    currentScreen = 0;
  
    
  }  
  }
  
}


//Printing the screen on the LCD:
void printScreen() {
  //lcd.clear();
  lcd.clear();

  //CURSOR
  lcd.setCursor(0,1);
  lcd.print(">");

  //1st line
  if(currentScreen - 1 < 0){
    upScreen = 9;
  }
  else {
    upScreen = currentScreen - 1;
  }
  lcd.setCursor(1,0);
  lcd.print(screens[upScreen]);

  //2nd line
  lcd.setCursor(1,1);
  lcd.print(screens[currentScreen]);
  
  //3rd line:
  if (currentScreen + 1 > 9){
    downScreen = 0;
  }
  else {
    downScreen = currentScreen + 1;
  }
  lcd.setCursor(1,2);
  lcd.print(screens[downScreen]);

  //4th line
  lcd.setCursor(0,3);
  lcd.print("STATUS:");
  //lcd.print(currentStat[currentScreen]);
}

/*
//CURRENT OPTION IS SELECTED:
void checkCurr(){
    switch(curr){
      case 0: //Shooter
        toSend.key = 0; //Hash Key
        if (toSend.locked[0]){ //If locked,
          toSend.locked[0] = false; //unlock it
        }
        else{ //If unlocked,
          toSend.locked[0] = true; //lock it
        }
        break;
      case 1: //Grabber
        toSend.key = 1; //Hash Key
        if (toSend.locked[1]){ //If locked,
          toSend.locked[1] = false; //unlock it
          readJS = true; //Read the joysticks
        }
        else{ //If unlocked,
          toSend.locked[1] = true; //lock it
          readJS = false; //Read the joysticks
        }
        break;
      case 2: //Pit Balls
        toSend.key = 2; //Hash Key
        if (toSend.locked[2]){ //If locked,
          toSend.locked[2] = false; //unlock it
        }
        else{ //If unlocked,
          toSend.locked[2] = true; //lock it
        }
        break;
      case 3: //Release Pit Balls
        toSend.key = 2; //Hash Key
        if (not toSend.locked[2]){ //If unlocked,
          if (toSend.state[2] = 0){
            toSend.state[2] = 1; //Open the gate
          }
          else{
             toSend.state[2] = 0; //... or Close the gate
          }
        }
        break;
      case 4: // Release 1 foam ball
        toSend.key = 3; //Hash Key
        if (not toSend.locked[0]){ //If shooter is unlocked,
          if (toSend.state[3] = 0){
            toSend.state[3] = 2; //Release 1
          }
        }
        break;
      case 5: //Lock all
        toSend.locked[0] = true;
        toSend.locked[1] = true;
        toSend.locked[2] = true;
        break;
      case 6: //Unock all
        toSend.locked[0] = false;
        toSend.locked[1] = false;
        toSend.locked[2] = false;
        break;
          
      default:
        //*TESTING*
        Serial.print("\nERROR: SWITCH CASE in checkFlags()\n"); //Should not even get here
    
    }
}
*/
