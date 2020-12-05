#include <SoftwareSerial.h>
const int RX = 12;
const int TX = 13;
const int RELAY = 15;
char data;
SoftwareSerial swSer(RX, TX);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  swSer.begin(9600);
  pinMode(15, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (swSer.available()){
    data = swSer.read();
  }
  if (data == '1'){
    digitalWrite(15, HIGH);
  }
  else if (data == '2'){
    digitalWrite(15, LOW);
  }
}
