#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

#include "config.h"

const int SELECTED_IP = -3;
const int SELECTED_CYCLE = -2;
const int SELECTED_NEXT = -1;


const char TIME_STATUS_MONTH = 'M';
const char TIME_STATUS_DAY = 'D';
const char TIME_STATUS_TIME = 'T';

struct launch_type {
  char launch_id[6 + 1];
  char launch_status;
  char time_status;
  char launch_time[10 + 1];
  char agency[10 + 1];
  char rocket[20 + 1];
  //char payload[20 + 1];
  //char destination[10 + 1];
};

const uint8_t MAX_LAUNCH_COUNT = 5;

class Settings {
  
  public:
    void loadFromEEPROM();
    void saveToEEPROM();

    //Things to save
    char url_user_part[60] = "1|";
    uint8_t intensity = 6;
    int32_t selected_launch_id = SELECTED_IP;

    //Things not to save
    uint8_t launch_count = 0;
    
    int getIndex(int launch_id, int return_if_not_found);
    void setLaunch(int launch_id);
    launch_type *getLaunch();
    void processApiResponse(int index, uint8_t data);
    
    int32_t show_launch_id = -1;
    
    uint32_t time_downloaded;

    launch_type launches[MAX_LAUNCH_COUNT];
    
  private:
    void setLaunchByte(int index, uint8_t data);
    
    /*
    //Thins not to save
    launch_type launches[MAX_LAUNCH_COUNT] = {
      //{1000, "DEMO    "}, // Can be used for demo
      {0, "LOADING "},
      {0, "LOADING "},
      {0, "LOADING "},
    }; // Launches that can be displayed
    */
    
};

extern Settings settings;

#endif

