#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "RCS620S.h"

#include <I2CLiquidCrystal.h>
#include <Wire.h>

#include "config.h"

#define COMMAND_TIMEOUT 400
#define POLLING_INTERVAL 100
#define BEAT 300

static const int PiezoPin = 16;        // for Speaker
static const int ArriveButtonPin = 13; // for pushing when arrive at work
static const int LeaveButtonPin = 14;  // for pushing when leave work at  

RCS620S rcs620s;

I2CLiquidCrystal lcd(20, (bool)false);

void setup() { 
  int ret;

  Serial.begin(115200);      // for RC-S620/S

  WiFi.begin(WIFI_SID, WIFI_PASSWORD);
  Serial.print("WiFi connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  Serial.println(" connected"); 

  // initialize RC-S620/S
  ret = rcs620s.initDevice();
  Serial.println("RCS620S: " + String(ret));
  while (!ret) {}             // blocking
  Serial.println("INIT"); 
  
  pinMode(ArriveButtonPin, INPUT_PULLUP);
  pinMode(LeaveButtonPin, INPUT_PULLUP);

  // set up the LCD's number of columns and rows: 
  initLCD();
}

void loop() {

  if (digitalRead(ArriveButtonPin) == LOW) {
    Serial.println("出社");
    
      tone(PiezoPin, 2000, 100); // 3番ピンに接続されたスピーカーを2000Hzで鳴らす
      delay(100);    // 100ms(1秒)待つ
      tone(PiezoPin, 1000, 100); // 3番ピンに接続されたスピーカーを1000Hzで鳴らす
      delay(100);    // 100ms(1秒)待つ

    lcd.setCursor(0, 0);
    lcd.kana("ｼｭｯｼｬ   ");
    lcd.setCursor(0, 1);
    lcd.print("        ");
    delay(2000);
    initLCD();
  }
  if (digitalRead(LeaveButtonPin) == LOW) {
    Serial.println("退社");
    tone(PiezoPin, 880, 500);
    lcd.setCursor(0, 0);
    lcd.kana("ﾀｲｼｬ    ");
    lcd.setCursor(0, 1);
    lcd.print("        ");
    delay(2000);
    initLCD();
  }

  // Polling
  rcs620s.timeout = COMMAND_TIMEOUT;
  int ret = rcs620s.polling();

  if(ret) {
    tone(PiezoPin, 3520, 300);
    char buf[9];
    sprintf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x", rcs620s.idm[0], rcs620s.idm[1], rcs620s.idm[2], rcs620s.idm[3], rcs620s.idm[4], rcs620s.idm[5], rcs620s.idm[6], rcs620s.idm[7]);
    Serial.println("IDM:" + String(buf));
    for (int i = 0; i < 16; i++) {
      lcd.setCursor((int)i % 8, (int)i / 8);
      lcd.print(String(buf[i]));
    }
    delay(500);
  }

  rcs620s.rfOff();
  delay(POLLING_INTERVAL);
}

void initLCD() {
  lcd.begin(8, 2);
  lcd.print("PLEASE  ");
  lcd.setCursor(0, 1);
  lcd.print("TOUCH ! ");
}

