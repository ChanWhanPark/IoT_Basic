# IoT_Basic
20-2 IoT기초   
   
한국산업기술대학교 전자공학부 임베디드시스템전공의 IoT기초에 연관된 자료 및 소스코드들을 담고 있습니다.   
이 저장소에는 ESP8266을 이용한 아두이노 소스들이 주로 사용되었으며, MQTT 서버 구축을 위한 파이썬 코드들도 일부 수록되어 있습니다.
   
### 20200921_3주차
시리얼 통신에 관한 과제 코드가 수록되어 있습니다. 과제 내용은 다음과 같습니다.
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
<SortwareSerial.h>과 그 기능에 대한 다양한 설명은 [Arduino SoftwareSerial][1]을 참고하시면 됩니다.
*****

### 20200928_4주차

*****

[1] : https://www.arduino.cc/en/Reference/softwareSerial
