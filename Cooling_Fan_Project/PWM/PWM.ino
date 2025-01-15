void setup() {
  pinMode(10, OUTPUT);
  analogWrite(10, 127); // 2.5V
  Serial.begin(115200);
}

void loop() {
  int value = analogRead(A0);
  Serial.println(value / 1023.0 * 5.0);
  delay(500);
}
