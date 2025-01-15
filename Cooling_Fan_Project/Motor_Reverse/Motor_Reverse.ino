#define PIN_MOTOR_CTRL_0 3
#define PIN_MOTOR_CTRL_1 11
#define PIN_SWITCH_GREEN 8
#define PIN_SWITCH_YELLOW 7

int prevSwitchGreen = HIGH;
int prevSwitchYellow = HIGH;
int fanSpeed = 0; // Fan speed (-255 to 255)

void setup() {
  pinMode(PIN_MOTOR_CTRL_0, OUTPUT);
  pinMode(PIN_MOTOR_CTRL_1, OUTPUT);
  pinMode(PIN_SWITCH_GREEN, INPUT_PULLUP);
  pinMode(PIN_SWITCH_YELLOW, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {
  int currentSwitchGreen = digitalRead(PIN_SWITCH_GREEN);
  int currentSwitchYellow = digitalRead(PIN_SWITCH_YELLOW);

  // 그린 버튼 누름
  if ((prevSwitchGreen == LOW) && (currentSwitchGreen == HIGH)) {
    fanSpeed += 10; // 속도 증가
    if (fanSpeed > 255) fanSpeed = 255; // 최대값 제한
    Serial.println(fanSpeed);
    controlFan(fanSpeed);
  }

  // 옐로우 버튼 누름
  if ((prevSwitchYellow == LOW) && (currentSwitchYellow == HIGH)) {
    fanSpeed -= 10; // 속도 감소
    if (fanSpeed < -255) fanSpeed = -255; // 최소값 제한
    Serial.println(fanSpeed);
    controlFan(fanSpeed);
  }

  prevSwitchGreen = currentSwitchGreen;
  prevSwitchYellow = currentSwitchYellow;
  delay(100);
}

void controlFan(int speed) {
  if (speed > 0) {
    analogWrite(PIN_MOTOR_CTRL_0, speed); // 정방향 회전
    analogWrite(PIN_MOTOR_CTRL_1, 0);
  } else if (speed < 0) {
    analogWrite(PIN_MOTOR_CTRL_0, 0);
    analogWrite(PIN_MOTOR_CTRL_1, -speed); // 역방향 회전
  } else {
    analogWrite(PIN_MOTOR_CTRL_0, 0); // 정지
    analogWrite(PIN_MOTOR_CTRL_1, 0);
  }
}
