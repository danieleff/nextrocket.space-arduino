#ifndef ESP8266

#include "ethernet.h"
#include "settings.h"
#include "display.h"

#include <EtherCard.h>

#define PIN_ETHERNET_CS 10

LaunchtimeEthernet net;

extern Display display;

// ethernet interface mac address
static byte mymac[] = { 0x74,0x61,0x62,0x2D,0x30,0x31 };

byte Ethernet::buffer[600];

extern uint32_t info_downloaded_millis;

void loop_error(char* error) {
  while(1) {
    display.write(error);
    delay(800);
    display.write("        ");
    delay(200);
  }
}

void LaunchtimeEthernet::setup() {
  display.write("NET BOOT");
  if (ether.begin(sizeof Ethernet::buffer, mymac, PIN_ETHERNET_CS) == 0) {
      Serial.println(F("Failed to access Ethernet controller"));

      loop_error("NET ERR ");
  }


  display.write("GET DHCP");
  if (!ether.dhcpSetup()) {
      Serial.println(F("DHCP failed"));

      loop_error("DHCP ERR");
  }
    
  ether.printIp(F("My IP: "), ether.myip);
  ether.printIp(F("GW IP: "), ether.gwip);
  ether.printIp(F("DNS IP: "), ether.dnsip);

  display.write("GET DNS");
  if (!ether.dnsLookup(settings.website, true)) {
      Serial.println(F("DNS failed"));
      
      loop_error("DNS ERR ");
  }

  ether.printIp(F("Server: "), ether.hisip);
    
  ether.persistTcpConnection(true);
}

#endif

