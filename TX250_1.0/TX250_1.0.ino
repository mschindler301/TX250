unsigned long loopStart, interval;

#define SD_Buzzer

#include "Voltage.h"
#include "NRF.h"
#include "Joystick.h"
#include "LCD.h"
#include <avr/wdt.h>

void setup() {
  lcdSetup();
  joystickSetup();
  nrfSetup();
  
  #ifdef SD_Buzzer
  SDSetup();
  #endif
}

void loop() {
  loopStart = micros();
    joystickLoop();
    voltLoop();
    LCDLoop();
    nrfLoop();
    #ifdef SD_Buzzer
    SDLoop();
    #endif
  interval = micros() - loopStart;
}
