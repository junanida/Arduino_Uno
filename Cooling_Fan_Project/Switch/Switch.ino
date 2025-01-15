void setup() {
  // put your setup code here, to run once:
  pinMode(8, INPUT);
  pinMode(7, INPUT);
  pinMode(6, INPUT);
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int switchGreen = digitalRead(8);
  int switchYellow = digitalRead(7);
  int switchRed = digitalRead(6);
  int switchBlack = digitalRead(5);
  int switchBlue = digitalRead(4);
  Serial.print("Green:");
  Serial.print(switchGreen);
  Serial.print(", Yellow:");
  Serial.print(switchYellow);
  Serial.print(", Red:");
  Serial.print(switchRed);
  Serial.print(", Black:");
  Serial.print(switchBlack);
  Serial.print(", Blue:");
  Serial.println(switchBlue);
  delay(1000);
}
