/* Board B */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>

#define EEPROM_LENGTH 1024

char ssid[30] = "chanan_park";
char password[30] = "19970204";
const char* mqttServer = "54.87.52.22";
const int mqttPort = 1883;

// 초기 value 설정
int count = 0; // 스위치 눌린 횟수 판단 위함
char countString[5]; // 횟수를 publish
const int pushSW = 0; // 로터리 스위치

// Captive Potal 자료형 선언
char eRead[30];
byte len;
bool captive = true;
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

WiFiClient espClient;
PubSubClient client(espClient);

String responseHTML = ""
 "<!DOCTYPE html><html><head><title>2016146022</title></head><body><center>"
 "<p>My Car Login System</p>"
 "<form action='/button'>"
 "<p><input type='text' name='ssid' placeholder='CAR_NAME' onblur='this.value=removeSpaces(this.value);'></p>"
 "<p><input type='text' name='password' placeholder='WLAN Password'></p>"
 "<p><input type='submit' value='Submit'></p></form>"
 "<p>You can connect to relay server.</p></center></body>"
 "<script>function removeSpaces(string) {"
 " return string.split(' ').join('');"
 "}</script></html>";
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  EEPROM.begin(EEPROM_LENGTH);

  ReadString(0, 30);
  if (!strcmp(eRead, "")){
    setup_captive();
  }
  else{
    captive = false;
    strcpy(ssid, eRead);
    ReadString(30, 30);
    strcpy(password, eRead);
    setup_runtime();
  }
  
}

void setup_runtime(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int i=0;
   while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to WiFi..");
    if (i++ > 15){
      captive = true;
      setup_captive();
      return;
    }
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while(!client.connected()){
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266_Key")){ 
      Serial.println("connected");
    }
    else{
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
    }
  }
  // 초기 설정값
  pinMode(pushSW, INPUT_PULLUP);
  attachInterrupt(pushSW, buttonClicked, RISING);
  client.subscribe("esp/Project/evt/Mode");
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
 webServer.send(200, "text/plain", "OK");
 ESP.restart();
}

ICACHE_RAM_ATTR void buttonClicked()
{
  count++;
  delay(1000);
  Serial.printf("%d count\n", count);
  dtostrf(count, 2, 0, countString);
  client.publish("cpu/Project/evt/Button", countString);
  if (count == 4){
    count = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (captive){
    dnsServer.processNextRequest();
  }
  webServer.handleClient();
  if (!captive){
    client.loop();
  }
}

void callback(char* topic, byte* payload, unsigned int len){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i=0;i<len;i++){
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  Serial.println("==================");
  if ((char)payload[0] == '1') {
    Serial.println("Mode 1");
  }
  else if ((char)payload[0] == '2') {
    Serial.println("Mode 2");
  }
  else if ((char)payload[0] == '3') {
    Serial.println("Mode 3");
  }
  else{
    Serial.println("Mode 4");
  }
}

/* Captive SSID, PW 읽는 과정 */
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
