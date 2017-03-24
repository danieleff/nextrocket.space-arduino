#include "ethernet.h"
#include "wifi.h"
#include "displays.h"
#include "http.h"
#include "settings.h"

#include "util.h"
#include "config.h"

uint32_t button_intensity_millis = 0; // when was the last button pressed
uint32_t button_menu_millis = 0; // when was the last button pressed

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

  displays.setMessage(F("SETUP   "));
  
  settings.loadFromEEPROM();
  
  pinMode(PIN_BUTTON_INTENSITY, INPUT_PULLUP);
  pinMode(PIN_BUTTON_MENU, INPUT_PULLUP);
  
  #ifdef ESP8266 
    wifi.setup();
  #endif
  
  if (USE_ETHERNET) {
    net.setup();
  }
  
}

void loop () {
  if (USE_ETHERNET) {
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

void process_buttons() {
  
  if (millis() - button_intensity_millis> 200) {
    if (digitalRead(PIN_BUTTON_INTENSITY) == LOW) {
      button_intensity_millis = millis();

      //TODO intensity
      //digitDisplay.shutdown(0, false);
      //digitDisplay.clearDisplay(0);
      
      settings.intensity += 4;
      //digitDisplay.setIntensity(0, settings.intensity);
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



