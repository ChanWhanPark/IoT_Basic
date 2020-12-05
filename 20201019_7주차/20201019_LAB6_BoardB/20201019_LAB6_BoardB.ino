#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>


const char* ssid = "732-2.4G";
const char* password = "kpu123456!";
int flag = 0;
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
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());

  if (MDNS.begin("chananpark")){
    Serial.println("MDNS responder started");
  }

  server.on("/on", ON);
  server.on("/off", OFF);
  server.on("/toggle", TOGGLE);

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  pinMode(15, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  MDNS.update();
  server.handleClient(); 
}

void ON(){
  char mBuf[500];
  char tmplt[] = "<html><head><meta charset=\"utf-8\">"
                 "<title>Relay ON</title></head>"
                 "<body>"
                 "<script></script>"
                 "<center><p>"
                 "<head>Relay ON"
                 "</center>"
                 "</body></html>";
  sprintf(mBuf, tmplt);
  digitalWrite(15, HIGH);
  Serial.println("ON");
  server.send(200, "text/html", mBuf);
}

void OFF(){
  char mBuf[500];
  char tmplt[] = "<html><head><meta charset=\"utf-8\">"
                 "<title>Relay OFF</title></head>"
                 "<body>"
                 "<script></script>"
                 "<center><p>"
                 "<head>Relay OFF"
                 "</center>"
                 "</body></html>";
  sprintf(mBuf, tmplt);
  digitalWrite(15, LOW); 
  Serial.println("OFF");
  server.send(200, "text/html", mBuf);  
}

void TOGGLE(){
  char mBuf[500];
  char tmplt[] = "<html><head><meta charset=\"utf-8\">"
                 "<title>Relay 상태 반전</title></head>"
                 "<body>"
                 "<script></script>"
                 "<center><p>"
                 "<head>Relay Toggle"
                 "</center>"
                 "</body></html>";
  sprintf(mBuf, tmplt);
  if (flag == 0){
    digitalWrite(15, HIGH);
    flag = 1;
  }
  else if (flag == 1){
    digitalWrite(15, LOW);
    flag = 0;
  }
  Serial.println("serving");
  server.send(200, "text/html", mBuf);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}
