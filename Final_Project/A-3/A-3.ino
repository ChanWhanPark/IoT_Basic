/* Board A-3 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

#define TRIG 16
#define ECHO 4


void motorforward();
void motorstop();
int servoDirection = 1;
int rad = 0;

char ssid[30] = "chanan_park";
char password[30] = "19970204";
const char* mqttServer = "54.87.52.22";
const int mqttPort = 1883;
char car_speed[10];
int carmode = 0;
int carspeed;

Servo servo;
WiFiClient espClient;
PubSubClient client(espClient);


void setup()  // 초기화
{
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
    if (client.connect("ESP8266_Motor")){ 
      Serial.println("connected");
    }
    else{
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
    }
   }
  client.subscribe("esp/Project/evt/mode");
  client.subscribe("cpu/Project/car/speed");
  servo.attach(D0);
}
void loop()  // 무한루프
{
  client.loop();  
  long duration,distance;

  //초음파
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);  //10uS 동안 동작
  digitalWrite(TRIG, LOW);
 
  //5800이면 1m 입니다. 최대 기다리는 시간은 1,000,000 입니다.
  // 5800을 58로 나누게 되면 cm 단위가 됩니다.
  //long distance = pulseIn(ECHO, HIGH, 5800) / 58; //5800uS 동안 기다렸으므로 1미터 측정이 된다.
  duration = pulseIn (ECHO, HIGH, 100000);
  distance = duration * 17 / 1000;
    
  //Serial.print(rad ); //초음파가 반사되어 돌아오는 시간을 보여줍니다.
  //Serial.print(",");
  //Serial.print(distance); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
  //Serial.println(".");
 
  rad += servoDirection;
  if (rad > 180) {
    rad = 179;
    servoDirection = -2;
  }
  else if (rad < 0) {
    rad = 1;
    servoDirection = 2;
  }
  servo.write(rad);
  delay(10); //서보모터가 움직이는 걸리는 시간을 줍니다.
}

void callback(char* topic, byte* payload, unsigned int len){
  Serial.print("Topic: ");
  Serial.println(topic);
  if (strcmp(topic, "cpu/Project/car/speed") == 0){
    Serial.print("Message: ");
    for(int i=0;i<len;i++){
      Serial.print((char)payload[i]);
      car_speed[i] = (char)payload[i];
    }
    carspeed = atoi(car_speed);
    if (car_speed > 0){
      Serial.println("전진");
     }
    else if (car_speed == 0) {Serial.println("정지");}
  }
  else if (strcmp(topic, "esp/Project/evt/mode") == 0){
    if (strcmp((char*)payload, "2") == 0){
      Serial.println("Power ON\n");
      carmode = 2;
    }
  }
}
