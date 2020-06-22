#include <SPI.h>
#include "RF24.h"

#define  CE_PIN  7
#define  CSN_PIN 8

const byte slaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

struct dataStruct {
  int Xposition;   
  int Yposition;  
  bool switchOn;  

  int X2position;       
  int Y2position;
  bool switch2On; 
} myData;    

struct dataStruct1 {    
  double feedback_1;
  double feedback_2;
  double feedback_3; 
  double feedback_4;
} myTele;

void setup() { 
  Serial.begin(115200);
  pinMode(4, OUTPUT);  
  pinMode(5, OUTPUT);  
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);

  radio.begin();       
  radio.setChannel(108);    
  radio.setDataRate(RF24_250KBPS); 
  radio.setPALevel(RF24_PA_MIN);
  
  radio.enableAckPayload();
  radio.openWritingPipe(slaveAddress);
}


void loop() {
  myData.Xposition  = analogRead(A3); 
  myData.Yposition  = analogRead(A2);
  myData.switchOn   = digitalRead(4);
  myData.X2position = analogRead(A1);
  myData.Y2position = analogRead(A0);
  myData.switch2On  = digitalRead(5);
  
  if (radio.write( &myData, sizeof(myData) )) {
    if ( radio.isAckPayloadAvailable() ) {
        radio.read(&myTele, sizeof(myTele));
               
        Serial.print(myTele.feedback_1); Serial.print("\t"); 
        Serial.print(myTele.feedback_2); Serial.print("\t");  
        Serial.print(myTele.feedback_3); Serial.print("\t");    
        Serial.print(myTele.feedback_4); Serial.print("\t");   
        Serial.println();
    }
  }
}
