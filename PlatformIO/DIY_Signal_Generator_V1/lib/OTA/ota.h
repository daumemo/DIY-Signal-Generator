#ifndef _OTA_FLASHING_H
#define _OTA_FLASHING_H

//#include <include/slist.h>
#include <Arduino.h>
#include <WiFi.h>
#include <mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
//#include "udpDebug.h"

void OtaSetup(void);
void OtaHandleRequests(void);

#endif
