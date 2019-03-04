#include <SPI.h>  
#include "RF24.h" 
#include <Servo.h>

#define MECHS 4 //Number of Mechanisms: 0 = Shooter, 1 = Grabber, 2 = Foam Balls, 3 = Pit Balls
//DC Motors
#define DC1PWM 3
#define DC1in1 5
#define DC1in2 6
#define DC2PWM 9
#define DC2in1 10
#define DC2in2 11

//Servos
Servo servo0;
//Servo servo1;
int pos0=0;
//int pos1=0;

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
        case 1:
          //Shooter
          //Notes: Only needs DC motor in 1 direction
          analogWrite(DC1PWM,2);
          digitalWrite(DCin1,LOW);
          digitalWrite(DC1in2,HIGH);
          delay(20);
          break;
        case 2:
          //Grabber
          //Notes: DC motor to go in both linear directions
          analogWrite(DC1PWM,2);
          digitalWrite(DC2in1,LOW);
          digitalWrite(DC2in2,HIGH);
          delay(1500);
          digitalWrite(DC2in1,HIGH);
          digitalWrite(DC2in2,LOW);
          delay(1500);
          break;
        case 3:
          //Pit Balls
          //Servo connected to pin 9
          for (pos0=0; pos0<=180; pos0+=45){
            servo0.write(pos0);
            delay(15);
          }
          for (pos0=180; pos0>=0; pos0-=45){
            servo0.write(pos0);
            delay(15);
          }
          break;
        case 4:
          //One Foam balls
          //Servo connected to pin 10
          /*
           for (pos1=0; pos1<=90; pos1+=30){
            servo1.write(pos1);
            delay(15);
          }
          
          for (pos1=90; pos1>=0; pos1-=30){
            servo1.write(pos1);
            delay(15);
          */
          break;
        case 5:
          //All Foam balls
          //Servo connected to pin 10
          /*
           for (pos1=0; pos1<=90; pos1+=30){
            servo1.write(pos1);
            delay(15);
          }
          delay (5000);
          for (pos1=90; pos1>=0; pos1-=30){
            servo1.write(pos1);
            delay(15);
          }
          */
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
