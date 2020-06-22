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
  pinMode(5, OUTPUT); 
  pinMode(4, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
 
  radio.begin(); 
  radio.setChannel(108);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);

  radio.openWritingPipe(addresses[0]); 
  radio.stopListening();
}


void loop() { 
    myData.Xposition = analogRead(A0);
    myData.Yposition = analogRead(A1);
    myData.switchOn  = digitalRead(5);
    
    myData.X2position = analogRead(A2);
    myData.Y2position = analogRead(A3);
    myData.switch2On  = digitalRead(4);

    radio.write(&myData, sizeof(myData), 1);
    Serial.println("Sent");
    delay(10);
}
