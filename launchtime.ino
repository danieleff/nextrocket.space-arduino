#include "ethernet.h"
#include "wifi.h"
#include "display.h"

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

const byte PIN_BUTTON_INTENSITY = 8;
const byte PIN_BUTTON_MENU = 9;
const byte PIN_BUTTON_DEMO = 4;

const int SELECTED_CYCLE = -2;
const int SELECTED_NEXT = -1;
const uint32_t SELECTED_CYCLE_DELAY_MILLIS = 10000;
const uint32_t MENU_BUTTON_SHOW_MENU_MILLIS = 1500;
const uint32_t MENU_BUTTON_SHOW_NAME_MILLIS = 3000;
const byte LAUNCH_COUNT = 5;

struct launch_type {
  int32_t seconds_left;
  char name[9];
};

launch_type launches[LAUNCH_COUNT] = {
  {10000, "Falcon 9"},
  {20000, "Ariane  "},
  {30000, "Mars    "},
  {40000, "BFR     "},
  {50000, "Testing "},
  }; // Launches that can be displayed

int selected_menu = SELECTED_CYCLE;

int selected_launch = 0; //selected launch index

uint8_t intensity = 2;

#ifdef ESP8266
  LaunchtimeWifi net = LaunchtimeWifi();
#else
  LaunchtimeEthernet net = LaunchtimeEthernet();
#endif

Display display(PIN_DISPLAY_DIN, PIN_DISPLAY_LOAD, PIN_DISPLAY_CLK);


uint32_t selected_launch_changed_millis = 0;
uint32_t info_downloaded_millis = 0; // when was the last launch time downloaded
uint32_t button_intensity_millis = 0; // when was the last button pressed
uint32_t button_menu_millis = 0; // when was the last button pressed

boolean demo_mode = false;

void setup () {
  Serial.begin(115200);
  Serial.println("\nSetup");

  pinMode(PIN_BUTTON_INTENSITY, INPUT_PULLUP);
  pinMode(PIN_BUTTON_MENU, INPUT_PULLUP);
  pinMode(PIN_BUTTON_DEMO, INPUT_PULLUP);
  
  display.setup();

  if (digitalRead(PIN_BUTTON_DEMO) == LOW) {
    demo_mode =true;
  }
    
  if (!demo_mode) {
    net.setup((char*) &launches, sizeof(launches));
  }
}

void loop () {
  
  if (!demo_mode) {
    net.loop();
  }

  update_display();

  process_buttons();
}

void process_buttons() {
  
  if (millis() - button_intensity_millis> 200) {
    if (digitalRead(PIN_BUTTON_INTENSITY) == LOW) {
      button_intensity_millis = millis();
      
      display.setup();
      intensity += 4;
      display.setIntensity(intensity);
    }
  }


  if (millis() - button_menu_millis> 200) {
    if (digitalRead(PIN_BUTTON_MENU) == LOW) {
      button_menu_millis = millis();
      selected_launch_changed_millis = millis();
      
      selected_menu++;
      if (selected_menu >= LAUNCH_COUNT) {
        selected_menu = SELECTED_CYCLE;
      }
      for(int i=0;i<LAUNCH_COUNT;i++) {
        Serial.print(launches[i].seconds_left);
        Serial.print(" ");
        Serial.println(launches[i].name);
      }
    }
  }
}

void update_display() {
  int selected_current = -1;
  
  if (selected_menu == SELECTED_NEXT) {
    int32_t max = INT32_MAX;
    for(int i = 0; i < LAUNCH_COUNT; i++) {
      if (max > launches[i].seconds_left && launches[i].seconds_left > 0) {
        selected_current = i;
        max = launches[i].seconds_left;
      }
    }
    
  } else if (selected_menu == SELECTED_CYCLE) {
    selected_current = (millis() / SELECTED_CYCLE_DELAY_MILLIS) % LAUNCH_COUNT;
  } else {
    selected_current = selected_menu;
  }

  if (selected_launch != selected_current) {
    selected_launch = selected_current;
    selected_launch_changed_millis = millis();
  }
  
  int32_t seconds_left = launches[selected_launch].seconds_left - (millis() - info_downloaded_millis) / 1000;

  if (selected_menu == SELECTED_CYCLE && (millis() - button_menu_millis) < MENU_BUTTON_SHOW_MENU_MILLIS) {
    display.write("ALL     ");
  } else if (selected_menu == SELECTED_NEXT && (millis() - button_menu_millis) < MENU_BUTTON_SHOW_MENU_MILLIS) {
    display.write("NEXT    ");
  } else if (seconds_left % 60 == 59 || (millis() - selected_launch_changed_millis) < MENU_BUTTON_SHOW_NAME_MILLIS) {
    display.write(launches[selected_launch].name);
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

