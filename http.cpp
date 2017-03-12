#include "http.h"
#include "ethernet.h"
#include "settings.h"

#define FAILED_REQUEST_RATE  5000 // Újrapróbálkozás milisec idő ha eddig nem sikerült lekérdezni
#define SUCCESS_REQUEST_RATE 60000 // Újrapróbálkozás milisec idő ha sikerült lekérdezni

char url_buffer[sizeof(settings.url_setting_part) + sizeof(settings.url_const_part) + sizeof(settings.url_user_part)];

const char* parameter_rocket_set_selected = "?r=";

const char* parameter_rocket_get_selected = "/get_selected";

HttpClient httpClient;

HttpServer httpServer;


// called when the client request is complete
static void http_client_got_response (uint8_t status, uint16_t off, uint16_t len) {
  
  //static int counter = 0;
  /*
  Serial.print(F("Got response, size with headers: "));
  Serial.print(counter);
  counter ++;
  Serial.print(F(", "));
  Serial.print(len);
  Serial.print(F(", "));
  Serial.println(off);
  */
//  int data_len = sizeof(settings.launches);

  const char* response = ((char*)Ethernet::buffer) + off;
  
  Serial.write(response, len);
  
  const char *response_without_header = strstr(response, "\r\n\r\n") + 4;
  len -= (response_without_header - response);
  response = response_without_header;
  


  //if (len > (data_len - offset)) {
  //  len = data_len - offset;
  //}
  
  settings.launch_count = response[0];

  if (settings.selected_menu > settings.launch_count) {
    settings.selected_menu = SELECTED_CYCLE;
  }

  for(int i=0; i<len; i++) {
    settings.processApiResponse(i, response[i]);
  }
  //memcpy(settings.launches + offset, response + 1, len);
  httpClient.info_downloaded_millis = millis();

  settings.loadLaunch(0);
  //Serial.println(settings.launch.rocket);
  
  httpClient.next_try_millis = millis() + SUCCESS_REQUEST_RATE;
}

void HttpClient::loop() {
  if (millis() > next_try_millis) {
    next_try_millis = millis() + FAILED_REQUEST_RATE;
    
    strcpy(url_buffer, settings.url_setting_part);
    strcat(url_buffer, settings.url_const_part);
    strcat(url_buffer, settings.url_user_part);
    
    Serial.print(F("Http client sending request to: "));
    Serial.println(url_buffer);
    
    ether.browseUrl(PSTR(""), url_buffer, PSTR("nextrocket.space"), http_client_got_response);
  }
}

////////////////////
//Server


int main_page(char* request, char* response) {
  BufferFiller bfill = (uint8_t*)response;

  char buffer1[sizeof(settings.website)];
  char buffer2[sizeof(settings.url_setting_part)];
  
  strcpy(buffer1, settings.website);
  strcpy(buffer2, settings.url_setting_part);
  
  bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
      "Content-Type: text/html\r\n"
      "\r\n"
      "<!DOCTYPE html>"
      "<html><head><meta charset=\"utf-8\">"
      "<link rel=\"stylesheet\" type=\"text/css\" href=\"http://$S$S/style.css\">"
      "<script src=\"http://$S$S/script.js\"></script>"
      "<script src=\"http://$S$S/jquery.min.js\"></script>"
      "</head>"
      "<body>"
      "<h1>nextrocket.space</h1>"
      "<table id=\"launch_table\" class=\"launch_table\"></table>"
      "<script type='text/javascript'>init_embedded();</script>"
      "</body>"
      "</html>"
      ), buffer1, buffer2, buffer1, buffer2, buffer1, buffer2);
  
  return bfill.position();
}

int set_selected_rockets(char* request, char* response) {
  char* start = strstr(request, parameter_rocket_set_selected) + strlen(parameter_rocket_set_selected);
  char* end = strstr(start, " ");

  int size = end - start;

  if (size > sizeof(settings.url_user_part) - 1) {
    size = sizeof(settings.url_user_part) - 1;
  }
  
  strncpy(settings.url_user_part, start, size);

  settings.url_user_part[size] = '\0';

  settings.saveToEEPROM();

  httpClient.next_try_millis = millis();

  BufferFiller bfill = (uint8_t*) response;
  bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n\r\n"));
  
  return bfill.position();
}

int get_selected_rockets(char* request, char* response) {
  BufferFiller bfill = (uint8_t*) response;
  bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n\r\n"));
  bfill.emit_raw(settings.url_user_part, strlen(settings.url_user_part));
  return bfill.position();
}

void HttpServer::loop() {
  
  word pos = ether.packetLoop(ether.packetReceive());
  if (pos) {
    int size;
    
    char* request = (char*) Ethernet::buffer + pos;
    char* response = (char*) ether.tcpOffset();

    Serial.print(F("Http server received request: "));
    Serial.write(request, strchr(request, '\n') - request);
    Serial.println();

    if (strstr(request, parameter_rocket_set_selected)) {
      size = set_selected_rockets(request, response);
    } else if (strstr(request, parameter_rocket_get_selected)) {
      size = get_selected_rockets(request, response);
    } else {
      size = main_page(request, response);
    }

    Serial.print(F("Sending response, size: "));
    Serial.println(size);
    
    ether.httpServerReply(size);
    }
}

