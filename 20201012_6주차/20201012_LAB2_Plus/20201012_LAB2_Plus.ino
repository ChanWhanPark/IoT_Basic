#include <OLED32.h>

const int trigPin = 13;
const int echoPin = 12;

long duration;
float distance;

OLED display(4, 5);
char buf1[50];
char buf2[50];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(200);
  display.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  sprintf(buf1, "Dur: %6d", duration);
  display.print(buf1, 1, 1);
  distance = duration * 0.017;

  sprintf(buf2, "Dis: %.1f cm", distance);
  display.print(buf2, 2, 1);

  delay(1000);
}
