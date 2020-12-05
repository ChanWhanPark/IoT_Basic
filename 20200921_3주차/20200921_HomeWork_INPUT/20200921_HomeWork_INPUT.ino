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
}

void loop() {
  // put your main code here, to run repeatedly:
  char data = Serial.read();
  swSer.write(data);

  if (data == '1'){
    Serial.println("Relay ON");
  }
  else if (data == '2'){
    Serial.println("Relay OFF");
  }
}
