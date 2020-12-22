#include <Servo.h>  //서보 라이브러리를 불러온다
#define TRIG 16    //트리거는 5번
#define ECHO 4    //에코는 4번
Servo servo;
 
int servoDirection = 1, rad = 0; // rad는 각도를 의미합니다.
 
void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  servo.attach(5); //서보모터 핀은 16번
  servo.write(0);
}
 
void loop() {
  long duration,distance;
  
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);  //2uS 동안 멈춤
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);  //10uS 동안 동작
  digitalWrite(TRIG, LOW);
 
  //5800이면 1m 입니다. 최대 기다리는 시간은 1,000,000 입니다.
  // 5800을 58로 나누게 되면 cm 단위가 됩니다.
  //long distance = pulseIn(ECHO, HIGH, 5800) / 58; //5800uS 동안 기다렸으므로 1미터 측정이 된다.
  duration = pulseIn (ECHO, HIGH,100000);
  distance = duration * 17 / 1000; 
  
  Serial.print(rad ); //초음파가 반사되어 돌아오는 시간을 보여줍니다.
  Serial.print(",");
  Serial.print(distance); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
  Serial.println(".");
 
  rad += servoDirection;
  if (rad > 180) {
    rad = 179;
    servoDirection = -2;
  }
  else if (rad < 0) {
    rad = 1;
    servoDirection = 2;
  }
  servo.write(rad);
  delay(10); //서보모터가 움직이는 걸리는 시간을 줍니다.
}
