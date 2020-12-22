#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define EA 14  // 모터드라이버 EA 핀, 아두이노 우노 보드 디지털 5번 핀에 연결
#define M_IN1 13  // 모터드라이버 IN1 핀, 아두이노 우노 보드 디지털 4번 핀에 연결
#define M_IN2 12  // 모터드라이버 IN2 핀, 아두이노 우노 보드 디지털 3번 핀에 연결
int motorA_vector = 1;  // DC모터의 회전방향이 반대일 시 1을 0으로
// "1"을 "0"으로 바꿔주면 DC모터의 회전방향이 바뀜.

char ssid[30] = "chanan_park";
char password[30] = "19970204";
const char* mqttServer = "54.87.52.22";
const int mqttPort = 1883;
char car_speed[10];
int carmode = 0;
int carspeed;

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
  pinMode(EA, OUTPUT);  // EA와 연결된 핀 출력 설정
  pinMode(M_IN1, OUTPUT);  // IN1과 연결된 핀 출력 설정
  pinMode(M_IN2, OUTPUT);  // IN2와 연결된 핀 출력 설정
  client.subscribe("cpu/Project/car/speed");
}
void loop()  // 무한루프
{
  client.loop();
  
  if (carspeed > 0){
      motorforward();
      
    }
  else {
      motorstop();
  }
 
  
}
void callback(char* topic, byte* payload, unsigned int len){
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for(int i=0;i<len;i++){
     Serial.print((char)payload[i]);
     car_speed[i] = (char)payload[i];
    }
    Serial.println("");
    carspeed = atoi(car_speed);
}
 
void motorforward(){
  digitalWrite(EA, HIGH);  // 모터구동 ON
  digitalWrite(M_IN1, LOW);  // IN1에 HIGH(or LOW)
  digitalWrite(M_IN2, HIGH);  // IN2에 LOW(or HIGH)
  Serial.println("정회전");
  delay(5000);

}

void motorstop(){
  digitalWrite(EA, LOW);  // 모터구동 OFF
  digitalWrite(M_IN1, LOW);  // IN1에 LOW
  digitalWrite(M_IN2, LOW);  // IN2에 LOW
  Serial.println("정지");
  delay(5000);

}
