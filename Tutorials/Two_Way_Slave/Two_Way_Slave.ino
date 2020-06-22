                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  #include <SPI.h> 
#include "RF24.h" 

#define  CE_PIN  7 
#define  CSN_PIN 8

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

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
  radio.begin();        
  radio.setChannel(108);      
  radio.setDataRate(RF24_250KBPS); 
  radio.setPALevel(RF24_PA_MIN);
  
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.enableAckPayload();
  radio.writeAckPayload(1, &myTele, sizeof(myTele));
  radio.startListening();
}


void loop() {
    if ( radio.available() ) {  
        radio.read( &myData, sizeof(myData) );
        myTele.feedback_1 = 1234.56;
        myTele.feedback_2 = 1234.56;
        myTele.feedback_3 = 1234.56;
        myTele.feedback_4 = 1234.56;
        
        radio.writeAckPayload(1, &myTele, sizeof(myTele));

        Serial.print(myData.Xposition);  Serial.print("\t");
        Serial.print(myData.Yposition);  Serial.print("\t");
        Serial.print(myData.switchOn);   Serial.print("\t");
        Serial.print(myData.X2position); Serial.print("\t");
        Serial.print(myData.Y2position); Serial.print("\t");
        Serial.print(myData.switch2On);  Serial.print("\t");
        Serial.println();
    }
}
