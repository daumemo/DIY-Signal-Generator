#ifndef DEVICE_WIFI_LIB_H
#define DEVICE_WIFI_LIB_H

#include "Arduino.h"
#include "device.h"

void launchWeb(int webtype);
void handleRoot(void);
void handleData(void);
void handleOutput(void);
void handleWaveForm(void);
void handleFreq(void);
void handleAmpl(void);
void handleOffs(void);
void handleDuty(void);
void handleCharger();

void init_first_boot();
bool testWifi(void);
void setupAP(void);
void createWebServer(int webtype);

void server_do_stuff();
void wifi_turn_off();
String wifi_get_ssid();
String wifi_get_pass();
void wifi_turn_on();
void ap_turn_off();

String ap_get_ssid();
String ap_get_pass();
String wifi_get_ip();
void ap_save_to_eeprom(String ssid, String pass);
void wifi_save_to_eeprom(String ssid, String pass);
void eeprom_write_byte(int index, uint8_t value);
uint8_t eeprom_read_byte(int index);

#endif