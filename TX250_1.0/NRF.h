#include <SPI.h>   // Comes with Arduino IDE
#include "RF24.h"  // Download and Install (See above)

#define  CE_PIN  7   // The pins to be used for CE and SN
#define  CSN_PIN 8

const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};
RF24 radio(CE_PIN, CSN_PIN);
byte addresses[][6] = {"1Node", "2Node"}; // These will be the names of the "Pipes"

bool newData;
bool signalFlag;
int readCount;
int strength;
unsigned long timerRead;
unsigned long noSignalTimer;

double droneVolt;
double roll, pitch, yaw;
double heading;
double latitude, longitude, velocity, alt;

boolean printData = false;

struct dataStruct {      //this is the NRF data. Max of 32 bytes
  int  X1;               //double  = 4 bytes
  int  Y1;               //int     = 2 bytes
  bool S1;               //boolean = 1 byte
  int  X2;
  int  Y2;
  bool S2;

  bool killFlag;
  int rcMode;
  double kp;
  double ki;
  double kd;
  int midpoint;
} myData;

struct dataStruct1 {
  int feedback_1;     //Voltage  (multiple of 1000)
  int feedback_2;     //Roll     (multiple of 100)
  int feedback_3;     //Pitch    (multiple of 100)
  int feedback_4;     //Yaw      (multiple of 100)
  int feedback_5;     //heading
  int feedback_6;  //latitude
  int feedback_7;  //longitiude
  int feedback_8;     //altitude
  int feedback_9;     //velcoity (multiple of 100)
  int feedback_10;    //
  int feedback_11;    //
  int feedback_12;    //
  int feedback_13;
  int feedback_14;
  int feedback_15;
  int feedback_16;
} myTele;


void nrfSetup () {
  radio.begin();
  radio.setChannel(108);                  // Above most WiFi frequencies
  radio.setDataRate(RF24_250KBPS);        // Fast enough.. Better range

  radio.setPALevel(RF24_PA_MAX);
  radio.enableAckPayload();
  radio.setRetries(7, 0);                 // delay, count
  radio.openWritingPipe(slaveAddress);
}

void nrfLoop() {
  newData = false;

  if (radio.write( &myData, sizeof(myData) )) {
    if ( radio.isAckPayloadAvailable() ) {
      radio.read(&myTele, sizeof(myTele));
      newData = true;
      signalFlag = true;
      readCount++;
      noSignalTimer = micros();

      droneVolt = myTele.feedback_1 * 0.001;
      roll      = myTele.feedback_2 * 0.01;
      pitch     = myTele.feedback_3 * 0.01;
      yaw       = myTele.feedback_4 * 0.01;
      heading   = myTele.feedback_5 * 0.1;
      latitude  = myTele.feedback_6;
      longitude = myTele.feedback_7;
      velocity  = myTele.feedback_8 * 0.01;
      alt       = myTele.feedback_9 * 0.1;

      if (printData == true) {
        Serial.print(millis());          Serial.print("\t");
        Serial.print(droneVolt);          Serial.print("\t");
        Serial.print(roll);               Serial.print("\t");
        Serial.print(pitch);              Serial.print("\t");
        Serial.print(yaw);                Serial.print("\t");
        Serial.print(heading);            Serial.print("\t");
        Serial.print(latitude,  6);       Serial.print("\t");
        Serial.print(longitude, 6);       Serial.print("\t");
        Serial.print(velocity);           Serial.print("\t");
        Serial.print(alt);                Serial.print("\t");
        Serial.print(myTele.feedback_10); Serial.print("\t");
        Serial.print(myTele.feedback_11); Serial.print("\t");
        Serial.print(myTele.feedback_12); Serial.print("\t");
        Serial.print(myTele.feedback_13); Serial.print("\t");
        Serial.print(myTele.feedback_14); Serial.print("\t");
        Serial.print(myTele.feedback_15); Serial.print("\t");
        Serial.print(myTele.feedback_16); Serial.print("\t");
        Serial.println();
      }
    }
  }

  timerRead += interval;
  if (timerRead > 1000000) {
    strength = readCount;
    timerRead = 0;
    readCount = 0;
  }

  if (micros() - noSignalTimer > 1000000) signalFlag = false;
}
