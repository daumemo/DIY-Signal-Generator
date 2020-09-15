#include "lcd_interface.h"
#include "../touch_drv/touch_drv.h"
#include <Arduino.h>
#include "../device/device.h"

#define MAX_CHAR_COUNT 20

#define LABEL_SINE "SINE"
#define LABEL_TRI "TRIANGLE"
#define LABEL_SQ "SQUARE"
#define LABEL_PWM "PWM"

static void kb_event_cb(lv_obj_t * _kb, lv_event_t e);

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

static lv_obj_t * kb;

static lv_disp_buf_t disp_buf; //LVGL stuff;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

lv_disp_drv_t disp_drv;
lv_indev_drv_t indev_drv;
lv_obj_t *screenMain;
static lv_style_t bigStyle;
static lv_style_t mediumStyle;
lv_obj_t *labelFreqNum;
lv_obj_t *labelFreqUnit;
lv_obj_t *labelAmplNum;
lv_obj_t *labelAmplUnit;
lv_obj_t *labelOffNum;
lv_obj_t *labelOffUnit;
lv_obj_t *labelBattery;
lv_obj_t *labelWiFi;
static lv_style_t style_led;
lv_obj_t *ledOutput;
lv_obj_t *labelOutput;
lv_obj_t *page;
lv_obj_t *labelWaveForm;
lv_obj_t *labelLeftChannel;
lv_obj_t *labelRightChannel;

lv_obj_t *checkBoxShowPass;

lv_obj_t *labelWiFiIP;

static lv_style_t styleBattLow;
static lv_style_t styleBattOk;

static const char * btnm_map[] = {"1", "#ff0000 2#", "3", "4", "5", "\n",
                                  "6", "7", "8", "9", "0", "\n",
                                  "Action1", "Action2", ""};

static const char * btnm_map2[] = {"SINE", "TRI", "SQ", "PWM", "...", "\n", 
                                  " ", " ", " ", " ", "Freq", "\n",
                                  " ", " ", " ", " ", "Amp", "\n",
                                  " ", " ", " ", " ", "Offs", "\n",
                                  " ", " ", " ", "Output",""};

static const char * btnm_map_freq[] = {" ", " ", " ", " ", "...", "\n", 
                                  "1", "2", "3", "Hz", "Freq", "\n",
                                  "4", "5", "6", "kHz", " ", "\n",
                                  "7", "8", "9", "MHz", " ", "\n",
                                  ".", "0", " ", "Output",""};

static const char * btnm_map_amp[] = {" ", " ", " ", " ", "...", "\n", 
                                  "1", "2", "3", "Vpp", " ", "\n",
                                  "4", "5", "6", " ", "Amp", "\n",
                                  "7", "8", "9", " ", " ", "\n",
                                  ".", "0", " ", "Output",""};

static const char * btnm_map_offs[] = {" ", " ", " ", " ", "...", "\n", 
                                  "1", "2", "3", "V", " ", "\n",
                                  "4", "5", "6", " ", " ", "\n",
                                  "7", "8", "9", " ", "Offs", "\n",
                                  ".", "0", "+/-", "Output",""};

static const char * btnm_map_pwm[] = {"SINE", "TRI", "SQ", "PWM", "...", "\n", 
                                  " ", " ", " ", " ", "Freq", "\n",
                                  " ", " ", " ", " ", "Duty", "\n",
                                  " ", " ", " ", " ", " ", "\n",
                                  " ", " ", " ", "Output",""};

static const char * btnm_map_pwm_duty[] = {"SINE", "TRI", "SQ", "PWM", "...", "\n", 
                                  "1", "2", "3", "%", " ", "\n",
                                  "4", "5", "6", " ", "Duty", "\n",
                                  "7", "8", "9", " ", " ", "\n",
                                  ".", "0", " ", "Output",""};

lv_obj_t * btnm1;

lv_obj_t *screenSettings;
lv_obj_t *labelSettingName;
lv_obj_t *labelConnectToWifi;
lv_obj_t *swConnectToWifi;
lv_obj_t *labelWifiSsid;
lv_obj_t *taSSID;
lv_obj_t *labelWifiPass;
lv_obj_t *taWifiPass;
lv_obj_t *labelCreateWifi;
lv_obj_t *swCreateWifi;
lv_obj_t *labelOwnWifiSsid;
lv_obj_t *taOwnSSID;
lv_obj_t *labelOwnWifiPass;
lv_obj_t *taOwnWifiPass;
lv_obj_t *labelDisplayBrightness;
lv_obj_t *sliderDispBrightness;


int aParamBeingEdited = 0; // 0 - no edit, 1 - freq, 2 - amp/duty, 3 - offs 
char writeableString[MAX_CHAR_COUNT];
char oldEditableString[MAX_CHAR_COUNT];
char oldEditableUnit[MAX_CHAR_COUNT];
bool pointCharHasBeenUsed = false;
int numbersAfterPoint = 0;
//int waveformUsed = 0; //default sine
//int freqUnitUsed = 1; //default khz
//int amplUnitUsed = 0; //defaul Vpp

bool outputRelayIsTurnedOn = false;


double charStringToDouble(char myArray[]){
    String resultString = String(myArray);
    return resultString.toDouble();
}

void doubleToCharString(double value, char myArray[]){
    String resultString = String(value,'\003');
    strcpy(myArray, resultString.c_str());
    myArray[MAX_CHAR_COUNT-1] = '\0';
}

void doubleToCharString(double value, char myArray[], char digits){
    String resultString = String(value, digits);
    strcpy(myArray, resultString.c_str());
    myArray[MAX_CHAR_COUNT-1] = '\0';
}

void updateLcdValues(){ // numbers and units
    if(Generator.GetFreqUnit() == "Hz"){ //Hz
        lv_label_set_text(labelFreqUnit, "Hz");
        doubleToCharString(Generator.GetFrequency(), writeableString, '\003');
        lv_label_set_text(labelFreqNum, writeableString);
    }
    else if (Generator.GetFreqUnit() == "kHz"){
        lv_label_set_text(labelFreqUnit, "kHz");
        doubleToCharString(Generator.GetFrequency()/1000, writeableString, '\003');
        lv_label_set_text(labelFreqNum, writeableString);
    }
    else if (Generator.GetFreqUnit() == "MHz"){
        lv_label_set_text(labelFreqUnit, "MHz");
        doubleToCharString(Generator.GetFrequency()/1000000, writeableString, '\003');
        lv_label_set_text(labelFreqNum, writeableString);
    }

    if(Generator.GetWaveform() != 4){ // sine, tri, sq
        if (Generator.GetAmplitudeUnit() == "Vpp"){//Vpp
            lv_label_set_text(labelAmplUnit, "Vpp");
            doubleToCharString(Generator.GetAmplitude()*2, writeableString, '\002');
            lv_label_set_text(labelAmplNum, writeableString);
        }
        else{
            lv_label_set_text(labelAmplUnit, "V");
            doubleToCharString(Generator.GetAmplitude(), writeableString, '\002');
            lv_label_set_text(labelAmplNum, writeableString);
        }

        lv_label_set_text(labelOffUnit, "Voff");
        doubleToCharString(Generator.GetOffset(), writeableString, '\002');
        lv_label_set_text(labelOffNum, writeableString);

        lv_label_set_text(labelRightChannel, LV_SYMBOL_UP);
        lv_label_set_text(labelLeftChannel, " ");
    }
    else{//pwm
        if (Generator.GetDutyUnit() == "%"){//%
            lv_label_set_text(labelAmplUnit, "%");
            doubleToCharString(Generator.GetDuty(), writeableString, '\001');
        }
        else {
            lv_label_set_text(labelAmplUnit, "");
            doubleToCharString(Generator.GetDuty()/100, writeableString,'\001');
        }
        
        lv_label_set_text(labelAmplNum, writeableString);

        lv_label_set_text(labelOffUnit, "---");
        lv_label_set_text(labelOffNum, "");

        lv_label_set_text(labelRightChannel, " ");
        lv_label_set_text(labelLeftChannel, LV_SYMBOL_UP);
    }

}


void appendWithChar(char* str, char c) {
        int len = strlen(str);
        str[len] = c;
        str[len+1] = '\0';
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

bool my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
  int XL = 0;
  int XH = 0;
  int YL = 0;
  int YH = 0;
  int tStatus = 0;

  XH = readTouchReg(TOUCH_REG_XH);
  XL = readTouchReg(TOUCH_REG_XL);
  YH = readTouchReg(TOUCH_REG_YH);
  YL = readTouchReg(TOUCH_REG_YL);

  data->point.x = ((XH & 0x0F) << 8) | XL;
  data->point.y = ((YH & 0x0F) << 8) | YL;
  tStatus = XH>>6;
  if (tStatus == 1){
    data->state = LV_INDEV_STATE_REL;
  }
  else {
    data->state = LV_INDEV_STATE_PR;
  }
  //data->state = LV_INDEV_STATE_PR; // or LV_INDEV_STATE_REL;
  return false; /*No buffering now so no more data read*/
}

static void swConnectWiFiHandler(lv_obj_t * obj, lv_event_t event){
    if(event == LV_EVENT_VALUE_CHANGED) {
        //printf("State: %s\n", lv_switch_get_state(obj) ? "On" : "Off");
        if(lv_switch_get_state(obj)){
            //turn on wifi
            Device.SetWiFi(String(lv_textarea_get_text(taSSID)), String(lv_textarea_get_text(taWifiPass)));
            Device.EnableWiFi(true);
            lv_switch_off(swCreateWifi, LV_ANIM_ON);
            lv_label_set_text(labelWiFiIP, Device.GetMyIP().c_str());
            lv_label_set_text(labelWiFi, LV_SYMBOL_WIFI);
        }
        else{//turn off wifi
            Device.EnableWiFi(false);
            lv_label_set_text(labelWiFiIP, "");
            lv_label_set_text(labelWiFi, "");
        }
    }
}

static void eventCheckPass (lv_obj_t * obj, lv_event_t event){
    if(event == LV_EVENT_VALUE_CHANGED) {
        if (lv_checkbox_is_checked(obj)){
            lv_textarea_set_pwd_mode(taOwnWifiPass, false);
        }
        else {
            lv_textarea_set_pwd_mode(taOwnWifiPass, true);
        }
    }
}

static void swConnectAPHandler(lv_obj_t * obj, lv_event_t event){
    if(event == LV_EVENT_VALUE_CHANGED) {
        //printf("State: %s\n", lv_switch_get_state(obj) ? "On" : "Off");
        if(lv_switch_get_state(obj)){
            //turn on ap
            Device.SetAP(String(lv_textarea_get_text(taOwnSSID)), String(lv_textarea_get_text(taOwnWifiPass)));
            Device.EnableAP(true);
            lv_switch_off(swConnectToWifi, LV_ANIM_ON);
            lv_label_set_text(labelWiFiIP, Device.GetMyIP().c_str());
            lv_label_set_text(labelWiFi, LV_SYMBOL_WIFI);
        }
        else{//turn off ap
            Device.EnableAP(false);
            lv_label_set_text(labelWiFiIP, Device.GetMyIP().c_str());
            lv_label_set_text(labelWiFi, "");
        }
    }
}

static void ta_event_cb(lv_obj_t * ta, lv_event_t e)
{
    if(e == LV_EVENT_RELEASED) {
        if(kb == NULL) {
            lv_obj_set_height(page, LV_VER_RES / 2);
            if ((ta == taOwnSSID) || (ta == taOwnWifiPass)) lv_page_scroll_ver(page, -120);
            kb = lv_keyboard_create(lv_scr_act(), NULL);
            lv_obj_set_event_cb(kb, kb_event_cb);

            lv_indev_wait_release(lv_indev_get_act());
        }
        lv_textarea_set_cursor_hidden(ta, false);
        //lv_page_focus(t1, lv_textarea_get_label(ta), LV_ANIM_ON);
        lv_keyboard_set_textarea(kb, ta);
        
        
    } else if(e == LV_EVENT_DEFOCUSED) {
        lv_textarea_set_cursor_hidden(ta, true);
    }
}


static void kb_event_cb(lv_obj_t * _kb, lv_event_t e)
{
    lv_keyboard_def_event_cb(kb, e);

    if(e == LV_EVENT_CANCEL || e == LV_EVENT_APPLY) {
        if(kb) {
            lv_obj_set_height(page, LV_VER_RES);
            lv_obj_del(kb);
            kb = NULL;
        }
    }
}


static void event_handler_exit_btn(lv_obj_t * obj, lv_event_t event){
    if(event == LV_EVENT_CLICKED) {
        Device.SaveSettings();
        lv_scr_load(screenMain);
    }
}


static void brght_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        
        //ledcWrite(6, (lv_slider_get_value(obj)*254/100 + 1));
        Device.SetBrightness((lv_slider_get_value(obj)*254/100 + 1));
    }
}


void appendWriteableString(){
    if(pointCharHasBeenUsed){
        if(aParamBeingEdited == 1){
             if(numbersAfterPoint == 0){
                    strcat(writeableString, "000");
                }
                else if(numbersAfterPoint == 1){
                    strcat(writeableString, "00");
                }
                else if(numbersAfterPoint == 2)
                {
                    strcat(writeableString, "0");
                }
                
            }
            else{
                strcat(writeableString, ".000");
            }
        }
        else if (aParamBeingEdited == 2){

        }
               
    writeableString[19] = '\0';
}

static void btn_mtx_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        const char * txt = lv_btnmatrix_get_active_btn_text(obj);
        int buttonNumber = lv_btnmatrix_get_active_btn(obj);

        //printf("%s was pressed\n", txt);
        //lv_label_set_text(labelFreqNum, txt);
        if (strcmp(txt, btnm_map2[4]) == 0){
            lv_scr_load(screenSettings);
        }

        if (strcmp(txt, btnm_map2[10]) == 0){ //set frequency
            if(aParamBeingEdited == 0){
                lv_btnmatrix_set_map(btnm1, btnm_map_freq);
                lv_btnmatrix_set_btn_width(btnm1, 23, 2); 
                strlcpy(oldEditableString, lv_label_get_text(labelFreqNum), 10);
                strlcpy(oldEditableUnit, lv_label_get_text(labelFreqUnit), 5);
                writeableString[0] = '\0';
                lv_label_set_text(labelFreqNum, (char *)writeableString);
                lv_label_set_text(labelFreqUnit, "---");
                pointCharHasBeenUsed = false;
                numbersAfterPoint = 0;
                aParamBeingEdited = 1; //editing frquency;
            }
            else if (aParamBeingEdited == 1){
                if(Generator.GetWaveform() == 4){ //if pwm
                    lv_btnmatrix_set_map(btnm1, btnm_map_pwm);
                    lv_btnmatrix_set_btn_width(btnm1, 23, 2);
                }
                else{
                    lv_btnmatrix_set_map(btnm1, btnm_map2);
                    lv_btnmatrix_set_btn_width(btnm1, 23, 2);
                }
                lv_label_set_text(labelFreqNum, (char *)oldEditableString);
                lv_label_set_text(labelFreqUnit, (char *)oldEditableUnit);
                aParamBeingEdited = 0; //disable freq edit;
            }
        }

        if(buttonNumber == 14){//set amplitude/duty
            if (aParamBeingEdited == 0){
                if(Generator.GetWaveform() == 4){//if duty
                    lv_btnmatrix_set_map(btnm1, btnm_map_pwm_duty);
                    lv_btnmatrix_set_btn_width(btnm1, 23, 2); 
                }
                else {
                    lv_btnmatrix_set_map(btnm1, btnm_map_amp);
                    lv_btnmatrix_set_btn_width(btnm1, 23, 2); 
                }
                
                strlcpy(oldEditableString, lv_label_get_text(labelAmplNum), 10);
                strlcpy(oldEditableUnit, lv_label_get_text(labelAmplUnit), 5);
                writeableString[0] = '\0';
                lv_label_set_text(labelAmplNum, (char *)writeableString);
                lv_label_set_text(labelAmplUnit, "---");
                pointCharHasBeenUsed = false;
                numbersAfterPoint = 0;
                aParamBeingEdited = 2; //editing ampl/duty;
            }
            else if (aParamBeingEdited == 2){
                if (Generator.GetWaveform() == 4){
                    lv_btnmatrix_set_map(btnm1, btnm_map_pwm);
                    lv_btnmatrix_set_btn_width(btnm1, 23, 2);
                }
                else {
                    lv_btnmatrix_set_map(btnm1, btnm_map2);
                    lv_btnmatrix_set_btn_width(btnm1, 23, 2);
                }
                lv_label_set_text(labelAmplNum, (char *)oldEditableString);
                lv_label_set_text(labelAmplUnit, (char *)oldEditableUnit);
                aParamBeingEdited = 0; //disable ampl/duty edit;
            }
        }

        if((buttonNumber == 19) && (Generator.GetWaveform() != 4)){//set offset, ignore when pwm
            if (aParamBeingEdited == 0){
                lv_btnmatrix_set_map(btnm1, btnm_map_offs);
                lv_btnmatrix_set_btn_width(btnm1, 23, 2); 
                strlcpy(oldEditableString, lv_label_get_text(labelOffNum), 10);
                strlcpy(oldEditableUnit, lv_label_get_text(labelOffUnit), 5);
                writeableString[0] = '\0';
                lv_label_set_text(labelOffNum, (char *)writeableString);
                lv_label_set_text(labelOffUnit, "---");
                pointCharHasBeenUsed = false;
                numbersAfterPoint = 0;
                aParamBeingEdited = 3; //editing offs;
            }
            else if (aParamBeingEdited == 3){
                lv_btnmatrix_set_map(btnm1, btnm_map2);
                lv_btnmatrix_set_btn_width(btnm1, 23, 2);
                lv_label_set_text(labelOffNum, (char *)oldEditableString);
                lv_label_set_text(labelOffUnit, (char *)oldEditableUnit);
                aParamBeingEdited = 0; //disable offs edit;
            }
        }

        if(numbersAfterPoint < 3){
            if((buttonNumber == 5) || (buttonNumber == 6) || (buttonNumber == 7) || (buttonNumber == 10) || (buttonNumber == 11) || (buttonNumber == 12) || (buttonNumber == 15) || (buttonNumber == 16) || (buttonNumber == 17) || (buttonNumber == 21)){
            //if number button;
            if(aParamBeingEdited == 1){//write to freq field
                strcat(writeableString, txt);
                lv_label_set_text(labelFreqNum, (char *)writeableString);
            }
            if(aParamBeingEdited == 2){//write to ampl field
                strcat(writeableString, txt);
                lv_label_set_text(labelAmplNum, (char *)writeableString);
            }
            if(aParamBeingEdited == 3){//write to offs field
                strcat(writeableString, txt);
                lv_label_set_text(labelOffNum, (char *)writeableString);
            }

            if(pointCharHasBeenUsed) numbersAfterPoint++;
        }
        }
        

        if((buttonNumber == 20) && (!pointCharHasBeenUsed)){// point
            pointCharHasBeenUsed = true;
            strcat(writeableString, txt);
            if(aParamBeingEdited == 1){//write to freq field
            lv_label_set_text(labelFreqNum, (char *)writeableString);
            }
            if(aParamBeingEdited == 2){
                lv_label_set_text(labelAmplNum, (char *)writeableString);
            }
            if(aParamBeingEdited == 3){
                lv_label_set_text(labelOffNum, (char *)writeableString);
            }
        }

        if(((buttonNumber == 8) || (buttonNumber == 13) || (buttonNumber == 18)) && (aParamBeingEdited == 1)){ //frequency
            if(Generator.GetWaveform() == 4){
                lv_btnmatrix_set_map(btnm1, btnm_map_pwm);
                lv_btnmatrix_set_btn_width(btnm1, 23, 2);
            }
            else {
                lv_btnmatrix_set_map(btnm1, btnm_map2);
                lv_btnmatrix_set_btn_width(btnm1, 23, 2);
            }
            
            lv_label_set_text(labelFreqUnit, txt);
            aParamBeingEdited = 0;
            //appendWriteableString();
            lv_label_set_text(labelFreqNum, (char *)writeableString);
            
            if(buttonNumber == 8){ // freq in Hz
                Generator.SetFrequency(charStringToDouble(writeableString));
                Generator.SaveFreqUnit("Hz");
                //freqUnitUsed = 0;
            }
            else if (buttonNumber == 13){ // freq in kHz
                Generator.SetFrequency(charStringToDouble(writeableString)*1000);
                Generator.SaveFreqUnit("kHz");
                //freqUnitUsed = 1;
            }
            else if (buttonNumber == 18){ // freq in MHz
                Generator.SetFrequency(charStringToDouble(writeableString)*1000000);
                Generator.SaveFreqUnit("MHz");
                //freqUnitUsed = 2;
            }
            updateLcdValues();
        }

        if((buttonNumber == 8) && (aParamBeingEdited == 2)){//amplitude,duty
            if(Generator.GetWaveform() == 4){
                lv_btnmatrix_set_map(btnm1, btnm_map_pwm);
                lv_btnmatrix_set_btn_width(btnm1, 23, 2);
                lv_label_set_text(labelAmplUnit, "%");
                Generator.SetDuty(charStringToDouble(writeableString));
                Generator.SaveDutyUnit("%");
            }
            else {
                lv_btnmatrix_set_map(btnm1, btnm_map2);
                lv_btnmatrix_set_btn_width(btnm1, 23, 2);
                lv_label_set_text(labelAmplUnit, "Vpp");
                Generator.SetAmplitude(charStringToDouble(writeableString)/2);
                Generator.SaveAmplitudeUnit("Vpp");
                //amplUnitUsed = 0;
            }
            //appendWriteableString();
            lv_label_set_text(labelAmplNum, (char *)writeableString);
            aParamBeingEdited = 0;
            updateLcdValues();
        }

        if((buttonNumber == 8) && (aParamBeingEdited == 3)){//offset
            lv_btnmatrix_set_map(btnm1, btnm_map2);
            lv_btnmatrix_set_btn_width(btnm1, 23, 2);
            lv_label_set_text(labelOffUnit, "Voff");
            //appendWriteableString();
            lv_label_set_text(labelOffNum, (char *)writeableString);
            aParamBeingEdited = 0;
            Generator.SetOffset(charStringToDouble(writeableString));
            Generator.SaveOffsetUnit("V");
            updateLcdValues();
        }

        if((buttonNumber == 22) && (aParamBeingEdited == 3)){ //+- button
            char tempChar[10] = "";
            strlcpy(tempChar, writeableString, 10);

            if(writeableString[0] == '-'){
                for(int i = 0; i < MAX_CHAR_COUNT-1; i++){
                    writeableString[i] = tempChar[i+1];
                }
                writeableString[19] ='\0';
            }
            else{
                writeableString[0] = '-';
                for (int i = 1; i < MAX_CHAR_COUNT-1; i++){
                    writeableString[i] = tempChar[i-1];
                }
                writeableString[19] = '\0';
            }
            lv_label_set_text(labelOffNum, (char *)writeableString);
        }

        if(buttonNumber == 23){ // Output button

            if(Generator.GetOutputRelay()){
                //outputRelayIsTurnedOn = false;
                lv_led_off(ledOutput);
                Generator.SetOutputRelay(false);
            }
            else{
                //outputRelayIsTurnedOn = true;
                lv_led_on(ledOutput);
                Generator.SetOutputRelay(true);
            }
        }

        if (buttonNumber == 0){ //sine
            Generator.SetWaveform(0);
            lv_label_set_text(labelWaveForm, LABEL_SINE);
            lv_btnmatrix_set_map(btnm1, btnm_map2);
            lv_btnmatrix_set_btn_width(btnm1, 23, 2);
            updateLcdValues();
        } 

        if (buttonNumber == 1){ //triangle
            Generator.SetWaveform(1);
            lv_label_set_text(labelWaveForm, LABEL_TRI);
            lv_btnmatrix_set_map(btnm1, btnm_map2);
            lv_btnmatrix_set_btn_width(btnm1, 23, 2);
            updateLcdValues();
        }

        if (buttonNumber == 2){ //square
            Generator.SetWaveform(2);
            lv_label_set_text(labelWaveForm, LABEL_SQ);
            lv_btnmatrix_set_map(btnm1, btnm_map2);
            lv_btnmatrix_set_btn_width(btnm1, 23, 2);
            updateLcdValues();
        }

        if (buttonNumber == 3){ //pwmout
            Generator.SetWaveform(4);
            lv_label_set_text(labelWaveForm, LABEL_PWM);
            lv_btnmatrix_set_map(btnm1, btnm_map_pwm);
            lv_btnmatrix_set_btn_width(btnm1, 23, 2);
            updateLcdValues();
        }

    }
}


void interface_init(){
    //******************************LVGL STUFF******************************
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(COLOR_BACKGROUND);

  lv_init(); //might need to put before eSPI init.

  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  /*Initialize the display*/
  
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 480;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the input device driver*/
  
  lv_indev_drv_init(&indev_drv);             /*Descriptor of a input device driver*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;    /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = my_input_read;      /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);         /*Finally register the driver*/

  //lv_obj_t *screenMain = lv_obj_create(NULL, NULL);
  screenMain = lv_obj_create(NULL, NULL);

    static lv_style_t style_nored;
    lv_style_init(&style_nored);
    //lv_style_set_bg_color(&style_nored, LV_STATE_PRESSED, LV_COLOR_BLUE);
    lv_style_set_border_color(&style_nored, LV_STATE_FOCUSED, lv_color_hex(0xd6dde3));
  
  lv_style_init(&bigStyle);
  lv_style_set_text_font(&bigStyle ,LV_STATE_DEFAULT, &lv_font_montserrat_36);

  
  lv_style_init(&mediumStyle);
  lv_style_set_text_font(&mediumStyle ,LV_STATE_DEFAULT, &lv_font_montserrat_26);
  //lv_style_set_bg_opa(&mediumStyle, LV_STATE_DEFAULT, LV_OPA_COVER);
  //lv_style_set_bg_color(&mediumStyle, LV_STATE_DEFAULT, LV_COLOR_CYAN);

  labelFreqNum = lv_label_create(screenMain, NULL);
  lv_obj_add_style(labelFreqNum, LV_LABEL_PART_MAIN, &bigStyle);
  lv_label_set_long_mode(labelFreqNum, LV_LABEL_LONG_BREAK);
  lv_label_set_text(labelFreqNum, "0.000");
  lv_label_set_align(labelFreqNum, LV_LABEL_ALIGN_RIGHT);
  lv_obj_set_size(labelFreqNum, 170, 40);
  lv_obj_set_pos(labelFreqNum, 60, 15);

  labelFreqUnit = lv_label_create(screenMain, NULL);
  lv_obj_add_style(labelFreqUnit, LV_LABEL_PART_MAIN, &mediumStyle);
  lv_label_set_long_mode(labelFreqUnit, LV_LABEL_LONG_BREAK);
  lv_label_set_text(labelFreqUnit, "kHz");
  lv_label_set_align(labelFreqUnit, LV_LABEL_ALIGN_LEFT);
  lv_obj_set_size(labelFreqUnit, 89, 40);
  lv_obj_set_pos(labelFreqUnit, 245, 22);

  labelAmplNum = lv_label_create(screenMain, NULL);
  lv_obj_add_style(labelAmplNum, LV_LABEL_PART_MAIN, &bigStyle);
  lv_label_set_long_mode(labelAmplNum, LV_LABEL_LONG_BREAK);
  lv_label_set_text(labelAmplNum, "0.00");
  lv_label_set_align(labelAmplNum, LV_LABEL_ALIGN_RIGHT);
  lv_obj_set_size(labelAmplNum, 170, 40);
  lv_obj_set_pos(labelAmplNum, 60, 70);

  labelAmplUnit = lv_label_create(screenMain, NULL);
  lv_obj_add_style(labelAmplUnit, LV_LABEL_PART_MAIN, &mediumStyle);
  lv_label_set_long_mode(labelAmplUnit, LV_LABEL_LONG_BREAK);
  lv_label_set_text(labelAmplUnit, "Vpp");
  lv_label_set_align(labelAmplUnit, LV_LABEL_ALIGN_LEFT);
  lv_obj_set_size(labelAmplUnit, 89, 40);
  lv_obj_set_pos(labelAmplUnit, 245, 77);

  labelOffNum = lv_label_create(screenMain, NULL);
  lv_obj_add_style(labelOffNum, LV_LABEL_PART_MAIN, &bigStyle);
  lv_label_set_long_mode(labelOffNum, LV_LABEL_LONG_BREAK);
  lv_label_set_text(labelOffNum, "0.00");
  lv_label_set_align(labelOffNum, LV_LABEL_ALIGN_RIGHT);
  lv_obj_set_size(labelOffNum, 170, 40);
  lv_obj_set_pos(labelOffNum, 60, 125);

  labelOffUnit = lv_label_create(screenMain, NULL);
  lv_obj_add_style(labelOffUnit, LV_LABEL_PART_MAIN, &mediumStyle);
  lv_label_set_long_mode(labelOffUnit, LV_LABEL_LONG_BREAK);
  lv_label_set_text(labelOffUnit, "Voff");
  lv_label_set_align(labelOffUnit, LV_LABEL_ALIGN_LEFT);
  lv_obj_set_size(labelOffUnit, 89, 40);
  lv_obj_set_pos(labelOffUnit, 245, 132);

  lv_style_init(&styleBattLow);
  lv_style_set_text_color(&styleBattLow, LV_STATE_DEFAULT, lv_color_hex(0xe00000));

  lv_style_init(&styleBattOk);
  lv_style_set_text_color(&styleBattOk, LV_STATE_DEFAULT, LV_COLOR_BLACK);

  labelBattery = lv_label_create(screenMain, NULL);
  lv_label_set_text(labelBattery, LV_SYMBOL_BATTERY_FULL);
  lv_obj_set_pos(labelBattery, 10, 15);
  lv_obj_add_style(labelBattery, LV_LABEL_PART_MAIN, &styleBattOk);

  labelWiFi = lv_label_create(screenMain, NULL);
  lv_label_set_text(labelWiFi, LV_SYMBOL_WIFI);
  lv_obj_set_pos(labelWiFi, 10, 35);

  labelWiFiIP = lv_label_create(screenMain, NULL);
  lv_label_set_text(labelWiFiIP, Device.GetMyIP().c_str());
  lv_obj_set_pos(labelWiFiIP, 10, 70);

  lv_style_init(&style_led);
  lv_style_set_bg_color(&style_led, LV_STATE_DEFAULT, lv_color_hex(0x00aa00));
  lv_style_set_border_color(&style_led, LV_STATE_DEFAULT, lv_color_hex(0x00aa00));
  lv_style_set_shadow_color(&style_led, LV_STATE_DEFAULT, lv_color_hex(0x00aa00));
  lv_style_set_shadow_spread(&style_led, LV_STATE_DEFAULT, 4);

  ledOutput = lv_led_create(screenMain, NULL);
  lv_obj_add_style(ledOutput, LV_LED_PART_MAIN, &style_led);
  lv_obj_set_pos(ledOutput, 10, 149);
  lv_obj_set_size(ledOutput, 10, 10);
  lv_led_off(ledOutput);

  labelOutput = lv_label_create(screenMain, NULL);
  lv_label_set_text(labelOutput, "OUT");
  lv_obj_set_pos(labelOutput, 28, 145);

  labelWaveForm = lv_label_create(screenMain, NULL);
  lv_label_set_text(labelWaveForm, "SINE");
  lv_obj_set_pos(labelWaveForm, 10, 110);

  labelLeftChannel = lv_label_create(screenMain, NULL);
  lv_label_set_text(labelLeftChannel, " ");
  lv_obj_set_pos(labelLeftChannel, 70, 0);

  labelRightChannel = lv_label_create(screenMain, NULL);
  lv_label_set_text(labelRightChannel, LV_SYMBOL_UP);
  lv_obj_set_pos(labelRightChannel, 230, 0);

  btnm1 = lv_btnmatrix_create(screenMain, NULL);
    lv_btnmatrix_set_map(btnm1, btnm_map);
    
  lv_btnmatrix_set_map(btnm1, btnm_map2);
    lv_btnmatrix_set_recolor(btnm1, true);
    lv_btnmatrix_set_btn_width(btnm1, 23, 2);        /*Make "Action1" twice as wide as "Action2"*/
    lv_obj_set_height(btnm1, 300);
    lv_obj_set_width(btnm1, 320);
    lv_obj_align(btnm1, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_obj_set_event_cb(btnm1, btn_mtx_event_handler);
    lv_obj_add_style(btnm1, LV_BTNMATRIX_PART_BG, &style_nored);

  lv_scr_load(screenMain);
  lv_task_handler();
  /***************************Second screen *******************************************************/

  screenSettings = lv_obj_create(NULL, NULL);

    page = lv_page_create(screenSettings, NULL);
    lv_obj_set_size(page, 320, 480);
    //lv_obj_align(page, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(page, LV_PAGE_PART_BG, &style_nored);


  //delay(5000);
  //lv_scr_load(screenSettings);
  labelSettingName = lv_label_create(page, NULL);
  lv_label_set_text(labelSettingName, "SETTINGS");
  lv_obj_set_pos(labelSettingName, 10, 10);

  labelConnectToWifi = lv_label_create(page, NULL);
  lv_label_set_text(labelConnectToWifi, "Connect to a Wi-Fi");
  lv_obj_set_pos(labelConnectToWifi, 10, 58);

  swConnectToWifi = lv_switch_create(page, NULL);
  lv_obj_set_pos(swConnectToWifi, 240, 52);
  if(Device.IsWiFiOn()){
      lv_switch_on(swConnectToWifi, LV_ANIM_ON);
  }
  else {
      lv_switch_off(swConnectToWifi, LV_ANIM_ON);
  }
  lv_obj_set_event_cb(swConnectToWifi, swConnectWiFiHandler);

  labelWifiSsid = lv_label_create(page, NULL);
  lv_label_set_text(labelWifiSsid, "SSID:");
  lv_obj_set_pos(labelWifiSsid, 50, 98);
  

  taSSID = lv_textarea_create(page, NULL);
    lv_textarea_set_pwd_mode(taSSID, false);
    //lv_obj_align(taSSID, NULL, LV_ALIGN_IN_TOP_RIGHT, 100, 65);
    lv_obj_set_pos(taSSID, 100, 91);
    lv_obj_set_width(taSSID, 140);
    lv_textarea_set_text(taSSID, "");
    lv_textarea_set_one_line( taSSID, true);
    lv_textarea_set_cursor_hidden(taSSID, true);
    lv_obj_set_event_cb(taSSID, ta_event_cb);

    lv_textarea_set_text(taSSID, Device.GetWiFiSSID().c_str());
    

  labelWifiPass = lv_label_create(page, NULL);
  lv_label_set_text(labelWifiPass, "Pass:");
  lv_obj_set_pos(labelWifiPass, 50, 138);

  taWifiPass = lv_textarea_create(page, NULL);
    lv_textarea_set_pwd_mode(taWifiPass, true);
    lv_textarea_set_one_line(taWifiPass, true);
    lv_textarea_set_text(taWifiPass, Device.GetWiFiPass().c_str());
    lv_obj_set_width(taWifiPass, 140);
    lv_obj_set_pos(taWifiPass, 100, 131);
    lv_textarea_set_cursor_hidden(taWifiPass, true);
    lv_obj_set_event_cb(taWifiPass, ta_event_cb);

    

  labelCreateWifi = lv_label_create(page, NULL);
  lv_label_set_text(labelCreateWifi, "Create own Wi-Fi AP");
  lv_obj_set_pos(labelCreateWifi, 10, 190);

  swCreateWifi = lv_switch_create(page, NULL);
  lv_obj_set_pos(swCreateWifi, 240, 184);
  //lv_obj_align(swConnectToWifi, NULL, LV_ALIGN_CENTER, 0, -50);
  lv_obj_set_event_cb(swCreateWifi, swConnectAPHandler);
  

  labelOwnWifiSsid = lv_label_create(page, NULL);
  lv_label_set_text(labelOwnWifiSsid, "SSID:");
  lv_obj_set_pos(labelOwnWifiSsid, 50, 230);
  

  taOwnSSID = lv_textarea_create(page, NULL);
  lv_textarea_set_pwd_mode(taOwnSSID, false);
  //lv_textarea_set_text(taOwnSSID, "");
  lv_textarea_set_text(taOwnSSID, Device.GetApSSID().c_str());
  lv_obj_set_pos(taOwnSSID, 100, 223);
  lv_obj_set_width(taOwnSSID, 140);
  lv_textarea_set_one_line(taOwnSSID, true);
  lv_textarea_set_cursor_hidden(taOwnSSID, true);
  lv_obj_set_event_cb(taOwnSSID, ta_event_cb);

  labelOwnWifiPass = lv_label_create(page, NULL);
  lv_label_set_text(labelOwnWifiPass, "Pass:");
  lv_obj_set_pos(labelOwnWifiPass, 50, 270);

  taOwnWifiPass = lv_textarea_create(page, NULL);
  //lv_textarea_set_text(taOwnWifiPass, "");
  lv_textarea_set_pwd_mode(taOwnWifiPass, true);
  lv_textarea_set_one_line(taOwnWifiPass, true);
  lv_textarea_set_text(taOwnWifiPass, Device.GetApPass().c_str());
  lv_obj_set_width(taOwnWifiPass, 140);
  lv_obj_set_pos(taOwnWifiPass, 100, 263);
  lv_textarea_set_cursor_hidden(taOwnWifiPass, true);
  lv_obj_set_event_cb(taOwnWifiPass, ta_event_cb);


  labelDisplayBrightness = lv_label_create(page, NULL);
  lv_label_set_text(labelDisplayBrightness, "Display Brightness:");
  lv_obj_set_pos(labelDisplayBrightness, 10, 400);

  sliderDispBrightness = lv_slider_create(page, NULL);
  //lv_obj_align(sliderDispBrightness, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_pos(sliderDispBrightness, 30, 440);
  lv_obj_set_width(sliderDispBrightness, 250);
  lv_slider_set_value(sliderDispBrightness, Device.GetBrightness()*100/255, false);
  lv_obj_set_event_cb(sliderDispBrightness, brght_event_handler);

  lv_obj_t * btnExit = lv_btn_create(page, NULL);
    lv_obj_set_event_cb(btnExit, event_handler_exit_btn);
    lv_obj_set_width(btnExit, 44);
    lv_obj_set_height(btnExit, 32);
    lv_obj_set_pos(btnExit, 250, 4);

  lv_obj_t * label = lv_label_create(btnExit, NULL);
    lv_label_set_text(label, "X");

    checkBoxShowPass = lv_checkbox_create(page, NULL);
    lv_checkbox_set_text(checkBoxShowPass, "Show password");
    lv_obj_set_pos(checkBoxShowPass, 100, 310);
    //lv_obj_align(checkBoxShowPass, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(checkBoxShowPass, eventCheckPass);

    updateLcdValues();
}

void interface_update(){
    if (Generator.GetWaveform() == 0){ //sine
        lv_label_set_text(labelWaveForm, LABEL_SINE);
        lv_btnmatrix_set_map(btnm1, btnm_map2);
        lv_btnmatrix_set_btn_width(btnm1, 23, 2);
    } 

    if (Generator.GetWaveform() == 1){ //triangle
        lv_label_set_text(labelWaveForm, LABEL_TRI);
        lv_btnmatrix_set_map(btnm1, btnm_map2);
        lv_btnmatrix_set_btn_width(btnm1, 23, 2);
        
    }

    if (Generator.GetWaveform() == 2){ //square
        lv_label_set_text(labelWaveForm, LABEL_SQ);
        lv_btnmatrix_set_map(btnm1, btnm_map2);
        lv_btnmatrix_set_btn_width(btnm1, 23, 2);
    }

    if (Generator.GetWaveform() == 4){ //pwmout
        lv_label_set_text(labelWaveForm, LABEL_PWM);
        lv_btnmatrix_set_map(btnm1, btnm_map_pwm);
        lv_btnmatrix_set_btn_width(btnm1, 23, 2);
    }
    updateLcdValues();
}

void interface_update_output(){
    if(Generator.GetOutputRelay() == true) lv_led_on(ledOutput);
    else lv_led_off(ledOutput);
}

void interface_update_battery(){
    int batt = Device.GetBatteryValue();

    if(batt >= 90){
        lv_label_set_text(labelBattery, LV_SYMBOL_BATTERY_FULL);
        lv_obj_add_style(labelBattery, LV_LABEL_PART_MAIN, &styleBattOk);
    }
    else if (batt >= 60) {
        lv_label_set_text(labelBattery, LV_SYMBOL_BATTERY_3);
        lv_obj_add_style(labelBattery, LV_LABEL_PART_MAIN, &styleBattOk);
    }
    else if (batt >= 40 ) {
        lv_label_set_text(labelBattery, LV_SYMBOL_BATTERY_2);
        lv_obj_add_style(labelBattery, LV_LABEL_PART_MAIN, &styleBattOk);
    }
    else if (batt >= 16) {
        lv_label_set_text(labelBattery, LV_SYMBOL_BATTERY_1);
        lv_obj_add_style(labelBattery, LV_LABEL_PART_MAIN, &styleBattOk);
    }
    else {
        lv_label_set_text(labelBattery, LV_SYMBOL_BATTERY_EMPTY);
        lv_obj_add_style(labelBattery, LV_LABEL_PART_MAIN, &styleBattLow);
    }

}