#include <DHTesp.h>
#include <OLED32.h>

DHTesp dht;
OLED display(4, 5);

int interval = 2000;
unsigned long lastDHTReadMillis = 0;
float humidity = 0;
float temperature = 0;
char buf[50];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.setup(14, DHTesp::DHT22);
  Serial.println("Temperature(C)\t Humidity (%)");
  display.begin();
  
  delay(1000);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  readDHT22();

  Serial.printf("%.1f C\t %.1f %%\n", temperature, humidity);
  sprintf(buf, "Tem : %.1f C", temperature);
  display.print(buf, 1, 1);
  sprintf(buf, "Hum : %.1f %%", humidity);
  display.print(buf, 2, 1);
  delay(500);
}

void readDHT22(){
  unsigned long currentMillis = millis();
  if (currentMillis - lastDHTReadMillis >= interval){
    lastDHTReadMillis = currentMillis;

    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
  }
}
