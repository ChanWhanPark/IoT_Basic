# IoT_Basic
20-2 IoT기초   
   
한국산업기술대학교 전자공학부 임베디드시스템전공의 IoT기초에 연관된 자료 및 소스코드들을 담고 있습니다.   
이 저장소에는 ESP8266을 이용한 아두이노 소스들이 주로 사용되었으며, MQTT 서버 구축을 위한 파이썬 코드들도 일부 수록되어 있습니다.
   
### 20200921_3주차
ESP8266 간의 시리얼 통신에 관한 과제 코드가 수록되어 있습니다. 과제 내용은 다음과 같습니다.
> ESP8266 보드 2개를 이용하여 보드 A에서 1 또는 2의 명령을 보내면, 보드 B에서는 Relay를 키거나 끄는 동작을 수행합니다.
이 동작을 수행하기 위해서는 **SoftwareSerial**을 사용하여 두 보드간의 시리얼 통신이 가능하게 해야 합니다.   
시리얼 통신을 위해서는 두 보드의 RX, TX를 설정하여 이를 교차시켜주고, SoftwareSerial의 header와 자료형을 선언해줍니다.
<pre>
<code>
#include <SoftwareSerial.h>
const int RX = 12;
const int TX = 13;
SoftwareSerial swSer(RX, TX);
</code>
</pre>
<SortwareSerial.h>과 그 기능에 대한 다양한 설명은 [Arduino SoftwareSerial](https://www.arduino.cc/en/Reference/softwareSerial)을 참고하시면 됩니다.
*****

### 20200928_4주차
[지수가중이동평균](https://en.wikipedia.org/wiki/Moving_average)을 이용하여 빛 센서 값을 획득하고 이를 OLED에 표시하는 과제를 담고 있습니다.   
OLED에 데이터를 표시하기 위해서는 다음과 같은 라이브러리가 요구됩니다.
<pre>
<code>
#include <OLED.h>
OLED display(RX, TX);
</code>
</pre>
*****

### 20201005_5주차
ESP8266에 사용되는 OLED 활용의 기초, 동작 인식 센서, 그리고 온도 센서 DHT22를 활용한 코드 및 과제를 담고 있습니다.
온도 센서인 DHT22를 활용하기 위해서는 다음과 같은 라이브러리 요구됩니다.
<pre>
<code>
#include <DHTesp.h> // DHT 라이브러리
DHTesp dht; // 온도 센서 객체 선언

void setup(){
   dht.setup(PIN, DHTesp::DHT22); // DHT22로 모드 설정
}

</code>
</pre>

온/습도를 받아오기 위해서는 다음 코드를 사용한다.
<pre>
<code>
humidity = dht.getHumidity();
temperature = dht.getTemperature();
</code>
</pre>
*****

### 20201012_6주차
