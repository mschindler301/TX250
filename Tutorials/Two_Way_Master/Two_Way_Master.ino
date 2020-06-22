// Master Code for Two Way Communication
// RF24 Library is Needed

#include <SPI.h>
#include "RF24.h"

#define  CE_PIN  7
#define  CSN_PIN 8

const byte slaveAddress[5] = {'R','x','A','A','A'}; //Address for the Pipes of the NRF

RF24 radio(CE_PIN, CSN_PIN);

struct dataStruct { // Send Data through a Struct. Maximum of 32 Bytes per message!
  int Xposition;    // bool   = 1 byte
  int Yposition;    // int    = 2 bytes
  bool switchOn;    // double = 4 bytes

  int X2position;       
  int Y2position;
  bool switch2On; 
} myData;           //Name of the Struct

struct dataStruct1 {    
  double feedback_1;
  double feedback_2;
  double feedback_3; 
  double feedback_4;
} myTele;

void setup() { 
  Serial.begin(115200);
  pinMode(4, OUTPUT);  
  pinMode(5, OUTPUT);               //Setup the Pushbuttons
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);

  radio.begin();                    //Initialize the NRF   
  radio.setChannel(108);            //Set the Radio Frequency (2.4Ghz + 0.(108)Ghz = 2.508Ghz)
  radio.setDataRate(RF24_250KBPS);  //Set Speed to 250KBPS, slower speed but better range
  radio.setPALevel(RF24_PA_MIN);    //Set Power Amplifier Level to Low for Testing
  
  radio.enableAckPayload();            //Enable Acknowledge Payloads
  radio.openWritingPipe(slaveAddress); //Open a writing Pipe
}


void loop() {
  myData.Xposition  = analogRead(A3); //Read the joystick values to send to the receiver
  myData.Yposition  = analogRead(A2);
  myData.switchOn   = digitalRead(4);
  myData.X2position = analogRead(A1);
  myData.Y2position = analogRead(A0);
  myData.switch2On  = digitalRead(5);
  
  if (radio.write( &myData, sizeof(myData) )) { //Send the myData struct containing the Joystick Data
    if ( radio.isAckPayloadAvailable() ) {      //Check to see if an Acknowledge payload has been received
        radio.read(&myTele, sizeof(myTele));    //Read the Data
               
        Serial.print(myTele.feedback_1); Serial.print("\t"); 
        Serial.print(myTele.feedback_2); Serial.print("\t");  
        Serial.print(myTele.feedback_3); Serial.print("\t");    
        Serial.print(myTele.feedback_4); Serial.print("\t");   
        Serial.println();
    }
  }
}
