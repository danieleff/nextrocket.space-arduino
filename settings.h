#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

const int SELECTED_IP = -3;
const int SELECTED_CYCLE = -2;
const int SELECTED_NEXT = -1;

const byte MAX_LAUNCH_COUNT = 8;

struct launch_type {
  int32_t seconds_left;
  char name[24];
};

class Settings {
  
  public:
    void load_from_EEPROM();
    void save_to_EEPROM();

    //Things to save
    int launch_count = 2;
    char website[24] = "danieleff.com";
    char url_setting_part[24] = "/launchtime/";
    char url_user_part[120] = "falcon+9,falcon+heavy";
    byte intensity = 2;
    int selected_launch = 0; //selected launch index
    
    //Thins not to save
    launch_type launches[MAX_LAUNCH_COUNT] = {
      {10000, "Falcon 9"},
      {20000, "Ariane  "},
      {30000, "Mars    "},
      {40000, "BFR     "},
      {50000, "Testing "},
    }; // Launches that can be displayed

    int selected_menu = SELECTED_CYCLE;

    char* url_const_part = "api.php?v=1&q=";

};

extern Settings settings;

#endif

