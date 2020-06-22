int X1, Y1;
int X2, Y2;
int b1, b2;

int X1_offset, Y1_offset, X2_offset, Y2_offset;

double deadStick = 1024 * 0.05;

void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);

  X1_offset = analogRead(A2);
  Y1_offset = analogRead(A3);
  X2_offset = analogRead(A0);
  Y2_offset = analogRead(A1);
}

void loop() {
  X1 = analogRead(A2) - X1_offset;
  if (abs(X1) < deadStick) X1 = 0;
  if (X1 < 0) X1 = map(X1, -deadStick, 0 - X1_offset,    0, -500);
  if (X1 > 0) X1 = map(X1,  deadStick, 1023 - X1_offset, 0,  500);
  
  Y1 = analogRead(A3) - Y1_offset;
  if (abs(Y1) < deadStick) Y1 = 0;
  if (Y1 < 0) Y1 = map(Y1, -deadStick, 0 - Y1_offset,    0, -500);
  if (Y1 > 0) Y1 = map(Y1,  deadStick, 1023 - Y1_offset, 0,  500);
  
  X2 = analogRead(A0) - X2_offset;
  if (abs(X2) < deadStick) X2 = 0;
  if (X2 < 0) X2 = map(X2, -deadStick, 0 - X2_offset,    0, -500);
  if (X2 > 0) X2 = map(X2,  deadStick, 1023 - X2_offset, 0,  500);
  
  Y2 = analogRead(A1) - Y2_offset;
  if (abs(Y2) < deadStick) Y2 = 0;
  if (Y2 < 0) Y2 = map(Y2, -deadStick, 0 - Y2_offset,    0, -500);
  if (Y2 > 0) Y2 = map(Y2,  deadStick, 1023 - Y2_offset, 0,  500);

  b1 = !digitalRead(4);
  b2 = !digitalRead(5);

  Serial.print(X1); Serial.print("\t");
  Serial.print(Y1); Serial.print("\t");
  Serial.print(b1); Serial.print("\t");
  Serial.print(X2); Serial.print("\t");
  Serial.print(Y2); Serial.print("\t");
  Serial.print(b2); Serial.print("\t");
  Serial.println();
}
