#ifndef DISPLAYS_H
#define DISPLAYS_H

#include <Arduino.h>
#include <LedControl.h>
#include "config.h"

extern uint32_t selected_launch_changed_millis;

class Rocket7SegmentDisplay {
  public:
    Rocket7SegmentDisplay(): ledControl(PIN_8_SEGMENT_DIN, PIN_8_SEGMENT_CLK, PIN_8_SEGMENT_LOAD), show_launch(false) {}
    void setup();
    void loop();
    void setMessage(char* message8Chars);
    void setMessage(const __FlashStringHelper *message8Chars);
    void setLaunch(int32_t launch_at_seconds, char* name8Chars);
  private:
    LedControl ledControl;
    void write(char* message8Chars);

    char data[8];
    int32_t launch_at_seconds;
    bool show_launch;
};

class RocketCharDisplay {
    void setup();
    void loop();
    void setMessage(char* message);
    void setLaunch(int launch_at_millis, char* name);
};

class RocketGraphicsDisplay {
  public:
    void setup();
    void loop();
    void setMessage(char* message);
    void setLaunch(int launch_at_millis, char* name);
    
};


class Displays {
  public:
    void setup();
    void loop();
    void setMessage(const __FlashStringHelper *string);
    void refresh();
    
  private:
    Rocket7SegmentDisplay rocket7SegmentDisplay;
};

extern Displays displays;

#endif

