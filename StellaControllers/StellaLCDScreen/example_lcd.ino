#include <Servo.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(6, 7, 5, 4, 3, 2);

int j = 0;
int armLock = 0;
int motorLock = 0;



const int numOfScreens = 4;
int currentScreen = 0;
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
      if( j == 0){
        lcd.clear();
        currentScreen = 2;
        armLock = 1; 
      }
      if(j == 1){
        lcd.clear();
        currentScreen = 3;
        motorLock = 1;
      }
  } 
}
  
  else if (input == 3) { // Restart??
  
  }
}

void printScreen() {
  //lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(1,1);
  lcd.print(screens[currentScreen][1]);
  

}
