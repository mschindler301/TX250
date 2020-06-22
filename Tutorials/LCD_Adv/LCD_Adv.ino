#include <LiquidCrystal.h>

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 6; //Pins connected the LCD Screen
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long start, interval, lcdRefresh;
int Screen;
int maxScreenNumber = 2; //Screen count (including 0)

int X1, Y1;
int X2, Y2;
int X1_offset, Y1_offset, X2_offset, Y2_offset;

double rawVoltage, battVoltage;
double r1 = 49900; //R1 Resistor Value in Ohms
double r2 = 10000; //R2 Resistor Value in Ohms

int level;

void setup() {
  lcd.begin(16, 2); //Set the ammount of (Columns, Rows)

  X1_offset = analogRead(A2); //Read Joystick Offsets
  Y1_offset = analogRead(A3);
  X2_offset = analogRead(A0);
  Y2_offset = analogRead(A1);
}

void loop() {
  start = millis(); //Record the start time of the Loop

  X1 = analogRead(A3) - X1_offset; //Apply Offset to the Joysticks
  Y1 = analogRead(A2) - Y1_offset;
  X2 = analogRead(A1) - X2_offset;
  Y2 = analogRead(A0) - Y2_offset;

  if (lcdRefresh > 250) { //Refresh the screen every 250ms (1/4 of a second, 4Hz)
    lcd.clear();          //Clear the Screen
    lcdRefresh = 0;       //Reset the Timer

    if (Y1 >  300) Screen--;                        //Decrease the Screen number if the stick is lowered
    if (Y1 < -300) Screen++;                        //Increase the Screen number if the stick is rasied
    Screen = constrain(Screen, 0, maxScreenNumber); //Constrain the screens the the max number of screens

    //// Remote Screens ////
    if (Screen == 0) screen_0();
    if (Screen == 1) screen_1();
    if (Screen == 2) screen_2();
  }

  lcdRefresh += interval;      //Add the loop interval time to the lcdRefresh Timer
  interval = millis() - start; //How long the loops has taken
}

void screen_0() {             //Screen 0 code
  lcd.print("Main Screen");
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
}

void screen_1() {             //Screen 1 code
  lcd.print("Battery Voltage");
  lcd.setCursor(0, 1);
  rawVoltage = analogRead(A4) * (5.0 / 1024.0);
  battVoltage = rawVoltage / (r2 / (r1 + r2));
  lcd.print(battVoltage);
}

void screen_2() {              //Screen 2 code
  lcd.print("Setting");
  lcd.setCursor(0, 1);
  if (Y2 >  200) level++;
  if (Y2 < -200) level--;
  lcd.print(level);
}
