#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

const int SELECTED_IP = -3;
const int SELECTED_CYCLE = -2;
const int SELECTED_NEXT = -1;

const byte MAX_LAUNCH_COUNT = 8;

struct launch_type {
  int32_t seconds_left;
  char name[9];
};

class Settings {
  
  public:
    void loadFromEEPROM();
    void saveToEEPROM();

    //Things to save
    char website[24] = "nextrocket.space";
    char url_setting_part[24] = "/";
    char url_user_part[32] = "1a";
    uint8_t intensity = 6;
    int8_t selected_menu = SELECTED_CYCLE;

    //Things not to save
    uint8_t launch_count = 1;
    
    //Thins not to save
    launch_type launches[MAX_LAUNCH_COUNT] = {
      //{1000, "DEMO    "}, // Can be used for demo
      {0, "LOADING "},
      {0, "LOADING "},
      {0, "LOADING "},
      {0, "LOADING "},
      {0, "LOADING "},
      {0, "LOADING "},
      {0, "LOADING "},
      {0, "LOADING "},
    }; // Launches that can be displayed
    
    uint8_t selected_launch = 0; //selected launch index
    
    char* url_const_part = "api.php?v=1&q=";

};

extern Settings settings;

#endif

