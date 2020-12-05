#include <OLED32.h>

const int           pulseA = 12; 
const int           pulseB = 13; 
const int           pushSW = 2;
const int           Relay = 15; 
volatile int        lastEncoded = 0; 
volatile long       encoderValue = 0; 

OLED display(4, 5);
char buf1[20];
char buf2[20];

ICACHE_RAM_ATTR void handleRotary() 
{ 
  int MSB = digitalRead(pulseA); 
  int LSB = digitalRead(pulseB);
  
  int encoded = (MSB << 1) | LSB;
  int sum  = (lastEncoded << 2) | encoded;
  
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++; 
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;     
  lastEncoded = encoded;
  
  if (encoderValue > 255) {         
    encoderValue = 255;     
  } 
  else if (encoderValue < 0 ) {         
    encoderValue = 0;     
  }
}

ICACHE_RAM_ATTR void buttonClicked() 
{     
  Serial.println("pushed"); 
} 

void setup() 
{     
  Serial.begin(115200);     
  pinMode(pushSW, INPUT_PULLUP);     
  pinMode(pulseA, INPUT_PULLUP);     
  pinMode(pulseB, INPUT_PULLUP);
  pinMode(Relay, OUTPUT);
  attachInterrupt(pushSW, buttonClicked, FALLING);     
  attachInterrupt(pulseA, handleRotary, CHANGE);     
  attachInterrupt(pulseB, handleRotary, CHANGE);
  display.begin(); 
} 

void loop() 
{     
  int bright = analogRead(0);
  sprintf(buf1, "Bright : %4d", bright);
  display.print(buf1, 1, 1);
  sprintf(buf2, "Rotary : %4d", encoderValue);
  display.print(buf2, 2, 1);
  if (encoderValue > bright){
    digitalWrite(Relay, HIGH);
  }
  else{
    digitalWrite(Relay, LOW);
  }
  delay(500);  
} 
