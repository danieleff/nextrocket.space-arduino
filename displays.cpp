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
  
  int show_launch_id = -1;

  if (settings.selected_launch_id == SELECTED_IP) {
    
  } else if (settings.selected_launch_id == SELECTED_NEXT) {
    
    show_launch_id = settings.launches[0].launch_id;
    
  } else if (settings.selected_launch_id == SELECTED_CYCLE) {
    
    show_launch_id = settings.launches[(millis() / SELECTED_CYCLE_DELAY_MILLIS) % settings.launch_count].launch_id;
    
  } else {
    
    show_launch_id = settings.selected_launch_id;
    
  }

  if (settings.show_launch_id != show_launch_id) {
    selected_launch_changed_millis = millis();

    settings.setLaunch(show_launch_id);
    
  }
  
  if (settings.selected_launch_id == SELECTED_CYCLE && (millis() - button_menu_millis) < MENU_BUTTON_SHOW_MENU_MILLIS) {
    setMessage(F("CYCLE               "));
    
  } else if (settings.selected_launch_id == SELECTED_NEXT && (millis() - button_menu_millis) < MENU_BUTTON_SHOW_MENU_MILLIS) {
    setMessage(F("UPCOMING            "));
    
  } else {

    rocket7SegmentDisplay.setLaunch();
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
  
  /*
  if (settings.launch.time_status == TIME_STATUS_TIME) {
      sprintf(buf, "%2d days %02d:%02d:%02d", days, hours, minutes, seconds);
  } else if (settings.launch.time_status == TIME_STATUS_DAY) {
      sprintf(buf, "%2d days", days);
  } else if (settings.launch.time_status == TIME_STATUS_MONTH) {
      sprintf(buf, "%2d days", days); //TODO months
  }
  */
  
}

void Displays::setMessage(const __FlashStringHelper *string) {
  rocket7SegmentDisplay.setMessage(string);
  
  //write((char*)string);
}

void Displays::refresh() {
  
}

void Displays::setIntensity(int intensity) {
  rocket7SegmentDisplay.setIntensity(intensity);
}

