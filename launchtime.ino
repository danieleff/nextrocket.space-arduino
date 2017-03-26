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

boolean demo_mode = false;

void setup () {
  pinMode(PIN_ETHERNET_CS, OUTPUT);
  digitalWrite(PIN_ETHERNET_CS, HIGH);

  pinMode(PIN_GRAPHICS_CS, OUTPUT);
  digitalWrite(PIN_GRAPHICS_CS, HIGH);

  pinMode(PIN_TOUCH_CS, OUTPUT);
  digitalWrite(PIN_TOUCH_CS, HIGH);
  
  pinMode(PIN_BUTTON_INTENSITY, INPUT_PULLUP);
  pinMode(PIN_BUTTON_MENU, INPUT_PULLUP);
  
  Serial.begin(115200);

  displays.setup();
  
  Serial.println(F("setup()"));

  displays.setMessage(F("SETUP   "));

  if (digitalRead(PIN_BUTTON_INTENSITY) == LOW) {
    settings.loadFromEEPROM();
  }
  
  if (digitalRead(PIN_BUTTON_MENU) == HIGH) {
    demo_mode = true;
  }

  if (!demo_mode) {
    
    #ifdef ESP8266 
      wifi.setup();
    #endif
    
    if (USE_ETHERNET) {
      net.setup();
    }
  } else {
    // TODO set demo launch
  }
  
}

void loop () {
  if (!demo_mode && USE_ETHERNET) {
    httpServer.loop();
    httpClient.loop();
  }

  #ifdef ESP8266 
    wifi.loop();
  #endif

  displays.loop();

  process_buttons();
}

void process_buttons() {
  boolean both_buttons_pressed = (digitalRead(PIN_BUTTON_INTENSITY) == LOW) && (digitalRead(PIN_BUTTON_MENU) == LOW);
  displays.showIP(both_buttons_pressed);
  
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



