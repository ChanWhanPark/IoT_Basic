#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

const int pushSW = 2;
const char* ssid = "732-2.4G";
const char* password = "kpu123456!";
int n = LOW;
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());

  if (MDNS.begin("chananpark")){
    Serial.println("MDNS responder started");
  }
  
  pinMode(pushSW, INPUT);
}
  
void loop() {
  // put your main code here, to run repeatedly:
  MDNS.update();
  n = digitalRead(pushSW);
  if (n == LOW){
    callback();
  }
}

void callback(){
  WiFiClient client;
  HTTPClient http;
  if (http.begin(client, "http://172.30.1.35/toggle")){
      int httpCode = http.GET();
      if (httpCode == 200){
        Serial.println("Must be Toggled");
      }
      http.end();
    }
   else{
      Serial.printf("[HTTP] Unable to connect\n");
   }
   delay(1000);
}
