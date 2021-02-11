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
ESP8266에서 사용할 수 있는 다양한 센서들(네오픽셀 LED, 초음파 센서)의 사용법을 담았다.     
네오픽셀은 <Adafruit_NeoPixel.h> 라이브러리를 사용하여 다양한 기능을 수행할 수 있으며, 초음파 센서는 별도의 라이브러리 설치 없이 함수를 이용하여 사용이 가능하다.
LAB3~4에서는 로터리 엔코더에 관한 내용이 나오는데, 샤프트의 회전에 따라 두 신호의 Rising과 Falling 신호의 차이로 값을 증가시키거나 감소시킬 수 있다.
*****

### 20201019_7주차
ESP8266에서 Wi-Fi를 사용하여 인터넷 통신을 하는 기능들을 담고 있으며, 본격적으로 어려워지는 내용이기도 하다.   
우선, Wi-Fi 모드를 수행하기 위해 <ESP8266WiFi.h> 라이브러리를 사용한다는 것을 기억해야 한다.
또한, STA 모드와 AP 모드에 따라 ESP8266이 어떤 기능을 수행하는지 살펴보는 것이 중요하다.
이후, mDNS를 이용하여 여러 기기들이 접속할 수 있게 만들어주기도 하며, 이를 위해서는 <ESP8266mDNS.h> 헤더를 사용한다.
LAB1과 LAB2에서 각각의 모드에서 어떤 동작을 하는지 살펴볼 수 있다.
LAB3부터는 HTTP를 이용해 클라이언트 정보를 가져오는 동작을 실행한다. 이를 위해 <ESP8266HTTPClient.h> 헤더를 사용한다.
또한, 이후에는 [<ESP8266WebServer.h> 라이브러리](https://github.com/esp8266/Arduino/tree/master/)를 이용해 웹서버를 구현하는 것도 가능하다. 
기본적인 HTML 문법을 어느정도 알고 있다면, 기존 예제에 대해 추가적인 기능을 붙여 더욱 다양한 기능들을 만들 수도 있다.
특히, LAB5는 실시간으로 웹서버에 DHT22를 통한 온습도를 출력하는 것을, LAB6은 두 개의 ESP8266 보드를 이용하여 다른 보드를 제어하는 기능을 수행할 수 있다.
*****

### 20201102_9주차
파이썬을 이용하여 웹서버를 구현하는 예제를 담았다.

*****

### 20201109_10주차
