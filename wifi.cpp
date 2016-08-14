#ifdef ESP8266

#include "wifi.h"

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

LaunchtimeWifi net;

ESP8266WiFiMulti WiFiMulti;

#define FAILED_REQUEST_RATE  20000 // Újrapróbálkozás milisec idő ha eddig nem sikerült lekérdezni
#define SUCCESS_REQUEST_RATE 60000 // Újrapróbálkozás milisec idő ha sikerült lekérdezni

static long mext_try_millis = 0;

extern long end_millis;

void LaunchtimeWifi::setup() {
/*
    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }
*/
    Serial.println("\n[Wifi add linksys]");

    WiFiMulti.addAP("linksys", "");

    Serial.println("\n[Wifi add SMC]");
    
    WiFiMulti.addAP("SMC", "");
    
    Serial.println("\n[Wifi setup end]");

}

void LaunchtimeWifi::loop() {
    // wait for WiFi connection
    
    if (millis() > mext_try_millis) {
      
      if((WiFiMulti.run() == WL_CONNECTED)) {
        
          mext_try_millis = millis() + FAILED_REQUEST_RATE;
        
          Serial.print("[HTTP] start\n");
          
          HTTPClient http;
  
          Serial.print("[HTTP] begin...\n");
          // configure traged server and url
          http.begin("http://danieleff.com/countdown/countdown.php?seconds=1"); //HTTP
  
          Serial.print("[HTTP] GET...\n");
          // start connection and send HTTP header
          int httpCode = http.GET();
  
          // httpCode will be negative on error
          if(httpCode > 0) {
              // HTTP header has been send and Server response header has been handled
              Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
              // file found at server
              if(httpCode == HTTP_CODE_OK) {
                  String payload = http.getString();
                  Serial.println(payload);
                  
                  end_millis = atol(payload.c_str()) * 1000 + millis();
                  mext_try_millis = millis() + SUCCESS_REQUEST_RATE;
                  Serial.println(end_millis);
              }
          } else {
              Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          }
  
          http.end();
      }

    }
    
}

#endif
