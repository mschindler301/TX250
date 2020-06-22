#include <SPI.h>
#include "SdFat.h"
SdFat SD;
#define csSD 2

boolean SDDetect;
File logFile;
File countFile;
unsigned long SDFlush;
boolean cardDetect;
boolean logData = false;

int fileCount;

void SDSetup() {
  //// Check Card Detect Pin ////
  pinMode(A5, INPUT);
  cardDetect = digitalRead(A5);

  //// If card is detected, initialize card ////
  if (cardDetect == 0) SDDetect = SD.begin(csSD);

  //// Count the amount of Files in the LogFiles Directory ////
  if (SDDetect) {
    countFile = SD.open("LogFiles/");
    while (true) {
      File entry =  countFile.openNextFile();
      if (! entry) {
        break;
      }
      entry.close();
      fileCount++;
    }
    fileCount++; //Add one to fileCount to creat the next file
  }
}

void createNewFile() {
  char fName[] = "LogFiles/Log0000.csv";
  const byte lastDigit = sizeof(fName) - 6;

  for (int n = 1; n <= fileCount; n++) {
    if (++fName[lastDigit] > '9') {
      fName[lastDigit] = '0';
      if (++fName[lastDigit - 1] > '9') {
        fName[lastDigit - 1] = '0';
        if (++fName[lastDigit - 2] > '9') {
          fName[lastDigit - 2] = '0';
          if (++fName[lastDigit - 3] > '9') {
            fName[lastDigit - 3] = '0';
          }
        }
      }
    }
  }
  logFile = SD.open(fName, FILE_WRITE);
}

void SDLoop() {
  SDFlush += interval;

  if (logFile && newData && signalFlag) {
    logFile.print(millis()); logFile.print(",");
    logFile.print(myTele.feedback_1);  logFile.print(",");
    logFile.print(myTele.feedback_2);  logFile.print(",");
    logFile.print(myTele.feedback_3);  logFile.print(",");
    logFile.print(myTele.feedback_4);  logFile.print(",");
    logFile.print(myTele.feedback_5);  logFile.print(",");
    logFile.print(myTele.feedback_6);  logFile.print(",");
    logFile.print(myTele.feedback_7);  logFile.print(",");
    logFile.print(myTele.feedback_8);  logFile.print(",");
    logFile.print(myTele.feedback_9);  logFile.print(",");
    logFile.print(myTele.feedback_10); logFile.print(",");
    logFile.print(myTele.feedback_11); logFile.print(",");
    logFile.print(myTele.feedback_12); logFile.print(",");
    logFile.print(myTele.feedback_13); logFile.print(",");
    logFile.print(myTele.feedback_14); logFile.print(",");
    logFile.print(myTele.feedback_15); logFile.print(",");
    logFile.println(myTele.feedback_16);
  }

  if (SDFlush > 250000) {
    logFile.flush();
    SDFlush = 0;
  }
}
