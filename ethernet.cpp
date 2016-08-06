#ifndef ESP8266


#include "ethernet.h"
#include <EtherCard.h>

#define FAILED_REQUEST_RATE  5000 // Újrapróbálkozás milisec idő ha eddig nem sikerült lekérdezni
#define SUCCESS_REQUEST_RATE 60000 // Újrapróbálkozás milisec idő ha sikerült lekérdezni

#define PIN_ETHERNET_CS 10

// ethernet interface mac address
static byte mymac[] = { 0x74,0x61,0x62,0x2D,0x30,0x31 };
// remote website name
const char website[] PROGMEM = "danieleff.com";

static long mext_try_millis = 0;

byte Ethernet::buffer[600];

char* data;
int data_len;
int offset = 0;

// called when the client request is complete
static void my_result_cb (byte status, word off, word len) {
  
  Serial.print("<<< reply ");
  Serial.print(" ");
  Serial.println(status);
  Serial.println(off);
  Serial.println(len);
  
  //Serial.write((const char*) Ethernet::buffer + off, len);

  const char* response = Ethernet::buffer + off;
  if (offset == 0) {
    const char *response_without_header = strstr(response, "\r\n\r\n") + 4;
    len -= (response_without_header - response);
    response = response_without_header;
  }

  Serial.println(len);
  if (len > (data_len - offset)) {
    len = data_len - offset;
  }
  Serial.println(data_len);
  Serial.println(offset);
  Serial.println(len);
  
  memcpy(data + offset, response, len);
  offset += len;
  
  mext_try_millis = millis() + SUCCESS_REQUEST_RATE;
  
/*
  const char* countdownString = strstr((const char*) Ethernet::buffer + off, "\r\n\r\n");

  

  if (countdownString != NULL) {
    
    //end_millis = atol(countdownString) * 1000 + millis();

    mext_try_millis = millis() + SUCCESS_REQUEST_RATE;
    
  }
  */
  
}


void LaunchtimeEthernet::setup(char* _data, int _data_len) {
  data = _data;
  data_len = _data_len;
  offset = 0;
  
  if (ether.begin(sizeof Ethernet::buffer, mymac, PIN_ETHERNET_CS) == 0) 
    Serial.println( "Failed to access Ethernet controller");

  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
    
  ether.printIp("My IP: ", ether.myip);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);

  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");

  ether.printIp("Server: ", ether.hisip);
    
  ether.persistTcpConnection(true);
}


void LaunchtimeEthernet::loop() {
  
  ether.packetLoop(ether.packetReceive());
  
  if (millis() > mext_try_millis) {
    mext_try_millis = millis() + FAILED_REQUEST_RATE;
    Serial.println("\n>>> REQ");
    offset = 0;
    ether.browseUrl(PSTR("/countdown/"), "countdown.php?version=1", website, my_result_cb);
  }
}


#endif

