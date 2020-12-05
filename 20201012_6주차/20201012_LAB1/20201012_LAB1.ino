#include <Adafruit_NeoPixel.h>

#define ledPin 15
#define ledNum 4

Adafruit_NeoPixel pixels;

void setup() {
  // put your setup code here, to run once:
  pixels = Adafruit_NeoPixel(ledNum, ledPin, NEO_GRB + NEO_KHZ800);

  Serial.begin(115200);
  pixels.begin();
  delay(500);
  Serial.println("Starting");
}

void loop() {
  // put your main code here, to run repeatedly:
  int R, G, B;
  for (int i=0; i<ledNum; i++){
    R = random(0, 255);
    G = random(0, 255);
    B = random(0, 255);
    pixels.setPixelColor(i, pixels.Color(R, G, B));
    yield();
  }
  pixels.show();
  delay(500);
}
