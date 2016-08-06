#ifndef DISPLAY_H
#define DISPLAY_H


class Display {
  
  public:
    Display(int din, int cs, int clk);
    void setup();
    void loop();

    void setIntensity(byte intensity);
    
    void showDigit(int position, byte digit, boolean dot);
    void showChar(int position, char digit, boolean dot);
    void write(char* string);
    
  private:
    void send(volatile byte opcode, volatile byte data);
    int din;
    int cs;
    int clk;
};

#endif

