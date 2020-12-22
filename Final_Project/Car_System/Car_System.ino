/* Board A-2 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OLED32.h>

OLED display(4, 5);
char buf[50];
const int ledR = 15;
const int ledB = 16;
const int pulseA = 12;
const int pulseB = 13;
volatile int lastEncoded = 0;
volatile float encoderValue = 20.0;

int carmode = 0;



char ssid[30] = "chanan_park";
char password[30] = "19970204";
const char* mqttServer = "54.87.52.22";
const int mqttPort = 1883;
char encoder_string[10];

WiFiClient espClient;
PubSubClient client(espClient);

int interval = 2000;
unsigned long lastPublished = -interval;

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
    if (client.connect("ESP8266_CarSystem")){ 
      Serial.println("connected");
    }
    else{
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
    }
   }
   pinMode(pulseA, INPUT_PULLUP);
   pinMode(pulseB, INPUT_PULLUP);
   pinMode(ledB, OUTPUT);
   pinMode(ledR, OUTPUT);
   attachInterrupt(pulseA, handleRotary, CHANGE);
   attachInterrupt (pulseB, handleRotary, CHANGE);
   display.begin();
   client.subscribe("esp/Project/evt/Mode");
}

ICACHE_RAM_ATTR void handleRotary(){
  int MSB = digitalRead(pulseA); 
  int LSB = digitalRead(pulseB);

  int encoded = (MSB << 1) | LSB;
  int sum  = (lastEncoded << 2) | encoded;
  if (carmode == 2){
    if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
      encoderValue = encoderValue + 0.5;
      sprintf(encoder_string, "%.1f", encoderValue);
      Serial.println(encoder_string);
      digitalWrite(ledR, HIGH);
      digitalWrite(ledB, LOW);
    }
    if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
      encoderValue = encoderValue - 0.5;
      sprintf(encoder_string, "%.1f", encoderValue);
      Serial.println(encoder_string);
      digitalWrite(ledB, HIGH);
      digitalWrite(ledR, LOW);
    }
    lastEncoded = encoded;

    if (encoderValue > 33.0) {         
      encoderValue = 33.0;     
    } 
    else if (encoderValue < 15.0 ) {         
      encoderValue = 15.0;     
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();
  if (carmode == 2){
    sprintf(buf, "Temp : %.1f C", encoderValue);
    display.print(buf, 1, 1);
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int len){
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for(int i=0;i<len;i++){
    Serial.print((char)payload[i]);
  }
  if ((char)payload[0] == '1'){
    Serial.println("Door OPEN");
    carmode = 1;
  }
  else if ((char)payload[0] == '2'){
    Serial.println("Power ON");
    carmode = 2;
  }
  else if ((char)payload[0] == '3'){
    Serial.println("POWER OFF");
    carmode = 3;
  }
  else if ((char)payload[4] == '4'){
    Serial.println("Door CLOSE");
    carmode = 4;
  }
}
