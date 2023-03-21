#include <Servo.h>
//#include <SoftwareSerial.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"
#include "HX711.h"
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DHTTYPE DHT11 //DHT11
#define WIFI_SSID "DT0105"
#define WIFI_PASSWORD "12349000"
#define FIREBASE_HOST "project3-2803f-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "EfPCerYRlx3ymow5DJZZhh17eI3SnxTkCJiPcs6P"

FirebaseData firebaseData;
String path = "/";
FirebaseJson json;

Servo s1;

const int DHTPIN = 2;
const int LOADCELL_DOUT_PIN = 14;
const int LOADCELL_SCK_PIN = 12;

HX711 scale;
float scaleData;
float x;
int i = 0;
//SoftwareSerial mySerial(D5,D6);


//LiquidCrystal_I2C lcd(0x27, 16, 2);

DHT dht(DHTPIN, DHTTYPE);
int h1, t1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //mySerial.begin(9600);
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
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(481);
  scale.tare();
//  lcd.clear();
//  lcd.begin();
//  lcd.backlight();
//  lcd.home();
//  lcd.print("Nhiet Do: ");
//  lcd.setCursor(0,1);
//  lcd.print("Do Am: ");
  dht.begin();
  s1.attach(D2);
  Firebase.setInt(firebaseData, path + "/weight", 0);
}

void readHumTem(){
  delay(1000);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // độ T
  int h1, t1;
  if(h -(int)h >= 0.5){
    h1 = (int)h + 1;
  }else{
    h1 = int(h);
  }
  if(t -(int)t >= 0.5){
    t1 = (int)t + 1;
  }else{
    t1 = int(t);
  }
  Firebase.setInt(firebaseData, path + "/hum", h1);
  Firebase.setInt(firebaseData, path + "/tem", t1);
//  lcd.setCursor(10, 0);
//  lcd.print(t1);
//  lcd.print(" *C");
//  lcd.setCursor(7, 1);
//  lcd.print(h1);
//  lcd.print(" %");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t Temperature: ");
  Serial.print(t);
  Serial.print(" *C");
  Serial.println();
}

void readScale(){
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  scaleData = scale.get_units(10);
  Serial.println(scaleData, 5);
  //Firebase.setFloat(firebaseData, path + "/scale", scaleData);
  if(Firebase.getInt(firebaseData, path + "/weight")){
      x = (float)firebaseData.intData();
  }
  if(x != 0 && i == 0){
    s1.write(180);
    i = 1;
  }
  if(scaleData >= x && i == 1){
    s1.write(0);
    Firebase.setFloat(firebaseData, path + "/weight", 0);
    i = 0;
  } 
  scale.power_down();
  delay(500);
  scale.power_up();
}

//void read_uart(){
//  //String st = "";
//  Serial.println("Read Uart");
//  while(mySerial.available()){
//    uartFlag[i] = mySerial.read();
//    //st +=  (char)mySerial.read();
//    Serial.println(mySerial.read());
//    i++;
//    if(i == 5){
//      i = 0;
//      Serial.println();
//      break;
//    }
//  }
//}

void loop() {
  // put your main code here, to run repeatedly:
  readHumTem();
  readScale();
  //read_uart();
//  delay(1000);
//  float h = dht.readHumidity();
//  float t = dht.readTemperature(); // độ T
//  if(h -(int)h >= 0.5){
//    h1 = (int)h + 1;
//  }else{
//    h1 = int(h);
//  }
//  if(t -(int)t >= 0.5){
//    t1 = (int)t + 1;
//  }else{
//    t1 = int(t);
//  }
//  lcd.setCursor(10, 0);
//  lcd.print(t1);
//  lcd.print(" *C");
//  lcd.setCursor(7, 1);
//  lcd.print(h1);
//  lcd.print(" %");
//  Serial.print("Humidity: ");
//  Serial.print(h);
//  Serial.print(" %\t Temperature: ");
//  Serial.print(t);
//  Serial.print(" *C");
//  Serial.println();
}
