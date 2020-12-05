#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include <ESP8266HTTPClient.h>
#include <DHTesp.h>
#include <String.h>

#define EEPROM_LENGTH 1024

char eRead[30];
byte len;
char ssid[] = "732-2.4G";
char password[] = "kpu123456!";
char url[] = "http://54.87.52.22:8086";

bool captive = true;
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
HTTPClient http;
DHTesp dht;

int interval = 2000;
unsigned long lastDHTReadMillis = 0;
float humidity = 0.0;
float temperature = 0;
int light = 0;
float alpha = 0.4;

ICACHE_RAM_ATTR void GPIO0(){
  SaveString(0, "");
  ESP.restart();
}

String responseHTML = ""
 "<!DOCTYPE html><html><head><title>2016146022</title></head><body><center>"
 "<p>CaptivePotal & Home IoT</p>"
 "<form action='/button'>"
 "<p><input type='text' name='ssid' placeholder='SSID' onblur='this.value=removeSpaces(this.value);'></p>"
 "<p><input type='text' name='password' placeholder='WLAN Password'></p>"
 "<p><input type='text' name='url' placeholder='influxDB url'></p>"
 "<p><input type='submit' value='Submit'></p></form>"
 "<p>You can connect to Home IoT server.</p></center></body>"
 "<script>function removeSpaces(string) {"
 " return string.split(' ').join('');"
 "}</script></html>";
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  EEPROM.begin(EEPROM_LENGTH);

  ReadString(0, 30);
  if(!strcmp(eRead, "")){
    setup_captive();
  }else{
    captive = false;
    strcpy(ssid, eRead);
    ReadString(30, 30);
    strcpy(password, eRead);
    ReadString(60, 30);
    strcpy(url, eRead);
    setup_runtime();
  }
}

void setup_runtime(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int i = 0;
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    if (i++ > 15){
      captive = true;
      setup_captive();
      return;
    }
  }
  Serial.println("");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
  http.begin((String)url + "/write?db=weatherDB");
  dht.setup(14, DHTesp::DHT22);

  if (MDNS.begin("channanpark")){
    Serial.println("MDNS responder started");
  }
  attachInterrupt(0, GPIO0, FALLING);
  while(1){
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
}

void setup_captive(){
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("chananpark");

  dnsServer.start(DNS_PORT, "*", apIP);

  webServer.on("/button", button);

  webServer.onNotFound([](){
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.begin();
  Serial.println("Captive Portal Started");
}

void button(){
 Serial.println("button pressed");
 Serial.println(webServer.arg("ssid"));
 SaveString( 0, (webServer.arg("ssid")).c_str());
 SaveString(30, (webServer.arg("password")).c_str());
 SaveString(60, (webServer.arg("url")).c_str());
 webServer.send(200, "text/plain", "OK");
 ESP.restart();
}

void SaveString(int startAt, const char* id) {
 for (byte i = 0; i <= strlen(id); i++) {
 EEPROM.write(i + startAt, (uint8_t) id[i]);
 }
 EEPROM.commit();
}

void ReadString(byte startAt, byte bufor) {
 for (byte i = 0; i <= bufor; i++) {
  eRead[i] = (char)EEPROM.read(i + startAt);
 }
 len = bufor;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (captive){
    dnsServer.processNextRequest();
  }
  webServer.handleClient();
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
