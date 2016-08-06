#include <Arduino.h>
#include "display.h"

#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

const static byte charTable [] PROGMEM  = {
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

Display::Display(int din, int cs, int clk) {
  this->din = din;
  this->cs = cs;
  this->clk = clk;

  pinMode(din, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
}

void Display::setup() {
  
  send(OP_DISPLAYTEST, 0); //displaytest
  send(OP_SCANLIMIT, 7); //scanlimit
  
  send(OP_INTENSITY, 8); //intensity
  send(OP_DECODEMODE, 0);
  
  send(OP_SHUTDOWN, 1); //shutdown false
  
}

void Display::setIntensity(byte intensity) {
  send(OP_INTENSITY, intensity & 0x0F);
}

void Display::showDigit(int position, byte digit, boolean dot) {
  showChar(position, digit + '0', dot);
}

void Display::showChar(int position, char digit, boolean dot) {
  send(position + 1, pgm_read_byte_near(charTable + digit) | (dot ? 0x80 : 0));
}

void Display::write(char* string) {
  
  for(int i=0;i<8;i++) {
    send(8-i, pgm_read_byte_near(charTable + string[i]));
  }
  
}

void Display::loop() {
  
}


void Display::send(volatile byte opcode, volatile byte data) {
  digitalWrite(cs,LOW);
  
  shiftOut(din, clk, MSBFIRST, opcode);
  shiftOut(din, clk, MSBFIRST, data);
      
  digitalWrite(cs,HIGH);
}

