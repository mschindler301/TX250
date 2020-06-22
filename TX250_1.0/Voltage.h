double v, v2;
double rcVolt = 5;
double r1 = 49900;
double r2 = 10000;
double vRef = 5.0;

int batteryCell;

void voltLoop() {
  // Read Analog Pin //
  v = (analogRead(A4) * vRef) / 1024.0;
  v2 = v / (r2 / (r1 + r2));

  // Digital Low Pass Filter //
  rcVolt = rcVolt + 0.1*(v2 - rcVolt);
}
