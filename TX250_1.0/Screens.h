int rcMode;
int midPoint = 1400;
int profileSelect = 1;
boolean profileEdit;

double kp = 0.55;
double ki = 4.00;
double kd = 5.50;

unsigned int minutes, seconds;

boolean pidEdit = false;
int variableEdit;

//// Flight Screen ////
void screen_0 () {
  if (signalFlag) {
    lcd.setCursor(0, 0);
    lcd.print("Dr:");
    lcd.print(droneVolt);

    lcd.setCursor(9, 0);
    lcd.print("Sig:");
    lcd.print(strength);
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print(droneVolt);
    lcd.setCursor(6, 0);
    lcd.print("No Signal!");
  }
  if (killFlag) {
    lcd.setCursor(9, 1);
    lcd.print("Kill On");
  }

  seconds = (flightTimer / 1000000) % 60;
  minutes = (((flightTimer / 1000000) - seconds) / 60) % 60;

  lcd.setCursor(0, 1);
  if (minutes < 10) {
    lcd.setCursor(0, 1);
    lcd.print("0");
  }
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) {
    lcd.setCursor(3, 1);
    lcd.print("0");
  }
  lcd.print(seconds);
}

//// RC Battery and Mode Screen ////
void screen_1 () {
  lcd.print("RC Batt: "); lcd.print(rcVolt);

  if (Y2 >  200) rcMode += 1;
  if (Y2 < -200) rcMode -= 1;
  rcMode = constrain(rcMode, 0, 4);

  lcd.setCursor(0, 1);
  lcd.print("Mode: ");
  lcd.setCursor(6, 1);

  if (rcMode == 0) lcd.print("Indoor");
  if (rcMode == 1) lcd.print("Normal");
  if (rcMode == 2) lcd.print("Outdoor");
  if (rcMode == 3) lcd.print("Sport");
  if (rcMode == 4) lcd.print("Acro");
}

//// Profile Select ////
void screen_2() {
  lcd.print("Profile:");
  lcd.setCursor(1, 1);

  if (profileSelect == 1) lcd.print("Carbon 3S");
  if (profileSelect == 2) lcd.print("Carbon 4S");
  if (profileSelect == 3) lcd.print("uTini");
  if (profileSelect == 4) lcd.print("F450");

  if (S2) profileEdit = !profileEdit;
  if (profileEdit == true) {
    lcd.setCursor(0, 1);
    lcd.print(">");
    if (Y2 >  300) profileSelect++;
    if (Y2 < -300) profileSelect--;
    profileSelect = constrain(profileSelect, 1, 4);
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print(" ");
  }

  if (profileSelect == 1 && profileEdit == true) {
    kp = 0.55;  //Carbon 3S
    ki = 4.00;
    kd = 5.50;
    midPoint = 1400;
  }
  if (profileSelect == 2 && profileEdit == true) {
    kp = 0.45;  //Carbon 4S
    ki = 3.00;
    kd = 4.50;
    midPoint = 1300;
  }
  if (profileSelect == 3 && profileEdit == true) {
    kp = 0.35;  //uTini
    ki = 3.00;
    kd = 1.80;
    midPoint = 1500;
  }
  if (profileSelect == 4 && profileEdit == true) {
    kp = 0.00;  //F450
    ki = 0.00;
    kd = 0.00;
    midPoint = 1000;
  }
}

//// PID Edit ////
void screen_3 () {
  if (S2) pidEdit = !pidEdit;
  if (X2 >  300) variableEdit++;
  if (X2 < -300) variableEdit--;
  variableEdit = constrain(variableEdit, 1, 4) ;
  midPoint = constrain(midPoint, 1000, 2000);

  if (pidEdit && variableEdit == 1) {
    if (Y2 >  300) midPoint += 4;
    if (Y2 < -300) midPoint -= 4;
    lcd.setCursor(0, 0);
    lcd.print(">");
  }

  if (pidEdit && variableEdit == 2) {
    if (Y2 >  300) kp += 0.01;
    if (Y2 < -300) kp -= 0.01;
    lcd.setCursor(8, 0);
    lcd.print(">");
  }

  if (pidEdit && variableEdit == 3) {
    if (Y2 >  300) ki += 0.10;
    if (Y2 < -300) ki -= 0.10;
    lcd.setCursor(0, 1);
    lcd.print(">");
  }

  if (pidEdit && variableEdit == 4) {
    if (Y2 >  300) kd += 0.10;
    if (Y2 < -300) kd -= 0.10;
    lcd.setCursor(8, 1);
    lcd.print(">");
  }

  lcd.setCursor(1, 0); lcd.print("M:"); lcd.print(midPoint);
  lcd.setCursor(9, 0); lcd.print("P:"); lcd.print(kp);
  lcd.setCursor(1, 1); lcd.print("I:"); lcd.print(ki);
  lcd.setCursor(9, 1); lcd.print("D:"); lcd.print(kd);
}



//// Euler Telemetry ////
void screen_4 () {
  lcd.print("R:"); lcd.print(roll);
  lcd.setCursor(8, 0); lcd.print("P:"); lcd.print(pitch);
  lcd.setCursor(0, 1); lcd.print("Y:"); lcd.print(yaw);
}

//// Joystick Calibration ////
void screen_5 () {
  lcd.print("Click in R Stick");
  lcd.setCursor(0, 1);
  lcd.print("To Calibrate");

  if (!digitalRead(SWITCH_R)) {
    lcd.clear();
    lcd.print("Center Sticks");
    lcd.setCursor(0, 1); lcd.print("3"); delay(1000);
    lcd.setCursor(0, 1); lcd.print("2"); delay(1000);
    lcd.setCursor(0, 1); lcd.print("1"); delay(1000);
    EEPROM.put(0, analogRead(JOYSTICK_X1));
    EEPROM.put(2, analogRead(JOYSTICK_Y1));
    EEPROM.put(4, analogRead(JOYSTICK_X2));
    EEPROM.put(6, analogRead(JOYSTICK_Y2));

    lcd.clear();
    lcd.print("Move Sticks Up");
    lcd.setCursor(0, 1); lcd.print("3"); delay(1000);
    lcd.setCursor(0, 1); lcd.print("2"); delay(1000);
    lcd.setCursor(0, 1); lcd.print("1"); delay(1000);
    EEPROM.put(8,  analogRead(JOYSTICK_Y1));
    EEPROM.put(10, analogRead(JOYSTICK_Y2));

    lcd.clear();
    lcd.print("Move Sticks Down");
    lcd.setCursor(0, 1); lcd.print("3"); delay(1000);
    lcd.setCursor(0, 1); lcd.print("2"); delay(1000);
    lcd.setCursor(0, 1); lcd.print("1"); delay(1000);
    EEPROM.put(12, analogRead(JOYSTICK_Y1));
    EEPROM.put(14, analogRead(JOYSTICK_Y2));

    lcd.clear();
    lcd.print("Move Sticks <---");
    lcd.setCursor(0, 1); lcd.print("3"); delay(1000);
    lcd.setCursor(0, 1); lcd.print("2"); delay(1000);
    lcd.setCursor(0, 1); lcd.print("1"); delay(1000);
    EEPROM.put(16, analogRead(JOYSTICK_X1));
    EEPROM.put(18, analogRead(JOYSTICK_X2));

    lcd.clear();
    lcd.print("Move Sticks --->");
    lcd.setCursor(0, 1); lcd.print("3"); delay(1000);
    lcd.setCursor(0, 1); lcd.print("2"); delay(1000);
    lcd.setCursor(0, 1); lcd.print("1"); delay(1000);
    EEPROM.put(20, analogRead(JOYSTICK_X1));
    EEPROM.put(22, analogRead(JOYSTICK_X2));

    EEPROM.put(24, 1); //Shows the Calibration has been completed

    joystickSetup();
  }
}

//// SD Card Logging ////
void screen_6 () {
  if (SDDetect) {
    lcd.print("SD Inserted");

    if (!digitalRead(SWITCH_R) && logData == false) {
      createNewFile();
      logData = true;
    }

    if (logData == true) {
      lcd.setCursor(0, 1);
      lcd.print("Data Logging");
    }
  }
  else lcd.print("SD Not Inserted");
}

//// Serial Logging ////
void screen_7 () {
    lcd.print("Serial Logging");

    if (!digitalRead(SWITCH_R) && printData == false) {
      printData = true;
      delay(200);
    }
    if (!digitalRead(SWITCH_R) && printData == true) {
      printData = false;
      delay(200);
    }

    if (printData == true) {
      lcd.setCursor(0, 1);
      lcd.print("Printing");
    }
    if (printData == false) {
      lcd.setCursor(0, 1);
      lcd.print("Not Printing");
  }
}
