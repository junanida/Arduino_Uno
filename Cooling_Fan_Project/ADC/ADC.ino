void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int switchGreen = analogRead(A0);
  int switchYellow = analogRead(A1);
  Serial.print("Green:");
  Serial.print(switchGreen);
  Serial.print(", Yellow:");
  Serial.println(switchYellow);
  delay(1000);
}
