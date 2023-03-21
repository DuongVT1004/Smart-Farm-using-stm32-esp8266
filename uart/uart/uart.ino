#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "FirebaseESP8266.h"

#define WIFI_SSID "DT0105"
#define WIFI_PASSWORD "12349000"
#define FIREBASE_HOST "project3-2803f-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "EfPCerYRlx3ymow5DJZZhh17eI3SnxTkCJiPcs6P"

FirebaseData firebaseData;
//WiFiClient client;
String path = "/";
FirebaseJson json;

int flag[5];
unsigned long currentTime = 0;
uint8_t x = 0;

SoftwareSerial mySerial(D5, D6);
uint8_t i = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  if (!Firebase.beginStream(firebaseData, path)){
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println();
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial.begin(9600);
  delay(100);
  while(mySerial.available()){
    flag[i] = (int)mySerial.read() - 48;
    Serial.print(flag[i]);
    //if(millis() - currentTime > 200){
    //}
    i++;
    if(i == 5){
      Serial.println();
      i = 0;
      break;
    }
  }
  Firebase.setInt(firebaseData, path + "/warn", flag[1]);
  Firebase.setInt(firebaseData, path + "/led", flag[2]);
  Firebase.setInt(firebaseData, path + "/pump", flag[3]);
  if(Firebase.getInt(firebaseData, path + "/pump_web")){
      x = firebaseData.intData();
      Serial.println(x);
      mySerial.print(x);
  }
  //uint8_t y = x;
  mySerial.end();
}
