#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

const char* ssid = "IoT518";
const char* password = "iot123456";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println();
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();


  Serial.println("");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());

  if (MDNS.begin("chananpark")){
    Serial.println("MDNS responder started");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  MDNS.update();

}
