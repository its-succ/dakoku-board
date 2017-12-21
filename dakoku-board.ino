#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "RCS620S.h"

#include <I2CLiquidCrystal.h>
#include <Wire.h>

#include "config.h"

#define COMMAND_TIMEOUT 400
#define POLLING_INTERVAL 500

RCS620S rcs620s;

I2CLiquidCrystal lcd(20, (bool)false);

void setup() { 
  int ret;

  Serial.begin(115200);      // for RC-S620/S

  WiFi.begin(WIFI_SID, WIFI_PASSWORD);
  Serial.print("WiFi connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println(" connected"); 

  // initialize RC-S620/S
  ret = rcs620s.initDevice();
  Serial.println("RCS620S: " + String(ret));
  while (!ret) {}             // blocking
  Serial.println("INIT"); 

  // set up the LCD's number of columns and rows: 
  lcd.begin(8, 2);
  lcd.print("PLEASE");
  lcd.setCursor(0, 1);
  lcd.print("TOUCH !");
}

void loop() {
  int ret, i;

  // Polling
  rcs620s.timeout = COMMAND_TIMEOUT;
  ret = rcs620s.polling();

  if(ret) {
    char buf[9];
    sprintf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x", rcs620s.idm[0], rcs620s.idm[1], rcs620s.idm[2], rcs620s.idm[3], rcs620s.idm[4], rcs620s.idm[5], rcs620s.idm[6], rcs620s.idm[7]);
    Serial.println("IDM:" + String(buf));
    for (int i = 0; i < 16; i++) {
      lcd.setCursor((int)i % 8, (int)i / 8);
      lcd.print(String(buf[i]));
    }
  }

  rcs620s.rfOff();
  delay(POLLING_INTERVAL);
}

