#ifndef DEVICE_LIB_H
#define DEVICE_LIB_H

#include <Arduino.h>
//#include "bq24295.h"



#define BATTERY_READS_TO_AVERAGE 100
#define BATT_PERCENT_TO_AVERAGE 100

class Device_Object{

    private:
    int currentBrightness;
    volatile int currentBatteryVal;
    volatile int batteryArray[BATTERY_READS_TO_AVERAGE];
    volatile int battPercentArray[BATT_PERCENT_TO_AVERAGE];
    volatile int currentBattPercentIndex;
    bool wifiIsOn = false;
    bool apIsOn = false;

    double adcToMiliVolt(int adcvalue);
    int miliVoltToPercent(double voltval);

    public:

    Device_Object(void);

    void SetAP(String SSID, String pass);
    void SetWiFi(String SSID, String pass);
    void EnableAP(bool status);
    void EnableWiFi(bool status);
    String GetMyIP();
    bool IsApOn();
    bool IsWiFiOn();
    void MeasureBattery();
    int GetBatteryValue();
    void SetBrightness(int value);
    int GetBrightness();
    void ServerHandlesClient();
    String GetWiFiSSID();
    String GetWiFiPass();
    String GetApSSID();
    String GetApPass();

    void SaveSettings();
    void LoadSettings();

    //BQ24295_IC Charger;
};

extern Device_Object Device;

#endif