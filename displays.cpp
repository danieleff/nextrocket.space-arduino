#include "displays.h"
#include "digit_display.h"

#include "ethernet.h"
#include "wifi.h"
#include "http.h"
#include "settings.h"


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C charDisplay1(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
LiquidCrystal_I2C charDisplay2(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


const uint32_t SELECTED_CYCLE_DELAY_MILLIS = 10000;
const uint32_t MENU_BUTTON_SHOW_MENU_MILLIS = 1500;
const uint32_t MENU_BUTTON_SHOW_NAME_MILLIS = 3000;

const uint32_t DISPLAYS_REFRESH = 100;

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

//TODO make nicer
uint32_t selected_launch_changed_millis = 0;
extern uint32_t button_intensity_millis;
extern uint32_t button_menu_millis;

DigitDisplay digitDisplay;


void Displays::setup() {
  lastUpdate = -DISPLAYS_REFRESH;
  
  digitDisplay.setup(PIN_DISPLAY_DIN, PIN_DISPLAY_LOAD, PIN_DISPLAY_CLK);
  
  charDisplay1.begin(16,2);
  charDisplay1.backlight();
  charDisplay1.clear();

  charDisplay2.begin(20,4);
  charDisplay2.backlight();
  charDisplay2.clear();
}

char* intToString(int i, char* chr) {
  if (i > 99) chr++;
  if (i > 9) chr++;

  char* ret = chr;
  do {
    *chr-- = (i % 10) + '0';
    i /= 10;
  } while(i);
  return ret+1;
}

void show_ip_digit_display() {
  static char ipString[3 + 4*4 + 1];
  char *chr = ipString;
  //*chr++ = 'I';
  //*chr++ = 'P' | 0x80;
  //*chr++ = 0x80;
  
  chr = intToString(ether.myip[0], chr);
  *(chr-1) |= 0x80;
  chr = intToString(ether.myip[1], chr);
  *(chr-1) |= 0x80;
  chr = intToString(ether.myip[2], chr);
  *(chr-1) |= 0x80;
  chr = intToString(ether.myip[3], chr);
  *(chr-1) |= 0x80;
  *chr = 0;
  
  //Serial.println(strlen(ipString));
  
  digitDisplay.write(ipString + ((millis() / 1000) % (strlen(ipString)- 8 + 1)));
}

void show_seconds_left_digit_display(int32_t time, boolean onDigitDisplay) {
  boolean negative = false;
  if (time < 0) {
    negative = true;
    time = -time;
  }
  
  long seconds = time % 60;
  long minutes = (time / 60) % 60;
  long hours = (time / 60 / 60) % 24;
  long days = (time / 60 / 60 / 24);

  if (onDigitDisplay) {
    digitDisplay.showDigit(0, seconds % 10, false);
    digitDisplay.showDigit(1, seconds / 10, false);
  
    digitDisplay.showDigit(2, minutes % 10, true);
    digitDisplay.showDigit(3, minutes / 10, false);
    
    digitDisplay.showDigit(4, hours % 10, true);
    digitDisplay.showDigit(5, hours / 10, false);
  
    digitDisplay.showDigit(6, days % 10, true);
    
    if (negative) {
      digitDisplay.showChar(7, '-', false);
    } else {
      digitDisplay.showDigit(7, days / 10, false);
    }
  }

  charDisplay1.setCursor(0,1);
  charDisplay1.print(days / 10);
  charDisplay1.print(days % 10);
  charDisplay1.print(" days ");
  charDisplay1.print(hours / 10);
  charDisplay1.print(hours % 10);
  charDisplay1.print(":");
  charDisplay1.print(minutes / 10);
  charDisplay1.print(minutes % 10);
  charDisplay1.print(":");
  charDisplay1.print(seconds / 10);
  charDisplay1.print(seconds % 10);


  charDisplay2.setCursor(0,1);
  charDisplay2.print(days / 10);
  charDisplay2.print(days % 10);
  charDisplay2.print(" days ");
  charDisplay2.print(hours / 10);
  charDisplay2.print(hours % 10);
  charDisplay2.print(":");
  charDisplay2.print(minutes / 10);
  charDisplay2.print(minutes % 10);
  charDisplay2.print(":");
  charDisplay2.print(seconds / 10);
  charDisplay2.print(seconds % 10);
}

void Displays::loop() {
  if (millis() - lastUpdate < DISPLAYS_REFRESH) {
    return;
  }

  lastUpdate = millis();

  digitDisplay.reset();
  
  //digitDisplay.reset();
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
    write("ALL     ");
    show_seconds_left_digit_display(seconds_left, false);
    
  } else if (settings.selected_menu == SELECTED_NEXT && (millis() - button_menu_millis) < MENU_BUTTON_SHOW_MENU_MILLIS) {
    write("NEXT    ");
    show_seconds_left_digit_display(seconds_left, false);
    
  } else if (
        settings.launches[settings.selected_launch].seconds_left == 0
        || seconds_left % 60 == 59 
        || (millis() - selected_launch_changed_millis) < MENU_BUTTON_SHOW_NAME_MILLIS) {
    write(settings.launches[settings.selected_launch].name);
    
    show_seconds_left_digit_display(seconds_left, false);
  } else {
    
    show_seconds_left_digit_display(seconds_left, true);
  }

  //show_ip_digit_display();


  charDisplay2.setCursor(0,2);
  charDisplay2.print("AAAA");

  charDisplay2.setCursor(0,3);
  charDisplay2.print("BBBB");
}

void Displays::write(char* string) {
  digitDisplay.write(string);

  charDisplay1.setCursor(0,0);
  charDisplay1.print(string);
  
  charDisplay2.setCursor(0,0);
  charDisplay2.print(string);
}

void Displays::write(const __FlashStringHelper *string) {
  digitDisplay.write(string);

  charDisplay1.setCursor(0,0);
  charDisplay1.print(string);

  charDisplay2.setCursor(0,0);
  charDisplay2.print(string);
}


