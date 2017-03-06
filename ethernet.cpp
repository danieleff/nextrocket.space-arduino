#include "ethernet.h"
#include "settings.h"
#include "displays.h"
#include "config.h"

LaunchtimeEthernet net;

// ethernet interface mac address
static byte mymac[] = { 0x74,0x61,0x62,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];

extern uint32_t info_downloaded_millis;

void loop_error(const __FlashStringHelper *error) {
  while(1) {
    displays.write(error);
    delay(800);
    displays.write(F("        "));
    delay(200);
  }
}

void LaunchtimeEthernet::setup() {
  displays.write(F("NET BOOT"));
  
  Serial.println(F("Starting up ethernet"));
  if (ether.begin(sizeof Ethernet::buffer, mymac, PIN_ETHERNET_CS) == 0) {
      Serial.println(F("Failed to access Ethernet controller"));

      loop_error(F("NET ERR "));
  }


  displays.write(F("GET DHCP"));
  Serial.println(F("Get DHCP"));
  if (!ether.dhcpSetup()) {
      Serial.println(F("DHCP failed"));

      loop_error(F("DHCP ERR"));
  }
    
  ether.printIp(F("My IP: "), ether.myip);
  ether.printIp(F("GW IP: "), ether.gwip);
  ether.printIp(F("DNS IP: "), ether.dnsip);

  displays.write(F("GET DNS"));
  Serial.println(F("Get DNS"));
  if (!ether.dnsLookup(settings.website, true)) {
      Serial.println(F("DNS failed"));
      
      loop_error(F("DNS ERR "));
  }

  ether.printIp(F("Server: "), ether.hisip);
    
  ether.persistTcpConnection(true);
}

