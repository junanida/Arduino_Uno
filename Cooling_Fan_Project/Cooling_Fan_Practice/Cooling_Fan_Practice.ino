#include <Servo.h>
#define PIN_MOTOR_CTRL_0 3
#define PIN_MOTOR_CTRL_1 11
#define PIN_SWITCH_GREEN 8
#define PIN_SWITCH_YELLOW 7
#define PIN_SWITCH_RED 6
#define PIN_SWITCH_BLACK 5
#define PIN_SWITCH_BLUE 4
#define PIN_SERVO_CTRL 13
int prevSwitchGreen = HIGH;
int prevSwitchYellow = HIGH;
int prevSwitchRed = HIGH;
int prevSwitchBlack = HIGH;
int prevSwitchBlue = HIGH;
int rotateFlag = 0;
byte motorCtrlValue0;
byte servoPosition = 90;
Servo servoRotate;

void setup() {
  servoRotate.attach(PIN_SERVO_CTRL);
  servoRotate.write(servoPosition);
  pinMode(PIN_SWITCH_GREEN, INPUT);
  pinMode(PIN_SWITCH_YELLOW, INPUT);
  pinMode(PIN_SWITCH_RED, INPUT);
  pinMode(PIN_SWITCH_BLACK, INPUT);
  pinMode(PIN_SWITCH_BLUE, INPUT);
  pinMode(PIN_MOTOR_CTRL_0, OUTPUT);
  pinMode(PIN_MOTOR_CTRL_1, OUTPUT);
  pinMode(PIN_SERVO_CTRL, OUTPUT);
  analogWrite(PIN_MOTOR_CTRL_0, 0);
  analogWrite(PIN_MOTOR_CTRL_1, 0);
  Serial.begin(115200);
}

void loop() {
  // Fan Control
  int currentSwitchGreen = digitalRead(PIN_SWITCH_GREEN);
  int currentSwitchYellow = digitalRead(PIN_SWITCH_YELLOW);
  int currentSwitchRed = digitalRead(PIN_SWITCH_RED);
  // 초록색 버튼 : 최대 속도의 50%로 Fan 회전
  if ((prevSwitchGreen == LOW) && (currentSwitchGreen == HIGH)) {
    analogWrite(PIN_MOTOR_CTRL_0, 127);
    Serial.println("50%");
  }
  // 노란색 버튼 : 최대 속도로 Fan 회전
  if ((prevSwitchYellow == LOW) && (currentSwitchYellow == HIGH)) {
    analogWrite(PIN_MOTOR_CTRL_0, 255);
    Serial.println("100%");
  }
  // 빨간색 버튼 : Fan 회전 정지
  if ((prevSwitchRed == LOW) && (currentSwitchRed == HIGH)) {
    analogWrite(PIN_MOTOR_CTRL_0, 0);
    Serial.println("Stop");
  }
  prevSwitchGreen = currentSwitchGreen;
  prevSwitchYellow = currentSwitchYellow;
  prevSwitchRed = currentSwitchRed;
  int currentSwitchBlack = digitalRead(PIN_SWITCH_BLACK);
  int currentSwitchBlue = digitalRead(PIN_SWITCH_BLUE);
  // 검정색 버튼 : Fan 목의 회전을 멈추고 정면을 바라보게 함
  if ((prevSwitchBlack == LOW) && (currentSwitchBlack == HIGH)) {
    rotateFlag = 0;
    servoPosition = 90;
    servoRotate.write(servoPosition);
    Serial.println("Fan Head Stop");
  }
  // 파란색 버튼 : Fan 목이 30~150도 사이에서 연속적으로 회전
  if ((prevSwitchBlue == LOW) && (currentSwitchBlue == HIGH))
  {
    rotateFlag = 1;
  }
  if (servoRotate.read() < 30 || servoRotate.read() > 150)
  {
    rotateFlag = rotateFlag * (-1);  
  }
  if (rotateFlag != 0)
  {
    servoRotate.write(servoRotate.read() + rotateFlag);
  }
  Serial.println("Rotate");
  prevSwitchBlack = currentSwitchBlack;
  prevSwitchBlue = currentSwitchBlue;
  delay(100);
}
