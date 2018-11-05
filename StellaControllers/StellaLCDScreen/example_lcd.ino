//WILL NEED TO TEST WITH SERVOS AND STEPPERS AS IF THEY WERE ARM OR MOTOR!!! DO IT ASAP DOOD.
//If anyone wants to mess with code and simplify it please do so! Thank you!
#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(6, 7, 5, 4, 3, 2);

// INITIAL VALUES
int j = 0;
int armLock = 0;
int motorLock = 0;



const int numOfScreens = 4; // total available of screens (subject to change)
int currentScreen = 0; // sets what current screen should be; based on array values
String screens[numOfScreens][2] = {{"Motor","Arm"},{"Leg","Head"},{"Arm","Unlocked"},{"Motor","Running"}}; //array of 0,1,2,3


// BUTTONS
const int numOfInputs = 4;  // # of buttons
const int inputPins[numOfInputs] = {8, 9, 10, 11}; // buttons attached on arduino
int inputState[numOfInputs];
int lastInputState[numOfInputs] = {LOW, LOW, LOW, LOW};
bool inputFlags[numOfInputs] = {LOW, LOW, LOW, LOW};
long lastDebounceTime[numOfInputs] = {0, 0, 0, 0};
long debounceDelay = 5;


void setup() {


lcd.clear();


  for (int i = 0; i < numOfInputs; i++) { // initialize inputs
    pinMode(inputPins[i], INPUT);
    digitalWrite(inputPins[i], HIGH);      // pull-up 20k
  }
  lcd.begin(16, 2); // lcd screen of 16 charc and 2 lines
  lcd.print("hello, select");
  lcd.setCursor(0, 1);
  lcd.print("Motor or Arm");
}
void loop() {
  setInputFlags();
  resolveInputFlags();


}


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




void inputAction(int input) {
  if (input == 0) { // change arrow cursor "up"
    if(0 <= currentScreen <= 1){
    if (j == 0) {
      j = 1;
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
   if(0 <= currentScreen <= 1){
    if (j == 1) {
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
    if (currentScreen == 0) {
      if( j == 0){ // locks Motors while unlocking arm controllability
        lcd.clear();
        currentScreen = 2; // message arm SHOULD be able to be moved (add check?)
        armLock = 1;
        motorLock = 0; 
      }
      if(j == 1){ // locks arm movement unlocks motors
        lcd.clear();
        currentScreen = 3; // message motor SHOULD be running (add check?)
        motorLock = 1;
        armLock = 0;
      }
  } 
}
  
  else if (input == 3) { // Restart; lock everything with reset or keep as is?
  if(2 <= currentScreen <= 3){
    j = 0;
    currentScreen = 0;
    lcd.clear();
    lcd.setCursor(0,j);
    lcd.print(">");
    
    
  }
  }
}

void printScreen() {
  //lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(1,1);
  lcd.print(screens[currentScreen][1]);
  

}
