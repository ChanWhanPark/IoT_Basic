#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHTesp.h>
#include <String.h>

char ssid[] = "916-2.4G";
char password[] = "kpu123456!";

HTTPClient http;
DHTesp dht;
int interval = 2000;
unsigned long lastDHTReadMillis = 0;
float humidity = 0.0;
float temperature = 0;
int light = 0;
float alpha = 0.4;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
  http.begin("http://54.87.52.22:8086/write?db=weatherDB");
  dht.setup(14, DHTesp::DHT22);
}

void loop() {
  // put your main code here, to run repeatedly:
  readDHT22();
  readLight();
  
  http.addHeader("Content-Type", "text/plain");
  int httpCode1 = http.POST("cpu,host=server01,region=temperature temperature="+(String)temperature);
  int httpCode2 = http.POST("cpu,host=server01,region=humidity humidity="+(String)humidity);
  int httpCode3 = http.POST("cpu,host=server01,region=light light="+(String)light);
  String payload = http.getString();
  Serial.printf("%d\t %d\t %d\n", httpCode1, httpCode2, httpCode3);
  Serial.println(payload);
  Serial.printf("%.1f\t %.1f\t %d\n", temperature, humidity, light);
  http.end();

  delay(5000);
}

void readDHT22(){
  unsigned long currentMillis = millis();
  if (currentMillis - lastDHTReadMillis >= interval){
    lastDHTReadMillis = currentMillis;

    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
  }
}

void readLight(){
  int x1 = analogRead(A0);
  int x0 = x0 * alpha + (1 - alpha) * x1;
  light = x0;
}
