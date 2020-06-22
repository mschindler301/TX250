#include <LiquidCrystal.h>

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 6; //Pins connected to the LCD screen
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
 lcd.begin(16, 2);            //Set Number of (Columns, Rows)
 lcd.print("Hello, World!");  //Print "Hello, World!" to the LCD Screen
}

void loop() {
  lcd.setCursor(0, 1);        //Set Cursor to the bottom Row
  lcd.print(millis() / 1000); //Display the amount of seconds that have passed
}
