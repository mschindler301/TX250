#include <EEPROM.h>

//// Left Joystick ////
#define JOYSTICK_X1  A3
#define JOYSTICK_Y1  A2
#define SWITCH_L    4
//// Right Joystick ////
#define JOYSTICK_X2  A1
#define JOYSTICK_Y2  A0
#define SWITCH_R     5

double deadStick = 0.05 * 1024; //5% of the joystick resolution

int middlePoint = 1500;
int minEndpoint = 1000;
int maxEndpoint = 2000;

boolean calFlag;

int midX1 = 512;
int minX1 = 0;
int maxX1 = 1024;

int midY1 = 512;
int minY1 = 0;
int maxY1 = 1024;

int midX2 = 512;
int minX2 = 0;
int maxX2 = 1024;

int midY2 = 512;
int minY2 = 0;
int maxY2 = 1024;

int X1, X2, Y1, Y2;
boolean S1, S2;

boolean killFlag = true;
unsigned long timerKill;
boolean stillClick;

int channel_1, channel_2, channel_3, channel_4;

unsigned long flightTimer;

void joystickSetup() {
  //Set the Joystick button pin to High. When pressed, the button will be pulled to ground
  pinMode(SWITCH_L, OUTPUT);
  digitalWrite(SWITCH_L, HIGH);
  pinMode(SWITCH_R, OUTPUT);
  digitalWrite(SWITCH_R, HIGH);

  EEPROM.get(24, calFlag);
  
  if (calFlag == 1) {
    EEPROM.get(0, midX1);
    EEPROM.get(2, midY1);
    EEPROM.get(4, midX2);
    EEPROM.get(6, midY2);

    EEPROM.get(8,  maxY1);
    EEPROM.get(10, maxY2);

    EEPROM.get(12, minY1);
    EEPROM.get(14, minY2);

    EEPROM.get(16, maxX1);
    EEPROM.get(18, maxX2);

    EEPROM.get(20, minX1);
    EEPROM.get(22, minX2);
  }
}

void joystickLoop() {
  X1 = -(analogRead(JOYSTICK_X1) - midX1);                     //read joystick and apply offset
  if (abs(X1) < deadStick) X1 = 0;                             //set deadstick
  if (X1 < 0) X1 = map(X1, -deadStick, midX1 - maxX1, 0, -500); //map bottom range
  if (X1 > 0) X1 = map(X1,  deadStick, midX1 - minX1, 0,  500); //map top range

  Y1 = analogRead(JOYSTICK_Y1) - midY1;
  if (abs(Y1) < deadStick) Y1 = 0;
  if (Y1 < 0) Y1 = map(Y1, -deadStick, minY1 - midY1, 0, -500);
  if (Y1 > 0) Y1 = map(Y1,  deadStick, maxY1 - midY1, 0,  500);

  X2 = -(analogRead(JOYSTICK_X2) - midX2);
  if (abs(X2) < deadStick) X2 = 0;
  if (X2 < 0) X2 = map(X2, -deadStick, midX2 - maxX2, 0, -500);
  if (X2 > 0) X2 = map(X2,  deadStick, midX2 - minX2, 0,  500);

  Y2 = analogRead(JOYSTICK_Y2) - midY2;
  if (abs(Y2) < deadStick) Y2 = 0;
  if (Y2 < 0) Y2 = map(Y2, -deadStick, minY2 - midY2, 0, -500);
  if (Y2 > 0) Y2 = map(Y2,  deadStick, maxY2 - midY2, 0,  500);

  S1 = !digitalRead(SWITCH_L);
  S2 = !digitalRead(SWITCH_R);


  //// Kill Switch ////
  timerKill += interval; //S2 must be clicked in   //0.1 seconds for anti-bounce   //stillClick for anti-hold
  if (S2 && timerKill > 100000 && !stillClick) {
    timerKill = 0;
    killFlag = !killFlag;
  }
  if (S2) stillClick = 1;
  else stillClick = 0;

  //// Flight Timer ////
  if (!killFlag) flightTimer += interval;

  //// Assign to NRF message ////
  myData.X1 = X1;
  myData.Y1 = Y1;
  myData.S1 = S1;
  myData.X2 = X2;
  myData.Y2 = -Y2; //// Delete this after updating to Drone Verison 5 ////
  myData.S2 = S2;
  myData.killFlag = !killFlag;

  //  Serial.print(X1); Serial.print("\t");
  //  Serial.print(Y1); Serial.print("\t");
  //  Serial.print(S1); Serial.print("\t");
  //  Serial.print(X2); Serial.print("\t");
  //  Serial.print(Y2); Serial.print("\t");
  //  Serial.print(S2); Serial.print("\t");
  //  Serial.println();
}
