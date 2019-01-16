#include <SPI.h>  
#include "RF24.h" 

//RF24:
typedef struct package
{
  int   state = 0; //State of the selected option: 0 = OFF; 1 = ON; 2 = SINGLE SHOT 
  int   key = 0; //"Hash key" of the selected option
  double xAxis = 0.0; //yAxis value from the joysticks
  double yAxis = 0.0; //xAxis value from the joysticks
  bool  locked = true; //True until we want to lock
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
    Serial.print("\nstate:");
    Serial.print(toReceive.state);
    Serial.print("\n");
    Serial.println(toReceive.key);
    Serial.println(toReceive.xAxis);
    Serial.println(toReceive.yAxis);
    Serial.println(toReceive.locked);
  }
  else{
    Serial.println("Controller Not Found");
  }

}
