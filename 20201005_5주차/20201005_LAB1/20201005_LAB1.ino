#include <Wire.h>
#include <OLED32.h>

OLED display(4, 5);

void setup() {
  // put your setup code here, to run once:
  display.begin();
  display.print("Hello World");
  display.print("Welcome to IoT Class!!", 3, 1);
  delay(2000);
  display.off();
  delay(2000);
  display.on();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
