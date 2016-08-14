#ifndef HTTP_H
#define HTTP_H

#include <Arduino.h>

class HttpClient {
  public:
    HttpClient(): next_try_millis(0), info_downloaded_millis(0) {};
    void loop();
    uint32_t info_downloaded_millis;
    uint32_t next_try_millis;
    
};

class HttpServer {
  public:
    void loop();
};

extern HttpClient httpClient;

extern HttpServer httpServer;

#endif

