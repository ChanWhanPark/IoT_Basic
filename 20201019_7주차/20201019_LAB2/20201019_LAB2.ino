#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

IPAddress apIP(192, 168, 1, 1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
  WiFi.softAP("chananpark");

  Serial.println("AP Started");
}

void loop() {
  // put your main code here, to run repeatedly:

}
