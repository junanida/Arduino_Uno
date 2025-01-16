#include <IRremote.h> // IR 리모컨 라이브러리


#define PIN_IR_RECEIVE 2
#define PIN_MOTOR_LEFT_FORWARD 10
#define PIN_MOTOR_LEFT_BACKWARD 9
#define PIN_MOTOR_RIGHT_FORWARD 6
#define PIN_MOTOR_RIGHT_BACKWARD 5
#define PIN_LINE_DETECT_RIGHT A0
#define PIN_LINE_DETECT_LEFT A1


int motorSpeedLeft = 0;
int motorSpeedRight = 0;
int isStartLineTracing = 0;
int detectValueLeft = 0; // 왼쪽 센서 값
int detectValueRight = 0; // 오른쪽 센서 값


float Kp = 0.9, Ki = 0 , Kd = 8 ; // PID
int previousError = 0; // 이전 오차값
int integral = 0; // 적분 값


// 모터 제어 함수
void processMotor() {
  if (motorSpeedLeft >= 0) {
    analogWrite(PIN_MOTOR_LEFT_FORWARD, motorSpeedLeft);
    analogWrite(PIN_MOTOR_LEFT_BACKWARD, 0);
  } else {
    analogWrite(PIN_MOTOR_LEFT_FORWARD, 0);
    analogWrite(PIN_MOTOR_LEFT_BACKWARD, -motorSpeedLeft);
  }

  if (motorSpeedRight >= 0) {
    analogWrite(PIN_MOTOR_RIGHT_FORWARD, motorSpeedRight);
    analogWrite(PIN_MOTOR_RIGHT_BACKWARD, 0);
  } else {
    analogWrite(PIN_MOTOR_RIGHT_FORWARD, 0);
    analogWrite(PIN_MOTOR_RIGHT_BACKWARD, -motorSpeedRight);
  }
}

void setup() {
  IrReceiver.begin(PIN_IR_RECEIVE, ENABLE_LED_FEEDBACK);

  // 모터 핀 설정
  pinMode(PIN_MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_BACKWARD, OUTPUT);

  // 모터 초기화
  analogWrite(PIN_MOTOR_LEFT_FORWARD, 0);
  analogWrite(PIN_MOTOR_LEFT_BACKWARD, 0);
  analogWrite(PIN_MOTOR_RIGHT_FORWARD, 0);
  analogWrite(PIN_MOTOR_RIGHT_BACKWARD, 0);

  Serial.begin(115200); // 시리얼 모니터 시작
}

void loop() {

  if (IrReceiver.decode() == true) {
    if (IrReceiver.decodedIRData.protocol == NEC) {
      if (IrReceiver.decodedIRData.command == 0x16) { // *
        isStartLineTracing = 1;
      } else if (IrReceiver.decodedIRData.command == 0x0D) { // #
        isStartLineTracing = 0;
      }
    }
    IrReceiver.resume(); // 다음 신호 대기
  }

  // 라인 트레이싱 모드
  if (isStartLineTracing == 0) {
    motorSpeedLeft = 0;
    motorSpeedRight = 0;
    processMotor(); // 모터 정지
    return;
  }

  // 센서 값 읽기

  //detectValueLeft = analogRead(PIN_LINE_DETECT_LEFT);
  detectValueRight = analogRead(PIN_LINE_DETECT_RIGHT);  //오른쪽 센서만 사용

  int error = 250 - (detectValueRight   ); // 입력 - 출력
  integral += error; // 적분 값 누적
  int correction = Kp * error + Ki * integral + Kd * (error - previousError);
  previousError = error; //

  int baseSpeed = 200 ; // 기본 속도







  //음수 우회전, 양수 좌회전
  //  모터 속도 계산
  if (correction > 100)
  {
    motorSpeedLeft = baseSpeed + correction; // 왼쪽 모터 속도
    motorSpeedRight = baseSpeed - (correction * 0.6); // 오른쪽 모터 속도
  }
  else if (correction < -100)
  {
    motorSpeedLeft = baseSpeed + (correction * 0.6) ; // 왼쪽 모터 속도
    motorSpeedRight = baseSpeed - correction; // 오른쪽 모터 속도
  }
  else
  {
    motorSpeedLeft = 255;  // 왼쪽 모터 속도
    motorSpeedRight = 255; // 오른쪽 모터 속도
  }
  motorSpeedLeft = constrain(motorSpeedLeft, -128, 255);
  motorSpeedRight = constrain(motorSpeedRight, -128, 255);






  // 모터 실행
  processMotor();


  Serial.print("Left: ");
  Serial.print(detectValueLeft);

  Serial.print(" Right: ");
  Serial.print(detectValueRight);

  Serial.print(" Error: ");
  Serial.print(error);

  Serial.print(" correction: ");
  Serial.print(correction);

  Serial.print(" motorSpeedLeft: ");
  Serial.print(motorSpeedLeft);

  Serial.print(" motorSpeedRight: ");
  Serial.print(motorSpeedRight);

  Serial.println();


}
