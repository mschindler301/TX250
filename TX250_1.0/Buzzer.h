#define buzzer 3

int buzzerCode;
int batteryCellDrone;
int batteryCellRC;
unsigned long buzzerTimer;

void buzzerStart() {
  pinMode(buzzer, OUTPUT);

  tone(buzzer, 2000); delay(200);
  tone(buzzer, 2500); delay(200);
  tone(buzzer, 3000); delay(200);
  noTone(buzzer);     delay(200);
  tone(buzzer, 3000); delay(100);
  noTone(buzzer);     delay(100);
  tone(buzzer, 3000); delay(100);
  noTone(buzzer);
}

void lowBattery() {
  if (buzzerTimer < 200000) tone(buzzer, 3500);
  else if (buzzerTimer < 400000) noTone(buzzer);
  else if (buzzerTimer < 600000) tone(buzzer, 3500);
  else if (buzzerTimer > 600000) noTone(buzzer);
  if (buzzerTimer > 1600000) buzzerTimer = 0;
}

void buzzerLoop() {
  buzzerTimer += interval;
  if (killFlag == true) {
    if (droneVolt > 5.1)  batteryCellDrone = 2;
    if (droneVolt > 9.0)  batteryCellDrone = 3;
    if (droneVolt > 13.0) batteryCellDrone = 4;
    if (droneVolt > 17.5) batteryCellDrone = 5;
    if (droneVolt > 22.0) batteryCellDrone = 6;
  }

  if (batteryCellDrone == 2 && droneVolt < 7.0)       buzzerCode = 1;
  else if (batteryCellDrone == 2 && droneVolt > 7.0)  buzzerCode = 0;

  if (batteryCellDrone == 3 && droneVolt < 10.5)      buzzerCode = 1;
  else if (batteryCellDrone == 3 && droneVolt > 10.5) buzzerCode = 0;

  if (batteryCellDrone == 4 && droneVolt < 14.0)      buzzerCode = 1;
  else if (batteryCellDrone == 4 && droneVolt > 14.0) buzzerCode = 0;

  if (batteryCellDrone == 5 && droneVolt < 17.5)      buzzerCode = 1;
  else if (batteryCellDrone == 5 && droneVolt > 17.5) buzzerCode = 0;

  if (batteryCellDrone == 6 && droneVolt < 21.0)      buzzerCode = 1;
  else if (batteryCellDrone == 6 && droneVolt > 21.0) buzzerCode = 0;

  if (droneVolt < 5.1) buzzerCode = 0;

  if (buzzerCode == 0) noTone(buzzer);
  if (buzzerCode == 1) lowBattery();
}
