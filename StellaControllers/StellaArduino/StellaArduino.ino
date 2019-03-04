#include <SPI.h>  
#include "RF24.h" 

#define MECHS 4 //Number of Mechanisms: 0 = Shooter, 1 = Grabber, 2 = Foam Balls, 3 = Pit Balls

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
}


void loop()  
{

  if ( Stella.available()) 
  {
    Serial.println("Controller Found");
    while (Stella.available())
    {
      Stella.read(&key, sizeof(key) );
    }
  }
  else{
    Serial.println("Controller Not Found");
  }

}
