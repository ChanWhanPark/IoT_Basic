#include <OLED32.h>
OLED display(4, 5);

// EMA 적용
float alpha = 0.4;
int x0 = 0; 
char buf[10];

void setup() {
  Serial.begin(115200);
  display.begin();
  pinMode(15, OUTPUT);
}

void loop() {
  int x1 = analogRead(A0);
  x0 = x0 * alpha + (1 - alpha) * x1;
  Serial.printf("%d %d\n", x1, x0);

  if (x0 < 600){
    digitalWrite(15, HIGH);
    sprintf(buf, "%4d Relay ON ", x0);
    display.print(buf, 1, 1);
  }
  else{
    digitalWrite(15, LOW);
    sprintf(buf, "%4d Relay OFF", x0);
    display.print(buf, 1, 1);
  }
  delay(100);
}
