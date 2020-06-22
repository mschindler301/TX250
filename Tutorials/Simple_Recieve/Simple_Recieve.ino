#include <SPI.h>
#include "RF24.h"

#define  CE_PIN  7
#define  CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);

byte addresses[][6] = {"1Node"};

struct dataStruct { 
  int Xposition;    
  int Yposition;  
  bool switchOn; 

  int X2position;       
  int Y2position;
  bool switch2On;         
} myData;  

void setup() { 
  Serial.begin(115200);
  radio.begin();   
  radio.setChannel(108);   
  radio.setDataRate(RF24_250KBPS); 
  radio.setPALevel(RF24_PA_MIN); 

  radio.openReadingPipe(1, addresses[0]); 
  radio.startListening();
}


void loop() {
  
  if (radio.available()) {      
    radio.read( &myData, sizeof(myData) );
    Serial.print(myData.Xposition);  Serial.print("\t");
    Serial.print(myData.Yposition);  Serial.print("\t");
    Serial.print(myData.switchOn);   Serial.print("\t");
    Serial.print(myData.X2position); Serial.print("\t");
    Serial.print(myData.Y2position); Serial.print("\t");
    Serial.print(myData.switch2On);  Serial.print("\t");
    Serial.println();
  }
  
}
