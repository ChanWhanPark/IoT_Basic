#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

const char* ssid = "IoT518";
const char* password = "iot123456";

ESP8266WebServer server(80);

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
  Serial.print("Conntected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());

  if (MDNS.begin("chananpark")){
    Serial.println("MDNS responder started");
  }

  server.on("/handleRoot", handleRoot);
  
  server.on("/inline",[](){
    server.send(200, "text/plain", "Hello\n");
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  MDNS.update();
  server.handleClient();
}

void handleRoot(){
  String message = (server.method() == HTTP_GET)?"GET":"POST";
  message += " " + server.uri() + "\n";
  for (uint8_t i=0;i<server.args();i++){
    message += " " + server.argName(i) + " : " + server.arg(i) + "\n";
  }
  message += "\nHello from ESP8266!\n";
  server.send(200, "text/plain", message);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}
