/*Libraries*/
#include <SPI.h>  //Built-in Serial-Peripherial Interface
#include <RF24.h>  //https://www.arduinolibraries.info/libraries/rf24
#include <nRF24L01.h>  //Part of RF24

/*This program was runned using the nRF24L01 TRANSCEIVER...
 * If we wish to use the nRF24L01 + PA + LNA we will need to get a 3.3V voltage regulator that
 * can handle 250mA (per Arduino) unless we need more than three radios, then we should use a 
 * 3.3V voltage regulator rate at 800mA for overhead...
 */
 
/* Global */
const byte address[6] = "00001"; // address for channel

/* Instance */
RF24 radio(7, 8); //(digital pin CE, digital pin CSN) creates an instance of the rf24 driver

/*Data Structure*/
struct Data_Package {
  byte j1PotY;  // Holds the value from joystick 1
  byte j2PotY;  // Holds the value from joystick 2
};

Data_Package data; //Create a variable with above structure

/*Setup function, only runs onces*/
void setup() {
  Serial.begin(9600);
  radio.begin();  //Communication with radio
  radio.openWritingPipe(address);  //Begin writing radio
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);  //Set power level
  radio.stopListening();  //Stop listening for channel (Master)
  
  data.j1PotY = 127; //Set to middle
  data.j2PotY = 127; //Set to middle
}

/*Loop is like main()*/
void loop() {
  /*Joystick Controls and Mapping
  *We have to map the values that are originally 0 to 1023 to 0 to 255
  *This is because the resolution of the RF transmitter is one byte
  */
  data.j1PotY = map(analogRead(A0), 0, 1023, 0, 255);  //Map joystick 1
  data.j2PotY = map(analogRead(A1), 0, 1023, 0, 255);  //Map joystick 2

  /*Print table to Serial Monitor*/
  Serial.print("j1PotY: ");
  Serial.print(data.j1PotY);
  Serial.print("\t");
  Serial.print("; j2PotY: ");
  Serial.println(data.j2PotY);
  radio.write(&data, sizeof(Data_Package));  //Send package to receiver!
}
