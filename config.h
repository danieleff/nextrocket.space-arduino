#ifndef CONFIG_H
#define CONFIG_H

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

