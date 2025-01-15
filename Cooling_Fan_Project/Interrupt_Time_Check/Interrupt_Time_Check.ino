#include <Servo.h>
#define PIN_MOTOR_CTRL_0 3
#define PIN_MOTOR_CTRL_1 11
#define PIN_SWITCH_GREEN 8
#define PIN_SWITCH_YELLOW 7
#define PIN_SWITCH_RED 6
#define PIN_SWITCH_BLACK 2
#define PIN_SWITCH_BLUE 4
#define PIN_SERVO_CTRL 13

int prevSwitchGreen = HIGH;
int prevSwitchYellow = HIGH;
int prevSwitchRed = HIGH;
int prevSwitchBlue = HIGH;

int rotateFlag;
int taskStartTime = 0;
int taskEndTime = 0;
byte motorCtrlValue0;
byte servoPosition = 90;
Servo servoRotate;

void setup() {
  attachInterrupt(digitalPinToInterrupt(PIN_SWITCH_BLACK), isrSwitchBlack, RISING);
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
  // Rotate Control
  int currentSwitchBlue = digitalRead(PIN_SWITCH_BLUE);
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
  prevSwitchBlue = currentSwitchBlue;
  taskEndTime = micors();
  Serial.println("execution time is " + (String)(taskEndTime - taskStartTime) + "[us]");
  delay(100);
}

void isrSwitchBlack()
{
  rotateFlag = 0;
  servoRotate.write(90);
}
