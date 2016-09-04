#include "settings.h"
#include <EEPROM.h>

const byte ROM_HEADER_ID = 0x23;
const byte ROM_HEADER_VERSION = 0x01;

const byte ROM_WEBSITE_START = 0x02;
const byte ROM_URL_SETTINGS_START = ROM_WEBSITE_START + sizeof(settings.website);
const byte ROM_URL_PART_START = ROM_URL_SETTINGS_START + sizeof(settings.url_setting_part);
const byte ROM_INTENSITY_START = ROM_URL_PART_START + sizeof(settings.url_user_part);
const byte ROM_SELECTED_MENU_START = ROM_INTENSITY_START + sizeof(settings.intensity);

Settings settings;

void Settings::loadFromEEPROM() {
  if (EEPROM.read(0) == ROM_HEADER_ID && EEPROM.read(1) == ROM_HEADER_VERSION) {
    for(int i = 0; i < sizeof(website); i++) {
      website[i] = EEPROM.read(ROM_WEBSITE_START + i);
    }
    for(int i = 0; i < sizeof(url_setting_part); i++) {
      url_setting_part[i] = EEPROM.read(ROM_URL_SETTINGS_START + i);
    }
    for(int i = 0; i < sizeof(url_user_part); i++) {
      url_user_part[i] = EEPROM.read(ROM_URL_PART_START + i);
    }
    intensity = EEPROM.read(ROM_INTENSITY_START);
    selected_menu = EEPROM.read(ROM_SELECTED_MENU_START);
    
  }
}

void Settings::saveToEEPROM() {
  #ifndef ESP8266
    for(int i = 0; i < sizeof(website); i++) {
      EEPROM.update(ROM_WEBSITE_START + i, website[i]);
    }
    for(int i = 0; i < sizeof(url_setting_part); i++) {
      EEPROM.update(ROM_URL_SETTINGS_START + i, url_setting_part[i]);
    }
    for(int i = 0; i < sizeof(url_user_part); i++) {
      EEPROM.update(ROM_URL_PART_START + i, url_user_part[i]);
    }
    EEPROM.update(ROM_INTENSITY_START, intensity);
    EEPROM.update(ROM_SELECTED_MENU_START, selected_menu);
    
    EEPROM.update(0, ROM_HEADER_ID);
    EEPROM.update(1, ROM_HEADER_VERSION);
  #endif
  
}


