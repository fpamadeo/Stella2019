#include <SPI.h>  
#include "RF24.h" 

#define MECHS 4 //Number of Mechanisms: 0 = Shooter, 1 = Grabber, 2 = Foam Balls, 3 = Pit Balls

//RF24:
typedef struct package
{
   int   key = 0; //"Hash key" of the selected option; 
                 //KEYS: Shooter - Grabber - PitBalls - FoamBalls - All
  bool  locked[MECHS] = {true, true, true, true}; //True until we want to unlock
} pkg; 
byte addresses[][6] = {("0")};
pkg toReceive;
RF24 Stella (8,7);


void setup() 
{
  Serial.begin(9600);
  delay(1000);

  Stella.begin(); 
  Stella.setChannel(115); 
  Stella.setPALevel(RF24_PA_MAX);
  Stella.setDataRate( RF24_250KBPS ) ; 
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
      Stella.read( &toReceive, sizeof(toReceive) );
    }
    Serial.print("\nStates:\n[0] = ");
    Serial.print(toReceive.state[0]);
    Serial.print("\n[1] = ");
    Serial.print(toReceive.state[1]);
    Serial.print("\n[2] = ");
    Serial.print(toReceive.state[2]);
    Serial.print("\n[3] = ");
    Serial.print(toReceive.state[3]);
    Serial.print("\n\nKEY = ");
    Serial.println(toReceive.key);
    Serial.print("\nX-AXIS = ");
    Serial.println(toReceive.xAxis);
    Serial.print("\nY-AXIS = ");
    Serial.println(toReceive.yAxis);
    Serial.print("\n\n LOCKED: ");
    if(toReceive.locked[0]){ //Shooter
      Serial.print("0 ");
      
    }
    if(toReceive.locked[1]){ //Grabber
      Serial.print("1 ");
      
    }
    if(toReceive.locked[2]){ //Foamballs
      Serial.print("2 ");
      
    }
    if(toReceive.locked[3]){ //Pitballs
      Serial.print("3 ");
      
    }
  }
  else{
    Serial.println("Controller Not Found");
  }

}
