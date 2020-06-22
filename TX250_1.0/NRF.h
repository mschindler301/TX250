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
  int feedback_1;  //Voltage (multiple of 1000)
  int feedback_2;  //Roll    (multiple of 100)
  int feedback_3;  //Pitch   (multiple of 100)
  int feedback_4;  //Yaw     (multiple of 100)
  int feedback_5;  //acc_x
  int feedback_6;  //acc_y
  int feedback_7;  //acc_x
  int feedback_8;  //gyro_x
  int feedback_9;  //gyro_y
  int feedback_10; //gyro_z
  int feedback_11; //angle_roll_acc   (multiple of 100)
  int feedback_12; //angle_roll_pitch (multiple of 100)
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
  radio.setRetries(7, 3);                 // delay, count  //5 for NRF
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
