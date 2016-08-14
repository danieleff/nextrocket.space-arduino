#ifndef ESP8266

#include "ethernet.h"
#include "settings.h"

#include <EtherCard.h>

#define PIN_ETHERNET_CS 10

LaunchtimeEthernet net;

// ethernet interface mac address
static byte mymac[] = { 0x74,0x61,0x62,0x2D,0x30,0x31 };

byte Ethernet::buffer[600];

extern uint32_t info_downloaded_millis;

void LaunchtimeEthernet::setup() {
  
  if (ether.begin(sizeof Ethernet::buffer, mymac, PIN_ETHERNET_CS) == 0) 
    Serial.println(F("Failed to access Ethernet controller"));

  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
    
  ether.printIp(F("My IP: "), ether.myip);
  ether.printIp(F("GW IP: "), ether.gwip);
  ether.printIp(F("DNS IP: "), ether.dnsip);

  if (!ether.dnsLookup(settings.website, true))
    Serial.println(F("DNS failed"));

  ether.printIp(F("Server: "), ether.hisip);
    
  ether.persistTcpConnection(true);
}

#endif

