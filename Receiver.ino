/*Libraries*/
#include <SPI.h>  //Built-in Serial-Peripherial Interface
#include <RF24.h>  //https://www.arduinolibraries.info/libraries/rf24
#include <nRF24L01.h>  //Part of RF24
#include "RoboClaw.h"
#include <Wire.h>

/*This program was runned using the nRF24L01 TRANSCEIVER...
 * If we wish to use the nRF24L01 + PA + LNA we will need to get a 3.3V voltage regulator that
 * can handle 250mA (per Arduino) unless we need more than three radios, then we should use a 
 * 3.3V voltage regulator rate at 800mA for overhead...
 */
 
#define address2 0x80
// Roboclaw Object
RoboClaw roboclaw(&Serial3, 10000); //Connecting TX, RX to roboclaw object

/* Global */
const byte address[6] = "00001"; // address for channel
unsigned long lastReceiveTime = 0;  // time of last received message
unsigned long currentTime = 0;  // current time

/* Instance */
RF24 radio(7, 8); //(digital pin CE, digital pin CSN) creates an instance of the rf24 driver

/*Data Structure*/
struct Data_Package {
  byte j1PotY;  // Holds the value from joystick 1
  byte j2PotY;  // Holds the value from joystick 2
};

Data_Package data; //Create a variable with the above structure

/*Setup function, only runs onces*/
void setup() {
  Serial.begin(9600);  //Communication with serial montiore(baudrate)
  Serial3.begin(38400);
  radio.begin();  //Communication with radio
  radio.openReadingPipe(0, address);  //Begin reading radio
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);  //Set power level
  radio.startListening();  //Start listening for channel (Slave)
  resetData(); //Reset data on setup()
}

void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotY = 127; //Set to middle
  data.j2PotY = 127; //Set to middle
}
void right(){
    /*Joystick Right Hand*/
    /*Middle: means roboclaw should have 0 (no movement)*/
    if(data.j2PotY >= 91 && data.j2PotY <= 191){
      data.j2PotY = 0;
      roboclaw.ForwardM2(address2, data.j2PotY);
      delay(10);
    }
    /*Middle/High: means roboclaw should have 64 (going forward at half speed)*/
    else if(data.j2PotY >= 192 && data.j2PotY <= 254){
      data.j2PotY = 64;
      roboclaw.ForwardM2(address2, data.j2PotY);
      delay(10);
    }
    /*High: means roboclaw should have 127 (going forward at full speed)*/
    else if(data.j2PotY == 255){
      data.j2PotY = 127;
      roboclaw.ForwardM2(address2, data.j2PotY);
      delay(10);
    }
    /*Middle/Low: means roboclaw should have 64 (going backwards at half speed)*/
    else if(data.j2PotY <= 90 && data.j2PotY >= 1){
      data.j2PotY = 64;
      roboclaw.BackwardM2(address2, data.j2PotY);
      delay(10);
    }
    /*Low: means roboclaw should have 127 (going backwards at full speed)*/
    else if(data.j2PotY == 0){
      data.j2PotY = 127;
      roboclaw.BackwardM2(address2, data.j2PotY);
      delay(10);
    }
    else{
      resetData();  //Reset data
    }
}
void left(){
    /*Joystick Left Hand*/
    /*Middle: Means roboclaw should have 0 (no movement)*/
    if(data.j1PotY >= 91 && data.j1PotY <= 191){
      data.j1PotY = 0;
      roboclaw.ForwardM1(address2, data.j1PotY);
      delay(10);
    }
    /*Middle/High: Means roboclaw should have 64 (going forward at half speed)*/
    else if(data.j1PotY >= 192 && data.j1PotY <= 254){
      data.j1PotY = 64;
      roboclaw.ForwardM1(address2, data.j1PotY);
      delay(10);
    }
    /*High: Means roboclaw should have 127 (going forward at full speed)*/
    else if(data.j1PotY == 255){
      data.j1PotY = 127;
      roboclaw.ForwardM1(address2, data.j1PotY);
      delay(10);
    }
    /*Middle/Low: Means roboclaw should have 64 (going backwards at half speed)*/
    else if(data.j1PotY <= 90 && data.j1PotY >= 1){
      data.j1PotY = 64;
      roboclaw.BackwardM1(address2, data.j1PotY);
      delay(10);
    }
    /*Low: means roboclaw should have 127 (going backwards at full speed)*/
    else if(data.j1PotY == 0){
      data.j1PotY = 127;
      roboclaw.BackwardM1(address2, data.j1PotY);
      delay(10);
    }
    else{
      resetData(); //Reset Data
    }
}
/*Loop is like main()*/
void loop() {
  // Check whether we keep receiving data, or we have a connection between the two modules
  currentTime = millis();
  if (currentTime - lastReceiveTime > 1000) {
    // If current time is more then 1 second since we have received the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior
  }
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); //Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // at this moment we have received the data
    }
    // Check whether we keep receiving data, or we have a connection between the two modules
    currentTime = millis();

    /*Call the motor controller algorithms*/
    left();  // Control left motor 
    delay(10);
    right(); // Control right motor
    delay(10);
    
    /*Table to Display Values on Serial Monitor*/
    Serial.print("j1PotY: ");
    Serial.print(data.j1PotY);
    Serial.print("\t");
    Serial.print("; j2PotY: ");
    Serial.println(data.j2PotY);
}
