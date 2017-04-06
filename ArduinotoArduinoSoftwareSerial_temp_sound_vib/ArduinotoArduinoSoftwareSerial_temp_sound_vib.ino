#include <DHT.h>    //라이브러리 불러옴
#include <SoftwareSerial.h>
#define SLAVE 4

SoftwareSerial BTSerial(4, 5);
SoftwareSerial hb(18, 19);

int pin=13;            //온도 센서 핀번호
DHT dht(pin,DHT11);        
int sensorPin = 0;// 사운드 센서를 0번핀에 연결합니다.
int motorPin = 11; // 진동센서

void setup()
{
  hb.begin(9600);
  Serial.begin(9600);
  BTSerial.begin(9600);
  dht.begin();
  pinMode(motorPin, OUTPUT);
}

int count = 0;
float temp_average = 0.0;
int heart_average = 0, sound_average = 0; 
  
void loop()
{
   hb.listen();
   int hbdata;
   if(hb.available()>0)
   {
      hbdata=hb.read();
     //Serial.print("hbdata:   ");
     //Serial.println(hbdata);   
   }

  int err; // 오류가 생겼을때
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  int data;
  data= hbdata;
//  HBSerial.listen();
//  while(HBSerial.available())
//  {
//    data = HBSerial.read();//심박 센서
//    Serial.print(data);
//  }

  int value = analogRead(sensorPin); // 사운드 센서로부터 센서값
  //delay(100);//10ms 동안 대기
 // Serial.print("Sound: ");
 // Serial.println(value);//센서값을 시리얼 모니터로 출력하여 현재 센서값을 보여줍니다.
  //delay(100);//100ms 동안 대기
  
  if(isnan(temp)||isnan(humi)) //온도, 습도 읽어와서 표시
  {
    Serial.print("error");
  }
  else
  {
   // Serial.print("temperature:");
   // Serial.println(temp);
  }
  
  temp_average += temp;
  heart_average += data;
  sound_average += value;
  count++;
  
  if(count == 5)
  {
    temp_average = temp_average/5 + 10;
    heart_average /= 5;
    sound_average /= 5;

    String average;

    if(sound_average > 200 )
    {
      digitalWrite(motorPin, HIGH);
      average = String(sound_average) + String("#") + String(heart_average)
      + String("#") + String(temp_average) + String("#"); 
    }
    else
    {
      digitalWrite(motorPin, LOW);
      average = String(sound_average) + String("#") + String(heart_average) + String("#") + String(temp_average) + String("#");
    }
    
    Serial.println(average);
    BTSerial.listen();
    BTSerial.println(average);
    count = 0;

    temp_average = heart_average = sound_average = 0;
  }
  
  delay(1000);

}



