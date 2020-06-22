// Receiver Code for One Way Communication
// RF24 Library is Needed

#include <SPI.h>
#include "RF24.h"

#define  CE_PIN  7
#define  CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);

byte addresses[][6] = {"1Node"}; //Address for the Pipe of the NRF

struct dataStruct { // Send Data through a Struct. Maximum of 32 Bytes per message!
  int Xposition;    // bool   = 1 byte
  int Yposition;    // int    = 2 bytes
  bool switchOn;    // double = 4 bytes

  int X2position;       
  int Y2position;
  bool switch2On;         
} myData;           //Name of the Struct

void setup() { 
  Serial.begin(115200);
  radio.begin();                    //Initialize the NRF   
  radio.setChannel(108);            //Set the Radio Frequency (2.4Ghz + 0.(108)Ghz = 2.508Ghz)   
  radio.setDataRate(RF24_250KBPS);  //Set Speed to 250KBPS, slower speed but better range
  radio.setPALevel(RF24_PA_MIN);    //Set Power Amplifier Level to Low for Testing

  radio.openReadingPipe(1, addresses[0]); //Open a reading Pipe to receive messages
  radio.startListening();                 //Put the NRF in "RX" mode 
}


void loop() {
  
  if (radio.available()) {                                //Check if data is available
    radio.read( &myData, sizeof(myData) );                //Read the data and assign it to the "myData" struct
    Serial.print(myData.Xposition);  Serial.print("\t");  //Print the Values
    Serial.print(myData.Yposition);  Serial.print("\t");
    Serial.print(myData.switchOn);   Serial.print("\t");
    Serial.print(myData.X2position); Serial.print("\t");
    Serial.print(myData.Y2position); Serial.print("\t");
    Serial.print(myData.switch2On);  Serial.print("\t");
    Serial.println();
  }
  
}
