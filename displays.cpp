#include "displays.h"
#include "util.h"

#include "ethernet.h"
#include "wifi.h"
#include "http.h"
#include "settings.h"

const uint32_t SELECTED_CYCLE_DELAY_MILLIS = 10000;
const uint32_t MENU_BUTTON_SHOW_MENU_MILLIS = 1500;
const uint32_t MENU_BUTTON_SHOW_NAME_MILLIS = 3000;

const uint32_t DISPLAYS_REFRESH = 30;

//TODO make nicer
uint32_t selected_launch_changed_millis = 0;
extern uint32_t button_intensity_millis;
extern uint32_t button_menu_millis;

void Displays::setup() {

  #if USE_7_SEGMENT_DISPLAY
    rocket7SegmentDisplay.setup();
  #endif

  
}

void Displays::loop() {
  RETURN_BEFORE_MILLIS(DISPLAYS_REFRESH);

  #if USE_7_SEGMENT_DISPLAY
    rocket7SegmentDisplay.loop();
  #endif
  
  int selected_current = -1;

  if (settings.selected_menu == SELECTED_IP) {
    
  } else if (settings.selected_menu == SELECTED_NEXT) {
    
    selected_current = 0;
    
  } else if (settings.selected_menu == SELECTED_CYCLE) {
    
    selected_current = (millis() / SELECTED_CYCLE_DELAY_MILLIS) % settings.launch_count;
    
  } else {
    
    selected_current = settings.selected_menu;
    
  }

  if (settings.selected_launch != selected_current) {
    settings.selected_launch = selected_current;
    
    selected_launch_changed_millis = millis();

    settings.loadLaunch(settings.selected_launch);
    
  }
  
  int32_t launch_time = atol(settings.launch.launch_time);

  int32_t seconds_left = launch_time - settings.time_downloaded - (millis() - httpClient.info_downloaded_millis) / 1000;

  if (settings.selected_menu == SELECTED_CYCLE && (millis() - button_menu_millis) < MENU_BUTTON_SHOW_MENU_MILLIS) {
    setMessage(F("CYCLE               "));
    //show_seconds_left_digit_display(seconds_left, false);
    
  } else if (settings.selected_menu == SELECTED_NEXT && (millis() - button_menu_millis) < MENU_BUTTON_SHOW_MENU_MILLIS) {
    setMessage(F("UPCOMING            "));
    //show_seconds_left_digit_display(seconds_left, false);
    
  } else if (
        settings.launch.launch_time == 0
        || seconds_left % 60 == 59 
        || (millis() - selected_launch_changed_millis) < MENU_BUTTON_SHOW_NAME_MILLIS) {
    //write(settings.launch.rocket);
    
    //show_seconds_left_digit_display(seconds_left, false);
    
    rocket7SegmentDisplay.setLaunch(launch_time, settings.launch.rocket);
  } else {
    
    //show_seconds_left_digit_display(seconds_left, true);
    
    rocket7SegmentDisplay.setLaunch(launch_time, settings.launch.rocket);
  }

  //show_ip_digit_display();

}

void show_seconds_left_digit_display(int32_t time, boolean onDigitDisplay, char* buf) {
  boolean negative = false;
  if (time < 0) {
    negative = true;
    time = -time;
  }
  
  uint32_t seconds = time % 60;
  uint32_t minutes = (time / 60) % 60;
  uint32_t hours = (time / 60 / 60) % 24;
  uint32_t days = (time / 60 / 60 / 24);



  //char buf[16 + 1] = {0};
  
  if (settings.launch.time_status == TIME_STATUS_TIME) {
      sprintf(buf, "%2d days %02d:%02d:%02d", days, hours, minutes, seconds);
  } else if (settings.launch.time_status == TIME_STATUS_DAY) {
      sprintf(buf, "%2d days", days);
  } else if (settings.launch.time_status == TIME_STATUS_MONTH) {
      sprintf(buf, "%2d days", days); //TODO months
  }

}

void Displays::setMessage(const __FlashStringHelper *string) {
  rocket7SegmentDisplay.setMessage(string);
  
  //write((char*)string);
}

void Displays::refresh() {
  
}

