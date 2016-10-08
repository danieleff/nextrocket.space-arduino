#ifndef DISPLAYS_H
#define DISPLAYS_H

#include <Arduino.h>

extern uint32_t selected_launch_changed_millis;

class Displays {
  public:
    void setup();
    void loop();
    void write(char* string);
    void write(const __FlashStringHelper *string);
    
};

extern Displays displays;

#endif

