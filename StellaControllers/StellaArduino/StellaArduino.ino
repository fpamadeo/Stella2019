#include <SPI.h>  
#include "RF24.h" 
#include <Servo.h>

#define MECHS 4 //Number of Mechanisms: 0 = Shooter, 1 = Grabber, 2 = Foam Balls, 3 = Pit Balls
//Servos
Servo servo0;
Servo servo1;
int pos0=0;
int pos1=0;

//RF24:
int key = 0;
byte addresses[][6] = {("0")};
RF24 Stella (8,7);


void setup() 
{
  Serial.begin(9600);
  delay(1000);

  Stella.begin(); 
  Stella.setChannel(115); 
  Stella.setPALevel(RF24_PA_MAX);
  Stella.setDataRate(RF24_250KBPS); 
  Stella.openReadingPipe(1, addresses[0]);
  Stella.startListening();
  servo0.attach(9);
  servo1.attach(10);
}


void loop()  
{

  if ( Stella.available()) 
  {
    Serial.println("Controller Found");
    while (Stella.available())
    {
      Stella.read(&key, sizeof(key) );
      switch(key){
        case 0:
          //Shooter
          //Notes: Only needs DC motor in 1 direction
          
          break;
        case 1:
          //Grabber
          //Notes: DC motor to go in both linear directions
          
          break;
        case 2:
          //Foam Balls
          //Servo connected to pin 9
          for (pos0=0; pos0<=90; pos0+=30){
            servo0.write(pos0);
            delay(15);
          }
          for (pos0=90; pos0>=0; pos0-=30){
            servo0.write(pos0);
            delay(15);
          }
          break;
        case 3:
          //Pit Balls
          //Servo connected to pin 10
          for (pos1=0; pos1<=90; pos1+=30){
            servo1.write(pos1);
            delay(15);
          }
          for (pos1=90; pos1>=0; pos1-=30){
            servo1.write(pos1);
            delay(15);
          }
          
          break;
        default:
          //No Default so just break
          break;
        
      }
      
    }
  }
  else{
    Serial.println("Controller Not Found");
  }

}
