#ifndef CONFIG_H
#define CONFIG_H

#define USE_8_SEGMENT_DISPLAY true
#define USE_2_LINE_CHAR_DISPLAY false
#define USE_4_LINE_CHAR_DISPLAY false
#define USE_GRAPH_DISPLAY true

#define USE_ETHERNET true
#define RUNNING_ON_WIFI_ESP8266 false

#ifdef ESP8266
  
  #error "untested"

  const int LAUNCH_RAM = 1024 * 32;
  
  const byte PIN_BUTTON_INTENSITY = 5;
  const byte PIN_BUTTON_MENU = 2;
  const byte PIN_BUTTON_DEMO = 4;
  
#elif defined(STM32)

  const int LAUNCH_RAM = 1024 * 4;
  
  const byte PIN_BUTTON_INTENSITY = 17;
  const byte PIN_BUTTON_MENU = 18;
  const byte PIN_BUTTON_DEMO = 19;
  
  const byte PIN_8_SEGMENT_DIN = 15;
  const byte PIN_8_SEGMENT_LOAD = 16;
  const byte PIN_8_SEGMENT_CLK = 25;

  const byte PIN_ETHERNET_CS = 31;

  const byte PIN_GRAPHICS_CS = 0;
  const byte PIN_GRAPHICS_DC = 3;
  const byte PIN_GRAPHICS_RESET = 1;

  const byte PIN_TOUCH_CS = 8;
  
#elif defined(ARDUINO_AVR_NANO)

  #error "untested"
  
  const int LAUNCH_RAM = 1024;

  const byte PIN_BUTTON_INTENSITY = 17;
  const byte PIN_BUTTON_MENU = 18;
  const byte PIN_BUTTON_DEMO = 19;
  
#else

  #error "unknown board"
  
#endif



#endif

