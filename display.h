#ifndef DISPLAY_H
#define DISPLAY_H


class Display {
  
  public:
    void setup(byte din, byte cs, byte clk);
    void reset();
    void loop();

    void setIntensity(byte intensity);
    
    void showDigit(int position, byte digit, boolean dot);
    void showChar(int position, char digit, boolean dot);
    void write(char* string);
    
  private:
    void send(volatile byte opcode, volatile byte data);
    byte din;
    byte cs;
    byte clk;
};

#endif

