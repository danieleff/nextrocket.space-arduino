#ifndef DIGIT_DISPLAY_H
#define DIGIT_DISPLAY_H

#include <Arduino.h>

class DigitDisplay {
  
  public:
    void setup(uint8_t din, uint8_t cs, uint8_t clk);
    void reset();
    void loop();

    void setIntensity(uint8_t intensity);
    
    void showDigit(int position, uint8_t digit, boolean dot);
    void showChar(int position, char digit, boolean dot);
    void write(char* string);
    void write(const __FlashStringHelper *string);
    
  private:
    void send(volatile byte opcode, volatile byte data);
    byte din;
    byte cs;
    byte clk;
};

#endif

