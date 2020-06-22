//Transmitter Code for One Way Communication
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
  pinMode(5, OUTPUT);               //Setup the Pushbuttons
  pinMode(4, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
 
  radio.begin();                    //Initialize the NRF   
  radio.setChannel(108);            //Set the Radio Frequency (2.4Ghz + 0.(108)Ghz = 2.508Ghz)
  radio.setDataRate(RF24_250KBPS);  //Set Speed to 250KBPS, slower speed but better range
  radio.setPALevel(RF24_PA_MIN);    //Set Power Amplifier Level to Low for Testing

  radio.openWritingPipe(addresses[0]);  //Open a writing Pipe to receive messages
  radio.stopListening();                //Put the NRF in "TX" mode 
}


void loop() { 
    myData.Xposition = analogRead(A0);      //Read the joystick values to send to the receiver
    myData.Yposition = analogRead(A1);
    myData.switchOn  = digitalRead(5);
    
    myData.X2position = analogRead(A2);
    myData.Y2position = analogRead(A3);
    myData.switch2On  = digitalRead(4);

    radio.write(&myData, sizeof(myData), 1); //Send the myData struct containing the Joystick Data
    Serial.println("Sent");
    delay(10);
}
