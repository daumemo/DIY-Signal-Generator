/************************************************************************************
 * 
 * This is an example code used to test R61529 based LCD with FT6236 touch controller
 * connected to an ESP32 MCU through parallel(8080) interface. This code uses eSPI 
 * library
 * 
 * 
 * Note: Although you will find some files that contains R61581 in it, but be aware 
 * that those are modified files from original eSPI library to support R61529 ICs
 * (Some or all code was changed in those header files).
 * 
 * LCD Connection to the ESP32:
 * 
 * D0 -> GPIO 16
 * D1 -> GPIO 17
 * D2 -> GPIO 18
 * D3 -> GPIO 19
 * D4 -> GPIO 21
 * D5 -> GPIO 22
 * D6 -> GPIO 23
 * D7 -> GPIO 25
 * 
 * CSX -> GPIO 33
 * DCX -> GPIO 26
 * RSX -> GPIO 32
 * WRX -> GPIO 27
 * 
 * TOUCH IC connection to the ESP32:
 * 
 * SDA -> GPIO 15
 * SCL -> GPIO 14->0
 * INT -> GPIO 35
 * RST -> GPIO 32 (connected together with the LCD reset pin) 
 *
 *************************************************************************************/


#include <Arduino.h>

#include "../lib/TFT_eSPI/TFT_eSPI.h"
#include "Wire.h"

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "ota.h"
#include <EEPROM.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <HTTPClient.h>

#include <SPI.h>

#include "../lib/AD9833/ad9833.h"
#include "bq24295.h"
#include "../lib/lvgl/src/lvgl.h"

#include "../lib/lcd_interface/lcd_interface.h"
#include "../lib/touch_drv/touch_drv.h"
#include "../lib/generator/generator.h"
#include "../lib/device/device.h"


uint64_t battMeasureTimer = 0;
int battUpdateTimer = 0;


void setup()
{
  
  Device.LoadSettings();
  
  if(Device.IsApOn() || Device.IsWiFiOn()){
    OtaSetup();
  }

  BQ24295_IC charger(15, 0, 100000);

  charger.setI2cWatchdogTimer(0);
  charger.setFastChargeCurrent(0b010000);
  charger.setPreChargeCurrent(0b0001);
  charger.setFastChargeTimer(0);
  charger.setBoostVoltage(0b1000);
  charger.enableOTG(false);

  
  touchInit();
  Wire.beginTransmission(0x38);
  Wire.write(0xA4);
  Wire.write(0x00); //turn on interrupt
  Wire.endTransmission();

  
  interface_init(); //initialise lcd interface: eSPI and lvgl
  
  Device.MeasureBattery();
  interface_update_battery();

}

void loop()
{
  
  OtaHandleRequests();

  Device.ServerHandlesClient();
  lv_task_handler(); /* let the GUI do its work */
 
  delay(1);
  battMeasureTimer++;
  if(battMeasureTimer > 1000){
    battMeasureTimer = 0;
    Device.MeasureBattery();
    if(battUpdateTimer > 5){
      battUpdateTimer = 0;
      interface_update_battery();
    }
    else {
      battUpdateTimer++;
    }
  }
}

