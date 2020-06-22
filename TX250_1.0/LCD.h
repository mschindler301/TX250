#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include "MicroSD.h"
#include "Screens.h"
#include "Buzzer.h"

boolean lcdRefresh;
int Screen;
unsigned long timerLCD;

unsigned long lcdRefreshTime = 500000; //micro-seconds
int maxScreenNumber = 6;

void lcdSetup() {
  lcd.begin(16, 2);
  lcd.print("TX250");
  lcd.setCursor(0, 1);
  lcd.print("Version 1.0");
  
  #ifdef SD_Buzzer
  buzzerStart();
  #endif
  
  delay(1000);
  lcd.clear();
}

void LCDLoop() {
  //// Go to the Menus if the Left Stick is Clicked in and Flight is Off ////
  if (!digitalRead(SWITCH_L) && Screen == 0 && killFlag) {
    Screen = 1;
    delay(200);
  }
  if (!digitalRead(SWITCH_L) && Screen > 0) {
    Screen = 0;
    delay(200);
  }

  if (Screen > 0) {
    lcdRefreshTime = 100000;
    killFlag = true;
  }
  else lcdRefreshTime = 500000;

  //// LCD Display Refresh ////
  if ((micros() - timerLCD) > lcdRefreshTime) {
    timerLCD = micros();
    lcdRefresh = true;
  }
  else lcdRefresh = false;

  if (lcdRefresh == true) {
    lcd.clear();
    if (Screen > 0) {
      if (Y1 >  300 && Screen > 0) Screen--;
      if (Y1 < -300 && Screen > 0) Screen++;
      Screen = constrain(Screen, 1, maxScreenNumber);
    }

    //// Remote Screens ////
    if (Screen == 0) screen_0(); //Telemtrey Screen
    if (Screen == 1) screen_1(); //RC Voltage and Mode
    if (Screen == 2) screen_2(); //Profile Select
    if (Screen == 3) screen_3(); //PID
    if (Screen == 4) screen_4(); //Euler
    if (Screen == 5) screen_5(); //Calibration
    if (Screen == 6) screen_6(); //SD Card

    if (Screen != 2) profileEdit = false;
    if (Screen != 3) pidEdit     = false;
  }

  #ifdef SD_Buzzer
  buzzerLoop();
  #endif

  myData.rcMode = rcMode;
  myData.kp = kp;
  myData.ki = ki;
  myData.kd = kd;
  myData.midpoint = midPoint;
}
