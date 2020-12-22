/* Board A-1 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <DHTesp.h>

#define servo_motor 15

char ssid[30] = "chanan_park";
char password[30] = "19970204";
const char* mqttServer = "54.87.52.22";
const int mqttPort = 1883;

int carmode = 0;
int flag = 0;

Servo servo;
int angle = 0;

DHTesp dht;
int interval = 2000;
unsigned long lastDHTReadMillis = 0;
unsigned long lastPublished = -interval;

float humidity = 0;
float temperature = 0;
char hum_string[10];
char tem_string[10];
char encoder_string[10];
const int pulseA = 12;
const int pulseB = 13;
const int pushSW = 2;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

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
    if (client.connect("ESP8266_Car")){ 
      Serial.println("connected");
    }
    else{
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
    }
  }
  dht.setup(14, DHTesp::DHT22);
  servo.attach(servo_motor);
  client.subscribe("esp/Project/evt/Mode");

  pinMode(pushSW, INPUT_PULLUP);     
  pinMode(pulseA, INPUT_PULLUP);     
  pinMode(pulseB, INPUT_PULLUP);
  attachInterrupt(pushSW, buttonClicked, FALLING);     
  attachInterrupt(pulseA, handleRotary, CHANGE);     
  attachInterrupt(pulseB, handleRotary, CHANGE);
}

ICACHE_RAM_ATTR void handleRotary(){ // 엑셀 역할
  int MSB = digitalRead(pulseA); 
  int LSB = digitalRead(pulseB);

  int encoded = (MSB << 1) | LSB;
  int sum  = (lastEncoded << 2) | encoded;
  
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue++;
    sprintf(encoder_string, "%d", encoderValue);
  }
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue--;
    sprintf(encoder_string, "%d", encoderValue);
  }
  lastEncoded = encoded;

  if (encoderValue > 200) {         
    encoderValue = 200;     
  } 
  else if (encoderValue < 0 ) {         
    encoderValue = 0;     
  }
}

ICACHE_RAM_ATTR void buttonClicked() 
{     
  flag=1;
  Serial.println("Break");

}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();
  unsigned long currentMillis = millis();
  if(carmode == 2){
    readDHT22();
    if (currentMillis - lastPublished >= interval){
      lastPublished = currentMillis;
      client.publish("cpu/Project/car/speed", encoder_string);
      client.publish("cpu/Project/car/temperature", tem_string);
      client.publish("cpu/Project/car/humidity", hum_string);
      if(flag ==1){
        while(encoderValue>0){
          encoderValue= encoderValue-10;
          delay(100);
          if( encoderValue<10){ encoderValue=0;}
          sprintf(encoder_string, "%d", encoderValue);
          client.publish("cpu/Project/car/speed", encoder_string);
        }
        flag = 0;
      }
    }
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
    servo.write(45);
    delay(1250);
    servo.write(90);
  }
  else if ((char)payload[0] == '2'){
    Serial.println("Power ON");
    carmode = 2;
    servo.write(135);
    delay(1250);
    servo.write(90);
  }
  else if ((char)payload[0] == '3'){
    Serial.println("POWER OFF");
    carmode = 3;
    servo.write(45);
    delay(1250);
    servo.write(90);
  }
  else{
    Serial.println("Door CLOSE");
    carmode = 4;
    servo.write(135);
    delay(1250);
    servo.write(90);
  }
}
void readDHT22(){
  unsigned long currentMillis = millis();
  if (currentMillis - lastDHTReadMillis >= interval){
    lastDHTReadMillis = currentMillis;

    humidity = dht.getHumidity();
    dtostrf(humidity, 3, 1, hum_string);
    Serial.println(hum_string);
    temperature = dht.getTemperature();
    dtostrf(temperature, 3, 1, tem_string);
    Serial.println(tem_string);
  }
}
