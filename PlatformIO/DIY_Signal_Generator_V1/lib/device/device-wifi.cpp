#include "device-wifi.h"
#include "lcd_interface.h"
#include "../../include/index.h"
#include <EEPROM.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <HTTPClient.h>

#include "../BQ24295/bq24295.h"

//BQ24295_IC charger2(15, 0, 100000);

WebServer server(80);
String apssid = "Generator"; //AP SSID
String appassphrase = "12345678"; //AP Password
String wifiScanResult = "";
String esid = "";
String epass = "";
bool accPointIsOn = false;

String wifi_get_ssid(){
  EEPROM.begin(512);
  delay(10);
  esid = "";
  for (int i = 0; i < 32; i++)
    {
      esid += char(EEPROM.read(i));
    }
  return esid;
}

String wifi_get_pass(){
  EEPROM.begin(512);
  delay(10);
  epass = "";
  for (int i = 32; i < 96; i++)
    {
      epass += char(EEPROM.read(i));
    }
  return epass;
}

String ap_get_ssid(){
  EEPROM.begin(512);
  delay(10);
  apssid = "";
  for (int i = 100; i < 132; i++)
    {
      apssid += char(EEPROM.read(i));
    }
  return apssid;
}

String ap_get_pass(){
  delay(10);
  appassphrase = "";
  for (int i = 132; i < 196; i++)
    {
      appassphrase += char(EEPROM.read(i));
    }
  return appassphrase;
}

String wifi_get_ip(){
  if(WiFi.isConnected())
  {return WiFi.localIP().toString();}
  else if(accPointIsOn){
    return WiFi.softAPIP().toString();
  }
  else return "";
}

void init_first_boot()
{
  EEPROM.begin(512);
  delay(10);
  esid = "";
  for (int i = 0; i < 32; ++i)
    {
      esid += char(EEPROM.read(i));
    }
  epass = "";
  for (int i = 32; i < 96; ++i)
    {
      epass += char(EEPROM.read(i));
    }
  if ( esid.length() > 1 ) {
      WiFi.mode(WIFI_STA);
      delay(200);
      WiFi.begin(esid.c_str(), epass.c_str());
      if (testWifi()) {
        
        launchWeb(0);
        return;
      }
  }
  
  if (!testWifi()) {
    setupAP();
  }
}

void wifi_turn_on(){
  EEPROM.begin(512);
  delay(10);
  esid = "";
  for (int i = 0; i < 32; ++i)
    {
      esid += char(EEPROM.read(i));
    }
  epass = "";
  for (int i = 32; i < 96; ++i)
    {
      epass += char(EEPROM.read(i));
    }
  if ( esid.length() > 1 ) {
      WiFi.mode(WIFI_STA);
      delay(200);
      WiFi.begin(esid.c_str(), epass.c_str());
      if (testWifi()) {
        
        launchWeb(0);
        return;
      }
  }
}

void wifi_turn_off(){
  server.stop();
  WiFi.disconnect();
}

void server_do_stuff(){
  server.handleClient();
}

bool testWifi(void) {
  int c = 0;
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED) { return true; }
    delay(500);
    c++;
  }
  return false;
}

void setupAP(void) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  /*delay(100);
  int n = WiFi.scanNetworks();
  String SSISstr = "\"SSID\":[";
  String RSSIstr = "\"RSSI\":[";
  String Protected = "\"Protected\":[";
  for (int i = 0; i < n; ++i)
    {
      if(i != 0) {
        RSSIstr += ", ";
        SSISstr += ", ";
        Protected += ", ";
      }
      RSSIstr += (WiFi.RSSI(i));
      SSISstr += "\""+ WiFi.SSID(i) + "\"";
      Protected += (WiFi.encryptionType(i) == 7)?"0":"1"; //if no encryption;
    }
  RSSIstr += "]";
  SSISstr += "]";
  Protected += "]";
  wifiScanResult = "{ " + RSSIstr +", "+ SSISstr + ", " + Protected +" }";
  delay(100);*/
  //ap_get_ssid();
  //ap_get_pass();
  accPointIsOn = WiFi.softAP(apssid.c_str(), appassphrase.c_str(), 6, 0);
  launchWeb(1);
}

void ap_save_to_eeprom(String ssid, String pass){
  /*if (qsid.length() > 0 && qpass.length() > 0) {
    for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
    for (int i = 0; i < qsid.length(); ++i){
      EEPROM.write(i, qsid[i]);
    }
    for (int i = 0; i < qpass.length(); ++i){
      EEPROM.write(32+i, qpass[i]);
    }
    EEPROM.commit();
    String s = SETTING_page;  
    server.send(200, "text/html", s);
  } else {
    server.send(404, "application/json", "{\"Error\":\"404 not found\"}");
  }*/

  if((ssid != apssid) || (pass != appassphrase)){
    if (ssid.length() > 0){
    for (int i = 100; i<196; i++){
      EEPROM.write(i, 0);
    }

    for(int i = 0; i<ssid.length(); i++){
      EEPROM.write(100+i, ssid[i]);
    }

    for(int i = 0; i<pass.length(); i++){
      EEPROM.write(132+i, pass[i]);
    }
    EEPROM.commit();
    apssid = ssid;
    appassphrase = pass;
  }
  }
  
}

void wifi_save_to_eeprom(String ssid, String pass){
  if((ssid != esid) || (pass != epass)){
    if (ssid.length() > 0){
    for (int i = 0; i<96; i++){
      EEPROM.write(i, 0);
    }

    for(int i = 0; i<ssid.length(); i++){
      EEPROM.write(0+i, ssid[i]);
    }

    for(int i = 0; i<pass.length(); i++){
      EEPROM.write(32+i, pass[i]);
    }
    EEPROM.commit();
    esid = ssid;
    epass = pass;
  }
  }
}

void eeprom_write_byte(int index, uint8_t value){
  EEPROM.write(index, value);
  EEPROM.commit();
}

uint8_t eeprom_read_byte(int index){
  EEPROM.begin(512);
  return EEPROM.read(index);
}

void ap_turn_off(){
  accPointIsOn = false;
  server.stop();
  WiFi.softAPdisconnect();
}

void launchWeb(int webtype) {
  createWebServer(webtype);
  server.begin(80);
}

void createWebServer(int webtype)
{
  if ( webtype == 1 ) {
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.on("/output", handleOutput);
    server.on("/waveform", handleWaveForm);
    server.on("/freq", handleFreq);
    server.on("/ampl", handleAmpl);
    server.on("/offs", handleOffs);
    server.on("/duty", handleDuty);
    server.on("/charger", handleCharger);
  } else if (webtype == 0) {
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.on("/output", handleOutput);
    server.on("/waveform", handleWaveForm);
    server.on("/freq", handleFreq);
    server.on("/ampl", handleAmpl);
    server.on("/offs", handleOffs);
    server.on("/duty", handleDuty);
    server.on("/charger", handleCharger);
  }
}

/****************************************************************************************************
 * 
 *                      Wi-Fi Handling Functions
 * 
 * *************************************************************************************************/

void handleCharger(){
  server.send(200, "text/json", "");
}

void handleData(){
  String returnStr = "";
  returnStr += "{";
  if(Generator.GetWaveform() == 0) returnStr += "\"wform\":\"" + String("sine") + "\", ";
  else if (Generator.GetWaveform() == 1) returnStr += "\"wform\":\"" + String("triangle") + "\", ";
  else if (Generator.GetWaveform() == 2) returnStr += "\"wform\":\"" + String("square") + "\", ";
  else returnStr += "\"wform\":\"" + String("pwm") + "\", ";
  returnStr += "\"freq\":" + String(Generator.GetFrequency(), '\003') + ", ";
  returnStr += "\"freqU\":\"" + String(Generator.GetFreqUnit()) + "\", ";
  returnStr += "\"ampl\":" + String(Generator.GetAmplitude()) + ", ";
  returnStr += "\"amplU\":\"" + String(Generator.GetAmplitudeUnit()) + "\", ";
  returnStr += "\"offset\":" + String(Generator.GetOffset()) + ", ";
  returnStr += "\"duty\":" + String(Generator.GetDuty()) + ", ";
  returnStr += "\"dutyU\":\"" + String(Generator.GetDutyUnit()) + "\", ";
  returnStr += "\"out\":" + String(Generator.GetOutputRelay()) + ", ";
  returnStr += "\"batt\":" + String(Device.GetBatteryValue());

  returnStr += "}";

  server.send(200, "text/json", returnStr);
}

void handleOutput(){
  String outputString(server.arg("value"));
  if(outputString.length() > 0){
    if(outputString.toInt() == 1){
      Generator.SetOutputRelay(true);
    }
    else {
      Generator.SetOutputRelay(false);
    }
    server.send(200, "text/json", "OK");
  }
  else{
    server.send(200, "text/json", "ERROR: empty or unknown value");
  }

  interface_update_output();
}

void handleWaveForm(){
  String valStr(server.arg("value"));
  if(valStr.length() > 0){
    if(valStr.toInt() == 0){
      Generator.SetWaveform(0);
    }
    else if (valStr.toInt() == 1){
      Generator.SetWaveform(1);
    }
    else if (valStr.toInt() == 2){
      Generator.SetWaveform(2);
    }
    else {
      Generator.SetWaveform(4);
    }
    server.send(200, "text/json", "OK");
  }
  else{
    server.send(200, "text/json", "ERROR: empty or unknown value");
  }

  interface_update();
}

void handleFreq(){
  String valStr(server.arg("value"));
  String unitStr(server.arg("unit"));
  if(valStr.length() > 0){
    if(unitStr.length() > 0){
      if(unitStr == "kHz"){
        Generator.SaveFreqUnit("kHz");
        Generator.SetFrequency(valStr.toDouble()*1000);
      }
      else if (unitStr == "MHz"){
        Generator.SaveFreqUnit("MHz");
        Generator.SetFrequency(valStr.toDouble()*1000000);
      }
      else {
        Generator.SaveFreqUnit("Hz");
        Generator.SetFrequency(valStr.toDouble());
      }
      server.send(200, "text/json", "OK");
    }
    else {
      Generator.SaveFreqUnit("kHz");
      Generator.SetFrequency(valStr.toDouble());
      server.send(200, "text/json", "ERROR: no unit value, set freq to Hz");
    }
    
  }
  else{
    server.send(200, "text/json", "ERROR: empty or unknown value");
  }

  interface_update();
}

void handleAmpl(){
  String valStr(server.arg("value"));
  String unitStr(server.arg("unit"));
  if(valStr.length() > 0){
    if(unitStr.length() > 0){
      if(unitStr == "Vpp"){
        Generator.SaveAmplitudeUnit("Vpp");
        Generator.SetAmplitude(valStr.toDouble()/2);
      }
      else {
        Generator.SaveAmplitudeUnit("V");
        Generator.SetAmplitude(valStr.toDouble());
      }
      server.send(200, "text/json", "OK");
    }
    else {
      Generator.SaveAmplitudeUnit("V");
      Generator.SetAmplitude(valStr.toDouble());
      server.send(200, "text/json", "ERROR: no unit value, set to V");
    }
    
  }
  else{
    server.send(200, "text/json", "ERROR: empty or unknown value");
  }

  interface_update();
}

void handleOffs(){
  String valStr(server.arg("value"));
  String unitStr(server.arg("unit"));
  if(valStr.length() > 0){
    Generator.SetOffset(valStr.toDouble());
    server.send(200, "text/json", "OK");
  }
  else{
    server.send(200, "text/json", "ERROR: empty or unknown value");
  }

  interface_update();
}

void handleDuty(){
  String valStr(server.arg("value"));
  String unitStr(server.arg("unit"));
  if(valStr.length() > 0){
    if(unitStr.length() > 0){
      if(unitStr == "%"){
        Generator.SaveDutyUnit("%");
        Generator.SetDuty(valStr.toDouble());
      }
      else{
        Generator.SaveDutyUnit("-");
        Generator.SetDuty(valStr.toDouble()*100);
      }
      server.send(200, "text/json", "OK");
    }
    else{
      Generator.SaveDutyUnit("%");
      Generator.SetDuty(valStr.toDouble());
      server.send(200, "text/json", "ERROR: no unit value, set to %");
    }
  }
  else{
    server.send(200, "text/json", "ERROR: empty or unknown value");
  }

  interface_update();
}

void handleRoot()
{
  String s = MAIN_page;             //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

/************************************************************************************
 * 
 *              END of Wi-Fi Page Handlers
 * 
 * *********************************************************************************/