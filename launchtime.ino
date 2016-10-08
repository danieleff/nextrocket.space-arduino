#include "ethernet.h"
#include "wifi.h"
#include "displays.h"
#include "http.h"
#include "settings.h"



#ifdef ESP8266 
  //Pins with esp8266 wifi chip
  const byte PIN_BUTTON_INTENSITY = 5;
  const byte PIN_BUTTON_MENU = 2;
  const byte PIN_BUTTON_DEMO = 4;

#else 
  //Pins with arduino nano + ethernet
  const byte PIN_BUTTON_INTENSITY = 8;
  const byte PIN_BUTTON_MENU = 9;
  const byte PIN_BUTTON_DEMO = 4;
#endif


uint32_t button_intensity_millis = 0; // when was the last button pressed
uint32_t button_menu_millis = 0; // when was the last button pressed

boolean use_ethernet = true;

void setup () {
  Serial.begin(115200);
  Serial.println(F("setup()"));

  displays.setup();
  
  displays.write(F("SETUP   "));
  
  //settings.loadFromEEPROM();
  
  pinMode(PIN_BUTTON_INTENSITY, INPUT_PULLUP);
  pinMode(PIN_BUTTON_MENU, INPUT_PULLUP);
  pinMode(PIN_BUTTON_DEMO, INPUT_PULLUP);
  
  if (digitalRead(PIN_BUTTON_DEMO) == LOW) {
    use_ethernet = false;
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
  yield();
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
      
      settings.selected_menu++;
      if (settings.selected_menu >= settings.launch_count) {
        settings.selected_menu = SELECTED_CYCLE;
      }

      settings.saveToEEPROM();
    }
  }
}



