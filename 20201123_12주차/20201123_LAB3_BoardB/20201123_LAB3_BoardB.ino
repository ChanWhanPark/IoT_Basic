#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "chanan_park";
const char* password = "00000000";
const char* mqttServer = "54.87.52.22";
const int mqttPort = 1883;

int interval = 2500;
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
    if (client.connect("ESP8266_B")){ 
      Serial.println("connected");
    }
    else{
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
    }
  }
  
  pinMode(15, OUTPUT);
  client.subscribe("cpu/2016146022/cmd/relay");
}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();


}

void callback(char* topic, byte* payload, unsigned int len){
  char state[4];
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i=0;i<len;i++){
    Serial.print((char)payload[i]);
    state[i] = (char)payload[i];
  }
  Serial.println("");
  if (state[1] == 'N'){
    digitalWrite(15, HIGH);
    Serial.println("Relay ON");
  }
  else if (state[1] == 'F'){
    digitalWrite(15, LOW);
    Serial.println("Relay OFF");
  }
}
