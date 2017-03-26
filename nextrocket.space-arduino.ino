#include "ethernet.h"
#include "wifi.h"
#include "displays.h"
#include "http.h"
#include "settings.h"

#include "util.h"
#include "config.h"

uint32_t button_intensity_millis = 0; // when was the last button pressed
uint32_t button_menu_millis = 0; // when was the last button pressed

boolean menu_pressed = false;
boolean intensity_pressed = false;

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
  displays.setIntensity(settings.intensity);
  
  Serial.println(F("setup()"));

  displays.setMessage(F("SETUP   "));

  if (digitalRead(PIN_BUTTON_INTENSITY) == HIGH) {
    settings.loadFromEEPROM();
  }

  if (digitalRead(PIN_BUTTON_MENU) == LOW) {
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
  if (millis() - button_intensity_millis> 50) {
    if (digitalRead(PIN_BUTTON_INTENSITY) == LOW && !intensity_pressed) {
      intensity_pressed = true;
      button_intensity_millis = millis();

      settings.intensity += 4;
      settings.intensity &= 0x0F;
      
      displays.setIntensity(settings.intensity);
    }

    if (digitalRead(PIN_BUTTON_INTENSITY) == HIGH && intensity_pressed) {
      intensity_pressed = false;
      button_intensity_millis = millis();
    }
    
  }


  if (millis() - button_menu_millis> 50) {
    if (digitalRead(PIN_BUTTON_MENU) == LOW && !menu_pressed) {
      menu_pressed = true;
      
      button_menu_millis = millis();
      selected_launch_changed_millis = millis();
      
      displays.refresh();

      if (settings.selected_launch_id < SELECTED_NEXT) {
        
        settings.selected_launch_id++;
        
      } else if (settings.selected_launch_id == SELECTED_NEXT) {
        
        settings.selected_launch_id = atoi(settings.launches[0].launch_id);
        
      } else {
        
        int index = settings.getIndex(settings.selected_launch_id, settings.launch_count);
        index++;
        if (index >= settings.launch_count) {
          settings.selected_launch_id = SELECTED_IP;
        } else {
          settings.selected_launch_id = atoi(settings.launches[index].launch_id);
        }
        
      }
      
      settings.saveToEEPROM();
    }
    
    if (digitalRead(PIN_BUTTON_MENU) == HIGH && menu_pressed) {
      menu_pressed = false;
      button_menu_millis = millis();
    }
  }
}



