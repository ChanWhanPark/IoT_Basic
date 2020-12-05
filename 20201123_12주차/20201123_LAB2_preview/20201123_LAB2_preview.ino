#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "732-2.4G";
const char* password = "kpu123456!";
const char* mqttServer = "54.87.52.22";
const int mqttPort = 1883;

int interval = 5000;
unsigned long lastPublished = -interval;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  while(!client.connected()){
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client")){ 
      Serial.println("connected");
    }
    else{
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
    }
  }
  client.subscribe("esp/#");
}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - lastPublished >= interval){
    lastPublished = currentMillis;
    client.publish("esp/test", "Hello from ESP8266");
  }
}

void callback(char* topic, byte* payload, unsigned int len){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i=0;i<len;i++){
    Serial.print((char)payload[i]);
  }
  Serial.println("-----------------");
}
