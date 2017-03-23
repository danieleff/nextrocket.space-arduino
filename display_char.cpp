#include "displays.h"

#ifdef X

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C charDisplay1(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
LiquidCrystal_I2C charDisplay2(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

  charDisplay1.begin(16,2);
  charDisplay1.backlight();
  charDisplay1.clear();
  
  charDisplay2.begin(20,4);
  charDisplay2.backlight();
  charDisplay2.clear();



  charDisplay1.setCursor(0, 1);
  charDisplay1.print(buf);
  
  charDisplay2.setCursor(0, 1);
  charDisplay2.print(buf);

  
  charDisplay2.setCursor(0,2);
  charDisplay2.print(settings.launch.destination);

  charDisplay2.setCursor(0,3);
  charDisplay2.print(settings.launch.payload);
#endif

