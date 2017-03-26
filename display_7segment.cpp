#include "displays.h"
#include "settings.h"
#include "ethernet.h"
#include "http.h"

const static byte charTo7Segment [] PROGMEM  = { 
    B00000000,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,//   0 
    B01111111,B01111011,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,//   8 
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,//  16 
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,//  24 
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,//  32  !"#$%&' 
    B00000000,B00000000,B00000000,B00000000,B10000000,B00000001,B10000000,B00000000,//  40 ()*+,-./ 
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,//  48 01234567 
    B01111111,B01111011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,//  56 89:;<=>? 
    B00000000,B01110111,B01111111,B01001110,B00111101,B01001111,B01000111,B01011110,//  64 @ABCDEFG 
    B00110111,B00000110,B00111100,B01010111,B00001110,B01010100,B01110110,B01111110,//  72 HIJKLMNO 
    B01100111,B01101011,B01100110,B01011011,B00001111,B00111110,B00111110,B00101010,//  80 PQRSTUVW 
    B00110111,B00111011,B01101101,B00000000,B00000000,B00000000,B00000000,B00001000,//  88 XYZ[\]^_ 
    B00000000,B01111101,B00011111,B00001101,B00111101,B01101111,B01000111,B01111011,//  96 'abcdefg 
    B00010111,B00000100,B00011000,B01010111,B00000110,B00010100,B00010101,B00011101,// 104 hijklmno 
    B01100111,B01110011,B00000101,B01011011,B00001111,B00011100,B00011100,B00010100,// 112 pqrstuvw 
    B00110111,B00111011,B01101101,B00000000,B00000000,B00000000,B00000000,B00000000 // 120 xyz{|}~ 
}; 

extern uint32_t selected_launch_changed_millis;

void Rocket7SegmentDisplay::setup() {
  ledControl.shutdown(0, false);
  ledControl.setIntensity(0, 7);
  ledControl.clearDisplay(0);
}

void Rocket7SegmentDisplay::loop() {
  if (settings.selected_launch_id == SELECTED_IP) {
    char buf[16];
    sprintf(buf, "IP.%d.%d.%d.%d", ether.myip[0], ether.myip[1], ether.myip[2], ether.myip[3]);
    int max_offset = strlen(buf) - 7;
    int offset = (millis() / 1000) % max_offset;
    write(buf + offset);
    
  } else if (!show_launch) {
    
    write(data);
    
  } else {
    launch_type *launch = settings.getLaunch();
    if (launch == NULL) {
        write("LOADING ");
        return;
    }
    
    int32_t launch_time = atol(launch->launch_time);

    int32_t time = launch_time - settings.time_downloaded - (millis() - httpClient.info_downloaded_millis) / 1000;

    if ((millis() - selected_launch_changed_millis) < 2000) {
      write(launch->rocket);
      return;
    }

    boolean negative = false;
    if (time < 0) {
      negative = true;
      time = -time;
    }
    
    uint32_t seconds = time % 60;
    uint32_t minutes = (time / 60) % 60;
    uint32_t hours = (time / 60 / 60) % 24;
    uint32_t days = (time / 60 / 60 / 24);

    if (seconds == 59) {
      write(launch->rocket);
      return;
    }
    
    if (launch->time_status == 'T') {
      ledControl.setDigit(0, 0, seconds % 10, false);
      ledControl.setDigit(0, 1, seconds / 10, false);
    
      ledControl.setDigit(0, 2, minutes % 10, true);
      ledControl.setDigit(0, 3, minutes / 10, false);
      
      ledControl.setDigit(0, 4, hours % 10, true);
      ledControl.setDigit(0, 5, hours / 10, false);

      ledControl.setDigit(0, 6, days % 10, true);
      
      if (negative) {
        ledControl.setDigit(0, 7, '-', false);
      } else {
        ledControl.setDigit(0, 7, days / 10, false);
      }
    } else {
      //ledControl.setDigit();
      //memcpy(data, F("    days"), 8);
      ledControl.setChar(0, 0, ' ', false);
      ledControl.setChar(0, 1, ' ', false);
      ledControl.setChar(0, 2, ' ', false);
      ledControl.setChar(0, 3, ' ', false);
      ledControl.setChar(0, 4, ' ', false);
      
      ledControl.setDigit(0, 5, days % 10, true);
    
      ledControl.setDigit(0, 6, (days / 10) % 10, false);
      ledControl.setDigit(0, 7, days / 100, false);
      
    }
  }
}

void Rocket7SegmentDisplay::setMessage(char* message8Chars) {
  show_launch = false;
  
  memcpy(data, message8Chars, 8);
  loop();
}

void Rocket7SegmentDisplay::setMessage(const __FlashStringHelper *message8Chars) {
  show_launch = false;
  
  PGM_P p = reinterpret_cast<PGM_P>(message8Chars);
  for(int i=0; i<8; i++) {
    data[i] = pgm_read_byte_near(p++);
  }
  loop();
}

void Rocket7SegmentDisplay::setLaunch() {
  show_launch = true;
}

void Rocket7SegmentDisplay::write(char* message8Chars) {
  for(int i = 0; i < 8; i++) { 
    uint8_t b = 0; 
     
    char chr = message8Chars[i]; 
    if (chr >> 7) { 
      chr &= 0x7F; 
      b = 0x80; 
    } 
    b |= pgm_read_byte_near(charTo7Segment + chr); 
    ledControl.setRow(0, 7-i, b); 
  }
  
}

void Rocket7SegmentDisplay::setIntensity(int intensity) {
  ledControl.setIntensity(0, intensity);
}

