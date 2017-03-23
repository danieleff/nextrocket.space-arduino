#include "displays.h"

#include "CompressedImage.h"
#include "heatshrink/heatshrink_decoder.h"

#include "ILI9341SPI.h"

#if USE_GRAPH_DISPLAY

ILI9341SPI graphic(PIN_GRAPHICS_CS, PIN_GRAPHICS_DC, PIN_GRAPHICS_RESET);

//#include "images/spacex_small.c"

extern compressed_image_t spacex_small;
heatshrink_decoder decoder;

void RocketGraphicsDisplay::setup() {

  graphic.begin();
  graphic.setRotation(1);
  graphic.fillScreen(BLACK);
  graphic.setTextColor(WHITE);
  graphic.setTextSize(1);
  
}

void RocketGraphicsDisplay::loop() {
  
 // drawFrame(8, 50, 300, 50);

  graphic.setCursor(10, 66);
  graphic.setTextSize(3);
  //graphic.print(buf);


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


#endif

