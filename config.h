#ifndef CONFIG_H
#define CONFIG_H

#ifdef ESP8266
  
  #error "untested"

  const int LAUNCH_RAM = 1024 * 32;
  
  const byte PIN_BUTTON_INTENSITY = 5;
  const byte PIN_BUTTON_MENU = 2;
  const byte PIN_BUTTON_DEMO = 4;
  
#elif defined(STM32)

#define USE_7_SEGMENT_DISPLAY true
#define USE_2_LINE_CHAR_DISPLAY false
#define USE_4_LINE_CHAR_DISPLAY false
#define USE_GRAPH_DISPLAY true
#define USE_EEPROM false

#define USE_ETHERNET true
#define RUNNING_ON_WIFI_ESP8266 false

  const int ETHERNET_BUFFER_SIZE = 700;

  const int LAUNCH_RAM = 1024 * 4;
  
  const byte PIN_BUTTON_INTENSITY = 17;
  const byte PIN_BUTTON_MENU = 18;
  
  const byte PIN_8_SEGMENT_DIN = 15;
  const byte PIN_8_SEGMENT_LOAD = 16;
  const byte PIN_8_SEGMENT_CLK = 25;

  const byte PIN_ETHERNET_CS = 31;

  const byte PIN_GRAPHICS_CS = 0;
  const byte PIN_GRAPHICS_DC = 3;
  const byte PIN_GRAPHICS_RESET = 1;

  const byte PIN_TOUCH_CS = 8;
  
#elif defined(ARDUINO_AVR_NANO)

#define USE_7_SEGMENT_DISPLAY true
#define USE_2_LINE_CHAR_DISPLAY false
#define USE_4_LINE_CHAR_DISPLAY false
#define USE_GRAPH_DISPLAY false
#define USE_EEPROM true

#define USE_ETHERNET true
#define RUNNING_ON_WIFI_ESP8266 false

  const int ETHERNET_BUFFER_SIZE = 500;

  const int LAUNCH_RAM = 400;

  const byte PIN_BUTTON_INTENSITY = 2;
  const byte PIN_BUTTON_MENU = 3;
  
  const byte PIN_8_SEGMENT_DIN = 8;
  const byte PIN_8_SEGMENT_LOAD = 7;
  const byte PIN_8_SEGMENT_CLK = 6;

  const byte PIN_ETHERNET_CS = 10;

  const byte PIN_GRAPHICS_CS = 9;
  const byte PIN_GRAPHICS_DC = 5;
  const byte PIN_GRAPHICS_RESET = 14;

  const byte PIN_TOUCH_CS = 15;
  
#else

  #error "unknown board"
  
#endif



#endif

