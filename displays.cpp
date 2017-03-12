#include "displays.h"
#include "digit_display.h"
#include "util.h"

#include "ethernet.h"
#include "wifi.h"
#include "http.h"
#include "settings.h"

#include "config.h"

#include "CompressedImage.h"
#include "heatshrink/heatshrink_decoder.h"

//#define USE_CHARDISPLAY

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C charDisplay1(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
LiquidCrystal_I2C charDisplay2(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

#include "ILI9341SPI.h"
ILI9341SPI graphic(PIN_GRAPHICS_CS, PIN_GRAPHICS_DC, PIN_GRAPHICS_RESET);


const uint32_t SELECTED_CYCLE_DELAY_MILLIS = 10000;
const uint32_t MENU_BUTTON_SHOW_MENU_MILLIS = 1500;
const uint32_t MENU_BUTTON_SHOW_NAME_MILLIS = 3000;

const uint32_t DISPLAYS_REFRESH = 30;

//TODO make nicer
uint32_t selected_launch_changed_millis = 0;
extern uint32_t button_intensity_millis;
extern uint32_t button_menu_millis;

DigitDisplay digitDisplay;

extern compressed_image_t spacex_small;
heatshrink_decoder decoder;

void Displays::setup() {
  
  digitDisplay.setup(PIN_8_SEGMENT_DIN, PIN_8_SEGMENT_LOAD, PIN_8_SEGMENT_CLK);

#ifdef USE_CHARDISPLAY
  charDisplay1.begin(16,2);
  charDisplay1.backlight();
  charDisplay1.clear();
  
  charDisplay2.begin(20,4);
  charDisplay2.backlight();
  charDisplay2.clear();
#endif

#if USE_GRAPH_DISPLAY
  graphic.begin();
  graphic.setRotation(1);
  graphic.fillScreen(BLACK);
  graphic.setTextColor(WHITE);
  graphic.setTextSize(1);
#endif

}

char* intToString(int i, char* chr) {
  if (i > 99) chr++;
  if (i > 9) chr++;

  char* ret = chr;
  do {
    *chr-- = (i % 10) + '0';
    i /= 10;
  } while(i);
  return ret+1;
}

void show_ip_digit_display() {
  static char ipString[3 + 4*4 + 1];
  char *chr = ipString;
  //*chr++ = 'I';
  //*chr++ = 'P' | 0x80;
  //*chr++ = 0x80;
  
  chr = intToString(ether.myip[0], chr);
  *(chr-1) |= 0x80;
  chr = intToString(ether.myip[1], chr);
  *(chr-1) |= 0x80;
  chr = intToString(ether.myip[2], chr);
  *(chr-1) |= 0x80;
  chr = intToString(ether.myip[3], chr);
  *(chr-1) |= 0x80;
  *chr = 0;
  
  //Serial.println(strlen(ipString));
  
  digitDisplay.write(ipString + ((millis() / 1000) % (strlen(ipString)- 8 + 1)));
}

void show_seconds_left_digit_display(int32_t time, boolean onDigitDisplay) {
  boolean negative = false;
  if (time < 0) {
    negative = true;
    time = -time;
  }
  
  uint32_t seconds = time % 60;
  uint32_t minutes = (time / 60) % 60;
  uint32_t hours = (time / 60 / 60) % 24;
  uint32_t days = (time / 60 / 60 / 24);

  if (onDigitDisplay) {
    
    if (settings.launch.time_status == 'T') {
      digitDisplay.showDigit(0, seconds % 10, false);
      digitDisplay.showDigit(1, seconds / 10, false);
    
      digitDisplay.showDigit(2, minutes % 10, true);
      digitDisplay.showDigit(3, minutes / 10, false);
      
      digitDisplay.showDigit(4, hours % 10, true);
      digitDisplay.showDigit(5, hours / 10, false);

      digitDisplay.showDigit(6, days % 10, true);
      
      if (negative) {
        digitDisplay.showChar(7, '-', false);
      } else {
        digitDisplay.showDigit(7, days / 10, false);
      }
    } else {
      digitDisplay.write(F("    days"));

      digitDisplay.showDigit(5, days % 10, true);
    
      digitDisplay.showDigit(6, (days / 10) % 10, false);
      digitDisplay.showDigit(7, days / 100, false);
      
    }
    
  }

  char buf[16 + 1] = {0};
  
  if (settings.launch.time_status == TIME_STATUS_TIME) {
      sprintf(buf, "%2d days %02d:%02d:%02d", days, hours, minutes, seconds);
  } else if (settings.launch.time_status == TIME_STATUS_DAY) {
      sprintf(buf, "%2d days", days);
  } else if (settings.launch.time_status == TIME_STATUS_MONTH) {
      sprintf(buf, "%2d days", days); //TODO months
  }

  graphic.setCursor(10, 66);
  graphic.setTextSize(3);
  graphic.print(buf);
#ifdef USE_CHARDISPLAY
  charDisplay1.setCursor(0, 1);
  charDisplay1.print(buf);
  
  charDisplay2.setCursor(0, 1);
  charDisplay2.print(buf);
#endif


#if USE_GRAPH_DISPLAY
  size_t input_size;
  size_t output_size;
  heatshrink_decoder_reset(&decoder);
  uint8_t *data = spacex_small.compressed_data;
  size_t size = spacex_small.compressed_data_size;
  unsigned long start = micros();
  graphic.protocol.beginTransaction();
  graphic.protocol.setAddrWindow(20+0, 120, 20+spacex_small.width - 1, 120 + spacex_small.height - 1);
  graphic.protocol.startBuffer();
  decoder.input_buffer = data;
  decoder.input_size = size;

  size -= input_size;
  data += input_size;
  
  HSD_poll_res poll_res;
  
  do {
    uint8_t buf[64];
    poll_res = heatshrink_decoder_poll(&decoder, buf, sizeof(buf), &output_size);
    for(int i=0; i<output_size; i++) {
      graphic.protocol.addToBuffer(Color(spacex_small.palette_data[buf[i] * 3], spacex_small.palette_data[buf[i] * 3 + 1], spacex_small.palette_data[buf[i] * 3 + 2]));
    }
  } while(poll_res == HSDR_POLL_MORE);
    
  graphic.protocol.sendBuffer();
  graphic.protocol.endTransaction();
#endif
  
}

void fillLine(int x, int y, int w, int h, int glow) {
  graphic.protocol.setAddrWindow(x, y, x + w - 1, y + h - 1);
  graphic.protocol.startBuffer();
  for(int i = 0; i < w + h - 1; i++) {
    Color c(0, 0, 255);
    int d = i - glow + 255;
    
    if (d > 0 && d < 200) {
      c = Color(d, d, 255);
    }
    graphic.protocol.addToBuffer(c);
  }
  graphic.protocol.sendBuffer();
}

void drawFrame(int x, int y, int w, int h) {
  int glow = (millis() / 5) % 1000;

  graphic.protocol.beginTransaction();
  
  fillLine(x, y, w, 1, glow);
  fillLine(x, y, 1, h, glow);
  fillLine(x, y + h, w, 1, glow - h);
  fillLine(x + w, y, 1, h, glow - w);
  
  graphic.protocol.endTransaction();
  
  //graphic.drawLine(x, y, x + w, y + h, millis());
}

void Displays::loop() {
  RETURN_BEFORE_MILLIS(DISPLAYS_REFRESH);

  drawFrame(8, 50, 300, 50);

  lastUpdate = millis();

  digitDisplay.reset();
  
  //digitDisplay.reset();
  int selected_current = -1;
  
  if (settings.selected_menu == SELECTED_NEXT) {
    
    selected_current = 1; //TODO
    
    
    int32_t max = INT32_MAX;
    for(int i = 0; i < settings.launch_count; i++) {
      settings.loadLaunch(i);
      /*if (max > settings.launch.launch_time && settings.launch.launch_time > 0) {
        selected_current = i;
        max = settings.launch.launch_time;
      }*/
    }
    
    
  } else if (settings.selected_menu == SELECTED_CYCLE) {
    selected_current = (millis() / SELECTED_CYCLE_DELAY_MILLIS) % settings.launch_count;
  } else {
    selected_current = settings.selected_menu;
  }

  if (settings.selected_launch != selected_current) {
    settings.selected_launch = selected_current;
    selected_launch_changed_millis = millis();
  }
  
  settings.loadLaunch(settings.selected_launch);

  int32_t launch_time = atol(settings.launch.launch_time);

  
  int32_t seconds_left = launch_time - settings.time_downloaded - (millis() - httpClient.info_downloaded_millis) / 1000;

  if (settings.selected_menu == SELECTED_CYCLE && (millis() - button_menu_millis) < MENU_BUTTON_SHOW_MENU_MILLIS) {
    write(F("CYCLE               "));
    show_seconds_left_digit_display(seconds_left, false);
    
  } else if (settings.selected_menu == SELECTED_NEXT && (millis() - button_menu_millis) < MENU_BUTTON_SHOW_MENU_MILLIS) {
    write(F("UPCOMING            "));
    show_seconds_left_digit_display(seconds_left, false);
    
  } else if (
        settings.launch.launch_time == 0
        || seconds_left % 60 == 59 
        || (millis() - selected_launch_changed_millis) < MENU_BUTTON_SHOW_NAME_MILLIS) {
    write(settings.launch.rocket);
    
    show_seconds_left_digit_display(seconds_left, false);
  } else {
    
    show_seconds_left_digit_display(seconds_left, true);
  }

  //show_ip_digit_display();

#ifdef USE_CHARDISPLAY
  charDisplay2.setCursor(0,2);
  charDisplay2.print(settings.launch.destination);

  charDisplay2.setCursor(0,3);
  charDisplay2.print(settings.launch.payload);
#endif
}

void Displays::write(char* string) {
  digitDisplay.write(string);

#ifdef USE_CHARDISPLAY
  charDisplay1.setCursor(0,0);
  charDisplay1.print(string);
  
  charDisplay2.setCursor(0,0);
  charDisplay2.print(string);
#endif

  graphic.setTextSize(1);
  graphic.setCursor(0, 0);
  graphic.print(string);
}

void Displays::write(const __FlashStringHelper *string) {
  write((char*)string);
}

void Displays::refresh() {
  lastUpdate = 0;
}



