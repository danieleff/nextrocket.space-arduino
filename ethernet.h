#ifndef ETHERNET_H
#define ETHERNET_H

#include <Arduino.h>

#ifdef STM32
#include <EtherCard_STM.h>
#else
#include <EtherCard.h>
#endif

class LaunchtimeEthernet {
  public:
    void setup();
};

extern LaunchtimeEthernet net;

#endif

