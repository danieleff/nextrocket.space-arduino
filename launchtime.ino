#include "ethernet.h"
#include "wifi.h"
#include "display.h"
#include "http.h"
#include "settings.h"

const byte PIN_BUTTON_INTENSITY = 5;
const byte PIN_BUTTON_MENU = 2;
const byte PIN_BUTTON_DEMO = 4;

const uint32_t SELECTED_CYCLE_DELAY_MILLIS = 10000;
const uint32_t MENU_BUTTON_SHOW_MENU_MILLIS = 1500;
const uint32_t MENU_BUTTON_SHOW_NAME_MILLIS = 3000;

#ifdef ESP8266 
  //Pins with esp8266 wifi chip
  const byte PIN_DISPLAY_DIN = 16;
  const byte PIN_DISPLAY_CLK = 14;
  const byte PIN_DISPLAY_LOAD = 12;
#else 
  //Pins with arduino nano + ethernet
  const byte PIN_DISPLAY_DIN = 7;
  const byte PIN_DISPLAY_CLK = 6;
  const byte PIN_DISPLAY_LOAD = 5;
#endif

uint32_t selected_launch_changed_millis = 0;
uint32_t button_intensity_millis = 0; // when was the last button pressed
uint32_t button_menu_millis = 0; // when was the last button pressed

boolean use_ethernet = false;

Display display;

void setup () {
  Serial.begin(115200);
  Serial.println(F("setup()"));

Serial.println(F("a"));
  
  display.setup(PIN_DISPLAY_DIN, PIN_DISPLAY_LOAD, PIN_DISPLAY_CLK);
Serial.println(F("b"));
  display.write(F("SETUP   "));
Serial.println(F("c"));
  settings.loadFromEEPROM();
Serial.println(F("d"));
  pinMode(PIN_BUTTON_INTENSITY, INPUT_PULLUP);
Serial.println(F("e"));
  pinMode(PIN_BUTTON_MENU, INPUT_PULLUP);
Serial.println(F("f"));
  pinMode(PIN_BUTTON_DEMO, INPUT_PULLUP);
Serial.println(F("g"));
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

  update_display();

  process_buttons();

  //Serial.println("Test");
  //delay(500);
  yield();
}

void process_buttons() {
  
  if (millis() - button_intensity_millis> 200) {
    if (digitalRead(PIN_BUTTON_INTENSITY) == LOW) {
      button_intensity_millis = millis();
      
      display.reset();
      
      settings.intensity += 4;
      display.setIntensity(settings.intensity);
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

void update_display() {
  int selected_current = -1;
  
  if (settings.selected_menu == SELECTED_NEXT) {
    int32_t max = INT32_MAX;
    for(int i = 0; i < settings.launch_count; i++) {
      if (max > settings.launches[i].seconds_left && settings.launches[i].seconds_left > 0) {
        selected_current = i;
        max = settings.launches[i].seconds_left;
      }
    }
    
  } else if (settings.selected_menu == SELECTED_CYCLE) {
    selected_current = (millis() / SELECTED_CYCLE_DELAY_MILLIS) % settings.launch_count;
  } else {
    selected_current = settings.selected_menu;
  }

  if (settings.selected_launch != selected_current) {
    settings.selected_launch = selected_current;
    selected_launch_changed_millis = millis();
  }
  
  int32_t seconds_left = settings.launches[settings.selected_launch].seconds_left - (millis() - httpClient.info_downloaded_millis) / 1000;

  if (settings.selected_menu == SELECTED_CYCLE && (millis() - button_menu_millis) < MENU_BUTTON_SHOW_MENU_MILLIS) {
    display.write("ALL     ");
  } else if (settings.selected_menu == SELECTED_NEXT && (millis() - button_menu_millis) < MENU_BUTTON_SHOW_MENU_MILLIS) {
    display.write("NEXT    ");
  } else if (
        settings.launches[settings.selected_launch].seconds_left == 0
        || seconds_left % 60 == 59 
        || (millis() - selected_launch_changed_millis) < MENU_BUTTON_SHOW_NAME_MILLIS) {
    display.write(settings.launches[settings.selected_launch].name);
  } else {
    show(seconds_left);
  }
}

void show(int32_t time) {
  boolean negative = false;
  if (time < 0) {
    negative = true;
    time = -time;
  }
  
  long seconds = time % 60;
  long minutes = (time / 60) % 60;
  long hours = (time / 60 / 60) % 24;
  long days = (time / 60 / 60 / 24);

  display.showDigit(0, seconds % 10, false);
  display.showDigit(1, seconds / 10, false);

  display.showDigit(2, minutes % 10, true);
  display.showDigit(3, minutes / 10, false);
  
  display.showDigit(4, hours % 10, true);
  display.showDigit(5, hours / 10, false);

  display.showDigit(6, days % 10, true);
  
  if (negative) {
    display.showChar(7, '-', false);
  } else {
    display.showDigit(7, days / 10, false);
  }
  
}

