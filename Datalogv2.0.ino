#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "DHT.h"
#include "Wire.h"
#include "RTClib.h"
#include "SPI.h"

#define DHTPIN 2    
#define DHTTYPE DHT22   
//#define BUZZER 7

DHT dht(DHTPIN, DHTTYPE);
File myFile;
RTC_DS3231 RTC;

const int chipSelect = 4;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
const long interval = 17000;
const long timeinterval = 3000;
int led1 = 8;
int led2 = 9;
int led3 = 10;
int sdcard = 5;
int vccdht = 6;
int rtc = 7; 
int dosmt = 1;


void setup(){
  int statuscheck = 0;
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(sdcard,OUTPUT);
  pinMode(vccdht,OUTPUT);
  pinMode(rtc,OUTPUT);
  
  Serial.begin(9600);
  dht.begin();
  Wire.begin();
  digitalWrite(sdcard,HIGH);
  digitalWrite(vccdht,HIGH);
  digitalWrite(rtc,HIGH);
  digitalWrite(led3,LOW);
  
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    delay(3000);
    LED(10, 100, led2);
  }else{ 
    Serial.println("initialization done.");
    digitalWrite(led2,HIGH);
    statuscheck = statuscheck + 1;
  }

    if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    delay(3000);
    LED(10, 100, led3);
  }else {
    Serial.println("RTC is running!");
    digitalWrite(led3,HIGH);
    statuscheck = statuscheck + 1;
  }
  
  if(statuscheck == 2){
    Serial.println("Board Ready");
    digitalWrite(led1,HIGH);
  }
  writesd();
  delay(2000);
}

void loop(){
  digitalWrite(vccdht,HIGH);
  int rt = 15; // round trip time (sec) 30 = 1 min 300 = 10 min 900 = 15 min  
    delay(1000);
    //err(1);
    for(int i=0;i<rt ;i++){
      readdata();
        if(i%10==0){
        Serial.println("x");
        LED3(6, 100, led1,led2,led3);
        }
      LED(1, 500, led1);
      Serial.println(i);
    }
    
    writesd();

}

void writesd() {

  myFile = SD.open("datalog.txt", FILE_WRITE);
  
  if (myFile) {
//    delay(dht.getMinimumSamplingPeriod());
//    float humidity = dht.getHumidity(); // ดึงค่าความชื้น
//    float temperature = dht.getTemperature(); // ดึงค่าอุณหภูมิ
    
    float h = dht.readHumidity();
    // Read temperature as Celsius
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    }


    DateTime now = RTC.now();
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(' ');
    myFile.print("\t");
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(" \t");

    myFile.print("TB: ");
    myFile.print(RTC.getTemperature()); // คำสั่งดึงอุณหภูมิออกมาแสดง
    myFile.print(" C \t");
    
//    myFile.print(dht.getStatusString());
//    myFile.print("\t");
//    myFile.print(humidity, 1);
//    myFile.print("\t\t");
//    myFile.print(temperature, 1);
//    myFile.print("\t\t");
//    myFile.println(dht.toFahrenheit(temperature), 1);

  

    myFile.print(h);
    myFile.print(" %\t");
    myFile.print(t);
    myFile.println(" *C ");


    myFile.close(); // ปิดไฟล์
    LED(5, 200, led1);
    Serial.println("done.");
  }
  else {
    Serial.println("error opening testdatalock.txt");
    err(1);
  }
}

void readdata() {
  
//  delay(dht.getMinimumSamplingPeriod());
   
    float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  
//  float humidity = dht.getHumidity(); // ดึงค่าความชื้น
//  float temperature = dht.getTemperature(); // ดึงค่าอุณหภูมิ
  
    DateTime now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print("\t");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(" \t");


    Serial.print("TB: ");
    Serial.print(RTC.getTemperature()); // คำสั่งดึงอุณหภูมิออกมาแสดง
    Serial.print(" C \t");


  

    Serial.print(h);
    Serial.print(" % \t");
    Serial.print(t);
    Serial.println(" *C ");
    
//    Serial.print(dht.getStatusString());
//    Serial.print("\t");
//    Serial.print(humidity, 1);
//    Serial.print("\t\t");
//    Serial.print(temperature, 1);
//    Serial.print("\t\t");
//    Serial.println(dht.toFahrenheit(temperature), 1);

}

void LED(int i, int j, int pi) { //led (round, delay, pinport)
  int Ledpin = pi;
  int y = 0;
  for (y = 0; y < i; y++) {
    digitalWrite(Ledpin, HIGH);
    delay(j);
    digitalWrite(Ledpin, LOW);
    delay(j);
    //Serial.println("");
  }
}


void LED3(int i, int j, int pi,int pi2,int pi3) { //led (round, delay, pinport)
  int Ledpin = pi;
  int Ledpin2 = pi2;
  int Ledpin3 = pi3;
  int y = 0;
  for (y = 0; y < i; y++) {
    digitalWrite(Ledpin, HIGH);
    digitalWrite(Ledpin2, HIGH);
    digitalWrite(Ledpin3, HIGH);
    delay(j);
    digitalWrite(Ledpin, LOW);
    digitalWrite(Ledpin2, LOW);
    digitalWrite(Ledpin3, LOW);
    delay(j);
    //Serial.println("");
  }
}

void  err(int te){
    switch(te){
    case 1: 
    Serial.println("sd card error");
      LED(10, 50, led2);
      LED(1, 100, led2);
      LED(10, 50, led2);
      LED(1, 100, led2);
    break;
    
    }
  } 
