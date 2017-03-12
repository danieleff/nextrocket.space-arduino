#include "ethernet.h"
#include "wifi.h"
#include "displays.h"
#include "http.h"
#include "settings.h"

#include "util.h"
#include "config.h"

uint32_t button_intensity_millis = 0; // when was the last button pressed
uint32_t button_menu_millis = 0; // when was the last button pressed

boolean use_ethernet = true;

Displays displays;

void setup () {
  pinMode(PIN_ETHERNET_CS, OUTPUT);
  digitalWrite(PIN_ETHERNET_CS, HIGH);

  pinMode(PIN_GRAPHICS_CS, OUTPUT);
  digitalWrite(PIN_GRAPHICS_CS, HIGH);

  pinMode(PIN_TOUCH_CS, OUTPUT);
  digitalWrite(PIN_TOUCH_CS, HIGH);
  
  Serial.begin(115200);

  displays.setup();
  
  Serial.println(F("setup()"));

  displays.write(F("SETUP   "));
  
  //settings.loadFromEEPROM();
  
  pinMode(PIN_BUTTON_INTENSITY, INPUT_PULLUP);
  pinMode(PIN_BUTTON_MENU, INPUT_PULLUP);
  pinMode(PIN_BUTTON_DEMO, INPUT_PULLUP);
  
  if (digitalRead(PIN_BUTTON_DEMO) == LOW) {
    //use_ethernet = false;
  }

  #ifdef ESP8266 
    wifi.setup();
  #endif
  
  if (use_ethernet) {
    net.setup();
  }
  
}

void loop () {
  if (use_ethernet) {
    httpServer.loop();
    httpClient.loop();
  }

  #ifdef ESP8266 
    wifi.loop();
  #endif

  displays.loop();

  process_buttons();

  //Serial.println("Test");
  //delay(500);
  //blink();
}

//TODO put into displays.cpp
#include "digit_display.h"
extern DigitDisplay digitDisplay;

void process_buttons() {
  
  if (millis() - button_intensity_millis> 200) {
    if (digitalRead(PIN_BUTTON_INTENSITY) == LOW) {
      button_intensity_millis = millis();

      digitDisplay.reset();
      settings.intensity += 4;
      digitDisplay.setIntensity(settings.intensity);
    }
  }


  if (millis() - button_menu_millis> 200) {
    if (digitalRead(PIN_BUTTON_MENU) == LOW) {
      button_menu_millis = millis();
      selected_launch_changed_millis = millis();
      
      displays.refresh();
      
      settings.selected_menu++;
      if (settings.selected_menu >= settings.launch_count) {
        settings.selected_menu = SELECTED_CYCLE;
      }

      settings.saveToEEPROM();
    }
  }
}



