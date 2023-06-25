#include <SoftwareSerial.h>
#include <Servo.h>
// (TXD, RXD) 연결시 아두이노 블루투스 묘둘을 반대로 연결해야 함!!
#define RXD A2  // RX연결핀번호 : A1
#define TXD A1  // TX연결핀번호 : A2
// rxPin = 블루투스 tx Pin, txPin = rxPin
SoftwareSerial bt(RXD, TXD);

#define piezo A0 // 피에조부저 핀 정의
unsigned long startPiezoMillis; // 피에조부저 작동된 시간

int runningFallingPiezo = 0; // 피에조부저 작동여부
int piezoFallingTones[] = {987, 830}; // 물체 감지 시 작동하는 음

int runningPumpPiezo = 0; // 피에조부저 작동여부
int piezoPumpTones[] = {493, 493, 0, 0, 493, 493, 0, 0, 493, 493, 415, 415, 659, 659}; // 음료 추출 완료 시 작동하는 음

Servo servoFir; // 서보모터1 정의
Servo servoSec; // 서보모터2 정의
Servo servoThr; // 서보모터3 정의
Servo servoFou; // 서보모터4 정의

int sensorFir = 6; // 적외선 센서1 핀 정의
int sensorSec = 7; // 적외선 센서2 핀 정의
int sensorThr = 8; // 적외선 센서3 핀 정의
int sensorFou = 9; // 적외선 센서4 핀 정의

int pumpFir = 10; // 모터1 핀 정의
int pumpSec = 11; // 모터2 핀 정의
int pumpThr = 12; // 모터3 핀 정의
int pumpFou = 13; // 모터4 핀 정의

boolean runningServoFir = false; // 서보모터1 작동여부
boolean runningServoSec = false; // 서보모터2 작동여부
boolean runningServoThr = false; // 서보모터3 작동여부
boolean runningServoFou = false; // 서보모터4 작동여부

boolean runningPumpFir = false; // 워터펌프1 작동여부
boolean runningPumpSec = false; // 워터펌프2 작동여부
boolean runningPumpThr = false; // 워터펌프3 작동여부
boolean runningPumpFou = false; // 워터펌프4 작동여부

unsigned long startServoFirMillis; // 서보모터1 작동된 시간
unsigned long startServoSecMillis; // 서보모터2 작동된 시간
unsigned long startServoThrMillis; // 서보모터3 작동된 시간
unsigned long startServoFouMillis; // 서보모터4 작동된 시간

unsigned long startFirMillis; // 워터펌프1 작동된 시간
unsigned long startSecMillis; // 워터펌프2 작동된 시간
unsigned long startThrMillis; // 워터펌프3 작동된 시간
unsigned long startFouMillis; // 워터펌프4 작동된 시간

unsigned long servoDelay = 500; // 서보모터 작동시간
unsigned long pumpDelay = 60000; // 워터펌프 작동시간
unsigned long piezoDelay = 120; // 피에조부저 딜레이

int startAngle = 50; // 서보모터 초기 각도
int endAngle = 130; // 서보모터 끝 각도

void setup() {
  Serial.begin(9600); // 시리얼 초기화
  bt.begin(9600);   // 소프트웨어 시리얼 통신
  servoFir.attach(2); // 서보모터1 핀 정의
  servoSec.attach(3); // 서보모터2 핀 정의
  servoThr.attach(4); // 서보모터3 핀 정의
  servoFou.attach(5); // 서보모터4 핀 정의
  pinMode(sensorFir,INPUT); // 적외선 센서1 핀 정의
  pinMode(sensorSec,INPUT); // 적외선 센서2 핀 정의
  pinMode(sensorThr,INPUT); // 적외선 센서3 핀 정의
  pinMode(sensorFou,INPUT); // 적외선 센서4 핀 정의
  pinMode(pumpFir,OUTPUT); // 워터펌프1 핀 정의
  pinMode(pumpSec,OUTPUT); // 워터펌프2 핀 정의
  pinMode(pumpThr,OUTPUT); // 워터펌프3 핀 정의
  pinMode(pumpFou,OUTPUT); // 워터펌프4 핀 정의
  servoFir.write(startAngle); // 서보모터1 초기 각도 지정
  servoSec.write(startAngle); // 서보모터2 초기 각도 지정
  servoThr.write(startAngle); // 서보모터3 초기 각도 지정
  servoFou.write(startAngle); // 서보모터4 초기 각도 지정
}

void loop() {
                                                    // --블루투스로 데이터 값 불러와 서보모터 작동시키는 부분--
  if (bt.available()) {                                     // 블루투스 모듈 쪽에서 데이터를 수신한 경우
    byte byteValue = bt.read();                             // 데이터를 byteValue에 byte 값으로 저장
    switch(byteValue){
      case '1':                                             // byteValue가 '1'인 경우
        servoFir.write(endAngle);                           // 서보모터1 작동
        startServoFirMillis = millis();                     // 서보모터1이 작동한 시간 측정
        runningServoFir = true;                             // 서보모터1이 작동중임을 표시
        break;
      case '2':                                             // byteValue가 '2'인 경우
        servoSec.write(endAngle);                           // 서보모터2 작동
        startServoSecMillis = millis();                     // 서보모터2가 작동한 시간 측정
        runningServoSec = true;                             // 서보모터2가 작동중임을 표시
        break;
      case '3':                                             // byteValue가 '3'인 경우
        servoThr.write(endAngle);                           // 서보모터3 작동
        startServoThrMillis = millis();                     // 서보모터3이 작동한 시간 측정
        runningServoThr = true;                             // 서보모터3이 작동중임을 표시
        break;
      case '4':                                             // byteValue가 '4'인 경우
        servoFou.write(endAngle);                           // 서보모터4 작동
        startServoFouMillis = millis();                     // 서보모터4가 작동한 시간 측정
        runningServoFou = true;                             // 서보모터4가 작동중임을 표시
        break;
      default:                                              // byteValue가 지정되지 않은 값일 경우
        Serial.println("[경고] 없는 서보모터 번호");         // 경고 출력
        break;
    }
  }
                                                    // --서보모터 작동중일 경우 시간 체크 후 초기값으로 되돌리는 부분--
  if(runningServoFir){                                      // 서보모터1이 작동중일 경우
    unsigned long currentMillis = millis();                 // 현재 시간 기록
    if(currentMillis - startServoFirMillis >= servoDelay){  // 서보모터1이 작동한 후로부터 현재 시간이 서보모터 작동시간(servoDelay)만큼 지났을 경우
      servoFir.write(startAngle);                           // 서보모터1을 초기 각도로 되롤린다
      runningServoFir = false;                              // 서보모터1이 작동중이 아님을 표시
    }
  }
  if(runningServoSec){                                      // 서보모터2가 작동중일 경우
    unsigned long currentMillis = millis();                 // 현재 시간 기록
    if(currentMillis - startServoSecMillis >= servoDelay){  // 서보모터2가 작동한 후로부터 현재 시간이 서보모터 작동시간(servoDelay)만큼 지났을 경우
      servoSec.write(startAngle);                           // 서보모터2를 초기 각도로 되롤린다
      runningServoSec = false;                              // 서보모터2가 작동중이 아님을 표시
    }
  }
  if(runningServoThr){                                      // 서보모터3이 작동중일 경우
    unsigned long currentMillis = millis();                 // 현재 시간 기록
    if(currentMillis - startServoThrMillis >= servoDelay){  // 서보모터3이 작동한 후로부터 현재 시간이 서보모터 작동시간(servoDelay)만큼 지났을 경우
      servoThr.write(startAngle);                           // 서보모터3을 초기 각도로 되롤린다
      runningServoThr = false;                              // 서보모터3이 작동중이 아님을 표시
    }
  }
  if(runningServoFou){                                      // 서보모터4가 작동중일 경우
    unsigned long currentMillis = millis();                 // 현재 시간 기록
    if(currentMillis - startServoFouMillis >= servoDelay){  // 서보모터4가 작동한 후로부터 현재 시간이 서보모터 작동시간(servoDelay)만큼 지났을 경우
      servoFou.write(startAngle);                           // 서보모터4를 초기 각도로 되롤린다
      runningServoFou = false;                              // 서보모터4가 작동중이 아님을 표시
    }
  }
                                                    // --적외선 센서 감지시 펌프 작동시키는 부분--
  if(digitalRead(sensorFir)==LOW){                          // 적외선 센서1에서 물체를 감지할 경우
    Serial.println("First Pump ON");                        // 워터펌프1이 작동함을 출력
    digitalWrite(pumpFir,HIGH);                             // 워터펌프1 작동
    startFirMillis = millis();                              // 워터펌프1이 작동한 시간 측정
    runningPumpFir = true;                                  // 워터펌프1이 작동중임을 표시
    runningFallingPiezo = 1;                                // 피에조 부저 작동
    startPiezoMillis = millis();                            // 피에조 부저 작동 시간 표시
  }
  if(digitalRead(sensorSec)==LOW){                          // 적외선 센서2에서 물체를 감지할 경우
    Serial.println("Second Pump ON");                       // 워터펌프2가 작동함을 출력
    digitalWrite(pumpSec,HIGH);                             // 워터펌프2 작동
    startSecMillis = millis();                              // 워터펌프2가 작동한 시간 측정
    runningPumpSec = true;                                  // 워터펌프2가 작동중임을 표시
    runningFallingPiezo = 1;                                // 피에조 부저 작동
    startPiezoMillis = millis();                            // 피에조 부저 작동 시간 표시
  }
  if(digitalRead(sensorThr)==LOW){                          // 적외선 센서3에서 물체를 감지할 경우
    Serial.println("Third Pump ON");                        // 워터펌프3이 작동함을 출력
    digitalWrite(pumpThr,HIGH);                             // 워터펌프3 작동
    startThrMillis = millis();                              // 워터펌프3이 작동한 시간 측정
    runningPumpThr = true;                                  // 워터펌프3이 작동중임을 표시
    runningFallingPiezo = 1;                                // 피에조 부저 작동
    startPiezoMillis = millis();                            // 피에조 부저 작동 시간 표시
  }
  if(digitalRead(sensorFou)==LOW){                          // 적외선 센서4에서 물체를 감지할 경우
    Serial.println("Fourth Pump ON");                       // 워터펌프4가 작동함을 출력
    digitalWrite(pumpFou,HIGH);                             // 워터펌프4 작동
    startFouMillis = millis();                              // 워터펌프4가 작동한 시간 측정
    runningPumpFou = true;                                  // 워터펌프4가 작동중임을 표시
    runningFallingPiezo = 1;                                // 피에조 부저 작동
    startPiezoMillis = millis();                            // 피에조 부저 작동 시간 표시
  }
                                                    // --워터펌프 작동중일 경우 시간 체크 후 중지시키는 부분--
  if(runningPumpFir){                                       // 워터펌프1이 작동중일 경우
    unsigned long currentMillis = millis();                 // 현재 시간 기록
    if(currentMillis - startFirMillis >= pumpDelay){        // 워터펌프1이 작동한 후로부터 현재 시간이 워터펌프 작동시간(pumpDelay)만큼 지났을 경우
      Serial.println("First Pump OFF");                     // 워터펌프1이 중지됨을 표시
      digitalWrite(pumpFir,LOW);                            // 워터펌프1 중지
      runningPumpFir = false;                               // 워터펌프1이 작동중이 아님을 표시
      runningPumpPiezo = 1;                                 // 피에조 부저 작동
      startPiezoMillis = millis();                          // 피에조 부저 작동 시간 표시
    }
  }
  if(runningPumpSec){                                       // 워터펌프2가 작동중일 경우
    unsigned long currentMillis = millis();                 // 현재 시간 기록
    if(currentMillis - startSecMillis >= pumpDelay){        // 워터펌프2가 작동한 후로부터 현재 시간이 워터펌프 작동시간(pumpDelay)만큼 지났을 경우
      Serial.println("Second Pump OFF");                    // 워터펌프2가 중지됨을 표시
      digitalWrite(pumpSec,LOW);                            // 워터펌프2 중지
      runningPumpSec = false;                               // 워터펌프2가 작동중이 아님을 표시
      runningPumpPiezo = 1;                                 // 피에조 부저 작동
      startPiezoMillis = millis();                          // 피에조 부저 작동 시간 표시
    }
  }
  if(runningPumpThr){                                       // 워터펌프3이 작동중일 경우
    unsigned long currentMillis = millis();                 // 현재 시간 기록
    if(currentMillis - startThrMillis >= pumpDelay){        // 워터펌프3이 작동한 후로부터 현재 시간이 워터펌프 작동시간(pumpDelay)만큼 지났을 경우
      Serial.println("Third Pump OFF");                     // 워터펌프3이 중지됨을 표시
      digitalWrite(pumpThr,LOW);                            // 워터펌프3 중지
      runningPumpThr = false;                               // 워터펌프3이 작동중이 아님을 표시
      runningPumpPiezo = 1;                                 // 피에조 부저 작동
      startPiezoMillis = millis();                          // 피에조 부저 작동 시간 표시
    }
  }
  if(runningPumpFou){                                       // 워터펌프4가 작동중일 경우
    unsigned long currentMillis = millis();                 // 현재 시간 기록
    if(currentMillis - startFouMillis >= pumpDelay){        // 워터펌프4가 작동한 후로부터 현재 시간이 워터펌프 작동시간(pumpDelay)만큼 지났을 경우
      Serial.println("Fourth Pump OFF");                    // 워터펌프4가 중지됨을 표시
      digitalWrite(pumpFou,LOW);                            // 워터펌프4 중지
      runningPumpFou = false;                               // 워터펌프4가 작동중이 아님을 표시
      runningPumpPiezo = 1;                                 // 피에조 부저 작동
      startPiezoMillis = millis();                          // 피에조 부저 작동 시간 표시
    }
  }

  if(runningFallingPiezo > 0){                       // --물체 감지 시 작동하는 피에조 부저--
    tone(piezo, piezoFallingTones[runningFallingPiezo-1]);  // 처음 톤 및 현재 톤 설정
    unsigned long currentMillis = millis();                 // 현재 시간 기록
    if(currentMillis - startPiezoMillis >= piezoDelay){     // 피에조부저가 작동한 후로부터 현재 시간이 피에조 부저의 딜레이(piezoDelay)만큼 지났을 경우
      tone(piezo, piezoFallingTones[runningFallingPiezo]);  // 현재 톤 설정
      startPiezoMillis = millis();                          // 현재 시간 기록
      runningFallingPiezo++;                                // 다음 톤으로 이동
    }
    if(runningFallingPiezo > sizeof(piezoFallingTones)/sizeof(int)){ // 피에조 부저의 음이 끝났을 경우
      noTone(piezo);                                        // 피에조 부저를 끔
      runningFallingPiezo = 0;                              // 피에조 부저 완료 및 변수 초기화
    }
  }
  
  if(runningPumpPiezo > 0){                       // --음료 추출 완료 시 작동하는 피에조 부저--
    tone(piezo, piezoPumpTones[runningPumpPiezo-1]);        // 처음 톤 및 현재 톤 설정
    unsigned long currentMillis = millis();                 // 현재 시간 기록
    if(currentMillis - startPiezoMillis >= piezoDelay){     // 피에조부저가 작동한 후로부터 현재 시간이 피에조 부저의 딜레이(piezoDelay)만큼 지났을 경우
      tone(piezo, piezoPumpTones[runningPumpPiezo]);        // 현재 톤 설정
      startPiezoMillis = millis();                          // 현재 시간 기록
      runningPumpPiezo++;                                   // 다음 톤으로 이동
    }
    if(runningPumpPiezo > sizeof(piezoPumpTones)/sizeof(int)){  // 피에조 부저의 음이 끝났을 경우
      noTone(piezo);                                        // 피에조 부저를 끔
      runningPumpPiezo = 0;                                 // 피에조 부저 완료 및 변수 초기화
    }
  }
}