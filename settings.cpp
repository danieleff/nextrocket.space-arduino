#include "settings.h"
#include <EEPROM.h>

const int ROM_HEADER_ID = 0x23;
const int ROM_HEADER_VERSION = 0x01;

const int ROM_URL_PART_START = 0x02;
const int ROM_INTENSITY_START = ROM_URL_PART_START + sizeof(settings.url_user_part);
const int ROM_SELECTED_MENU_START = ROM_INTENSITY_START + sizeof(settings.intensity);

const int ROM_LAUNCHES = ROM_SELECTED_MENU_START + sizeof(settings.selected_menu);

Settings settings;

void Settings::loadFromEEPROM() {
  if (EEPROM.read(0) == ROM_HEADER_ID && EEPROM.read(1) == ROM_HEADER_VERSION) {
    for(int i = 0; i < sizeof(url_user_part); i++) {
      url_user_part[i] = EEPROM.read(ROM_URL_PART_START + i);
    }
    intensity = EEPROM.read(ROM_INTENSITY_START);
    selected_menu = EEPROM.read(ROM_SELECTED_MENU_START);
    
  }
}

void Settings::saveToEEPROM() {
  
  return;
  
  #ifndef ESP8266
    for(int i = 0; i < sizeof(url_user_part); i++) {
      EEPROM.update(ROM_URL_PART_START + i, url_user_part[i]);
    }
    EEPROM.update(ROM_INTENSITY_START, intensity);
    EEPROM.update(ROM_SELECTED_MENU_START, selected_menu);
    
    EEPROM.update(0, ROM_HEADER_ID);
    EEPROM.update(1, ROM_HEADER_VERSION);
  #endif
  
}

void Settings::processApiResponse(int index, uint8_t data) {
  static char buf[10 + 1 + 3 + 1 + 1];
  if (index < sizeof(buf) - 1) {
    buf[index] = data;
    if (index == 10) {
      time_downloaded = atol(buf);
    } else if (index == 10 + 4) {
      launch_count = atol(buf + 11);
    }
    
  } else {
    setLaunchByte(index - sizeof(buf) + 1, data);
  }
}

void Settings::setLaunchByte(int index, uint8_t data) {
  
    if (data == '\n') {
      data = 0;
    }
    /*
    EEPROM.update(ROM_LAUNCHES + index, data);
    */
    ((uint8_t*)(&launches))[index] = data;
}

void Settings::loadLaunch(int index) {
  /*
  for(int i = 0; i < sizeof(launch); i++) {
    ((uint8_t*)(&launch))[i] = EEPROM.read(ROM_LAUNCHES + i + index * sizeof(launch));
  }
  */
  launch = launches[index];
}


