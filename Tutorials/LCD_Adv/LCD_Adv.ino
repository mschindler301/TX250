#include <LiquidCrystal.h>

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long start, interval, lcdRefresh;
int Screen;
int maxScreenNumber = 2;

int X1, Y1;
int X2, Y2;
int X1_offset, Y1_offset, X2_offset, Y2_offset;

double rawVoltage, battVoltage;
double r1 = 49900;
double r2 = 10000;

int level;

void setup() {
  lcd.begin(16, 2);

  X1_offset = analogRead(A2);
  Y1_offset = analogRead(A3);
  X2_offset = analogRead(A0);
  Y2_offset = analogRead(A1);
}

void loop() {
  start = millis();

  X1 = analogRead(A3) - X1_offset;
  Y1 = analogRead(A2) - Y1_offset;
  X2 = analogRead(A1) - X2_offset;
  Y2 = analogRead(A0) - Y2_offset;

  if (lcdRefresh > 250) {
    lcd.clear();
    lcdRefresh = 0;

    if (Y1 >  300) Screen--;
    if (Y1 < -300) Screen++;
    Screen = constrain(Screen, 0, maxScreenNumber);

    //// Remote Screens ////
    if (Screen == 0) screen_0();
    if (Screen == 1) screen_1();
    if (Screen == 2) screen_2();
  }

  lcdRefresh += interval;
  interval = millis() - start;
}

void screen_0() {
  lcd.print("Main Screen");
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
}

void screen_1() {
  lcd.print("Battery Voltage");
  lcd.setCursor(0, 1);
  rawVoltage = analogRead(A4) * (5.0 / 1024.0);
  battVoltage = rawVoltage / (r2 / (r1 + r2));
  lcd.print(battVoltage);
}

void screen_2() {
  lcd.print("Setting");
  lcd.setCursor(0, 1);
  if (Y2 >  200) level++;
  if (Y2 < -200) level--;
  lcd.print(level);
}
