#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <DHTesp.h>

const char* ssid = "732-2.4G";
const char* password = "kpu123456!";

ESP8266WebServer server(80);

DHTesp dht;
float temperature = 0;
float humidity = 0;
int interval = 2000;
unsigned long lastDHTReadMillis = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.setup(14, DHTesp::DHT22);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conntected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());

  if (MDNS.begin("chananpark")){
    Serial.println("MDNS responder started");
  }

  server.on("/thermo", thermo);
  
  server.on("/inline",[](){
    server.send(200, "text/plain", "Hello from the inline function\n");
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  readDHT22();
  MDNS.update();
  server.handleClient();
}

void thermo(){
  char mBuf[500];
  char tmplt[] = "<html><head><meta charset=\"utf-8\">"
                 "<meta http-equiv='refresh' content='5'/>"
                 "<title>온습도계</title></head>"
                 "<body>"
                 "<script></script>"
                 "<center><p>"
                 "<head>온도 : </head>%.1f"
                 "<p><head>습도 : </head>%.1f"
                 "</center>"
                 "</body></html>";
   sprintf(mBuf, tmplt, temperature, humidity);
   Serial.println("serving");
   server.send(200, "text/html", mBuf);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void readDHT22(){
  unsigned long currentMillis = millis();
  if (currentMillis - lastDHTReadMillis >= interval){
    lastDHTReadMillis = currentMillis;
    temperature = dht.getTemperature();
    humidity = dht.getHumidity();
  }
 }
