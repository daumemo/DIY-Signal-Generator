#include "device.h"
#include "generator.h"
#include "lcd_interface.h"
#include "device-wifi.h"

#include <WebServer.h>

#define EEPROM_ADDRESS_BRIGHTNESS 199
#define EEPROM_ADDRESS_WIFI_AP_STATUS 198

Device_Object Device;

Device_Object::Device_Object(){

    currentBattPercentIndex = -1;

    ledcSetup(6, 5000, 8);
    ledcAttachPin(12, 6);
    ledcWrite(6, 255);// brightness

    analogSetAttenuation(ADC_11db);
}

void Device_Object::SetAP(String SSID, String pass){
    ap_save_to_eeprom(SSID, pass);
}

void Device_Object::SetWiFi(String SSID, String pass){
    wifi_save_to_eeprom(SSID, pass);
}

void Device_Object::SaveSettings(){
    if (eeprom_read_byte(EEPROM_ADDRESS_BRIGHTNESS) != currentBrightness){
        eeprom_write_byte(EEPROM_ADDRESS_BRIGHTNESS, (uint8_t)currentBrightness);
    }

    if(IsWiFiOn()){
        if(eeprom_read_byte(EEPROM_ADDRESS_WIFI_AP_STATUS) != 1){
            eeprom_write_byte(EEPROM_ADDRESS_WIFI_AP_STATUS, 1);
        }
    }
    else if (IsApOn()){
        if(eeprom_read_byte(EEPROM_ADDRESS_WIFI_AP_STATUS) != 2){
            eeprom_write_byte(EEPROM_ADDRESS_WIFI_AP_STATUS, 2);
        }
    }
    else {
        if(eeprom_read_byte(EEPROM_ADDRESS_WIFI_AP_STATUS) != 0){
            eeprom_write_byte(EEPROM_ADDRESS_WIFI_AP_STATUS, 0);
        }
    }
}

void Device_Object::LoadSettings(){
    SetBrightness(eeprom_read_byte(EEPROM_ADDRESS_BRIGHTNESS));

    ap_get_pass();
    ap_get_ssid();

    uint8_t wifiStat = eeprom_read_byte(EEPROM_ADDRESS_WIFI_AP_STATUS);
    if (wifiStat == 1){
        EnableWiFi(true);
    }
    else if (wifiStat == 2){
        EnableAP(true);
    }
    else {
        EnableAP(false);
        EnableWiFi(false);
    }
}

void Device_Object::EnableAP(bool status){
    apIsOn = status;
    if(status){
        if(wifiIsOn){
            EnableWiFi(false);
            setupAP();
        }
        else {
            setupAP();
        }
    }
    else{
        //kill ap;
        ap_turn_off();
    }
}

void Device_Object::EnableWiFi(bool status){
    wifiIsOn = status;
    if(status){
        if(apIsOn){
            EnableAP(false);
            //init_first_boot();
            wifi_turn_on();
        }
        else{
            wifi_turn_on();
        }
    }
    else{
        wifi_turn_off();
    }
}

String Device_Object::GetWiFiSSID(){
    return wifi_get_ssid();
}

String Device_Object::GetWiFiPass(){
    return wifi_get_pass();
}

String Device_Object::GetApSSID(){
    return ap_get_ssid();
}

String Device_Object::GetApPass(){
    return ap_get_pass();
}

String Device_Object::GetMyIP(){
    return wifi_get_ip();
}

bool Device_Object::IsApOn(){
    return apIsOn;
}

bool Device_Object::IsWiFiOn(){
    return wifiIsOn;
}

void Device_Object::ServerHandlesClient(){
    server_do_stuff();
}



void Device_Object::MeasureBattery(){
    int temp = 0;
    int temp2 = 0;
    analogRead(36);
    if(currentBattPercentIndex == -1){
        currentBattPercentIndex = 0;
        for (int j = 0; j< BATT_PERCENT_TO_AVERAGE; j++){
            temp = 0;
            for(int i = 0; i< BATTERY_READS_TO_AVERAGE; i++){
                batteryArray[i] = analogRead(36);
                temp += batteryArray[i];
            }
            battPercentArray[j] = miliVoltToPercent(adcToMiliVolt(temp / BATTERY_READS_TO_AVERAGE));
        }

        for(int i = 0; i<BATT_PERCENT_TO_AVERAGE; i++){
            temp2 += battPercentArray[i];
        }

        currentBatteryVal = temp2 / BATT_PERCENT_TO_AVERAGE;
    }
    else {
        if(currentBattPercentIndex >= BATT_PERCENT_TO_AVERAGE){
            currentBattPercentIndex = 0;
        }
        for(int i = 0; i< BATTERY_READS_TO_AVERAGE; i++){
            batteryArray[i] = analogRead(36);
            temp += batteryArray[i];
        }

        battPercentArray[currentBattPercentIndex] = miliVoltToPercent(adcToMiliVolt(temp / BATTERY_READS_TO_AVERAGE));
        currentBattPercentIndex++;

        for(int i = 0; i<BATT_PERCENT_TO_AVERAGE; i++){
            temp2 += battPercentArray[i];
        }

        currentBatteryVal = temp2 / BATT_PERCENT_TO_AVERAGE;
    }
}

int Device_Object::GetBatteryValue(){
    return currentBatteryVal;
}

void Device_Object::SetBrightness(int value){
    ledcWrite(6, value);
    currentBrightness = value;
}

int Device_Object::GetBrightness(){
    return currentBrightness;
}


double Device_Object::adcToMiliVolt(int adcvalue){
    return (1.2973*adcvalue + 470.497);
}


int Device_Object::miliVoltToPercent(double voltval){
    if(voltval >= 4200){
        return 100;
    }
    else if(voltval > 4000){
        return (int)(0.05*voltval - 110);
    }
    else if(voltval >= 3600){
        return (int)(0.175*voltval - 610 + 0.5);
    }
    else if(voltval >= 3000){
        return (int)(0.03167*voltval - 94 + 0.5);
    }
    else return 0;
}

