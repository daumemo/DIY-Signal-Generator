#include "generator.h"
#include "calibration.h"

Generator_Object Generator;

Generator_Object::Generator_Object(){
    
    pinMode(OUTPUT_RELAY_PIN, OUTPUT); //relay init;
    digitalWrite(OUTPUT_RELAY_PIN, LOW);

    SetOutputRelay(false);
    //better to use normal public functions:
    ledcSetup(OFFSET_PWM_CHANNEL, 10000, 12);
    ledcAttachPin(OFFSET_PWM_PIN, OFFSET_PWM_CHANNEL);//offset
    ledcWrite(OFFSET_PWM_CHANNEL, 120);

    ledcSetup(AMPLITUDE_PWM_CHANNEL, 10000, 12);
    ledcAttachPin(AMPLITUDE_PWM_PIN, AMPLITUDE_PWM_CHANNEL);//gain
    ledcWrite(AMPLITUDE_PWM_CHANNEL, 150);

    ledcSetup(PWMOUT_PWM_CHANNEL, 2300, 8); //FREQ needs to be as local frequency....


    //AD9833 is already initialised when Generator_Object is created;
    
    SetFrequency(1000); // default value;
    SaveFreqUnit("kHz");
    SetAmplitude(1); //default amplitude;
    SaveAmplitudeUnit("Vpp");
    SetOffset(0); //default offset;
    SetDuty(50); // default duty - 50 percent;
    SetWaveform(0); // default - sinewave;
}

void Generator_Object::startSqOutput(){
    ledcSetup(PWMOUT_PWM_CHANNEL, GetFrequency(), 1);
    ledcAttachPin(PWMOUT_PWM_PIN, PWMOUT_PWM_CHANNEL);
    ledcWrite(PWMOUT_PWM_CHANNEL, 1);
}

void Generator_Object::stopSqOutput(){
    stopPwmOutput();
}

void Generator_Object::startPwmOutput(){
    uint32_t maxNumber = 0;

    if (GetFrequency() <= 18000){
        ledcSetup(PWMOUT_PWM_CHANNEL, GetFrequency(), 12);
        maxNumber = 4095;
    }
    else if(GetFrequency() <= 60000){
        ledcSetup(PWMOUT_PWM_CHANNEL, GetFrequency(), 10);
        maxNumber = 1023;
    }
    else if(GetFrequency() <= 300000){
        ledcSetup(PWMOUT_PWM_CHANNEL, GetFrequency(), 8);
        maxNumber = 255;
    }
    else if(GetFrequency() <= 1200000){
        ledcSetup(PWMOUT_PWM_CHANNEL, GetFrequency(), 6);
        maxNumber = 63;
    }
    else if(GetFrequency() <= 5000000){
        ledcSetup(PWMOUT_PWM_CHANNEL, GetFrequency(), 4);
        maxNumber = 15;
    }
    else {
        frequency = 5000000;
        ledcSetup(PWMOUT_PWM_CHANNEL, GetFrequency(), 4);
        maxNumber = 15;
    }

    ledcAttachPin(PWMOUT_PWM_PIN, PWMOUT_PWM_CHANNEL);
    if(GetDuty() > 100) SetDuty(100);

    ledcWrite(PWMOUT_PWM_CHANNEL, (uint32_t)(maxNumber*GetDuty()/100.0));
}


void Generator_Object::stopPwmOutput(){
    ledcDetachPin(PWMOUT_PWM_PIN);
}

void Generator_Object::convertToAmplitudePwm(double value){
    int32_t dutyToWrite = 0;
    if(waveform == 0 || waveform == 1 || waveform == 4){
        if(value > SINE_TRI_MAX_AMPLITUDE) value = SINE_TRI_MAX_AMPLITUDE;
        dutyToWrite = (int32_t)(SINE_TRI_COEF_A*value + SINE_TRI_COEF_B); // for sine, tri;
    }
    else{
        if(value > SQUARE_MAX_AMPLITUDE) value = SQUARE_MAX_AMPLITUDE;
        dutyToWrite = (int32_t)(SQUARE_COEF_A*value + SQUARE_COEF_B); // for square;
    }
    if(dutyToWrite > 4095) dutyToWrite = 4095;
    if(dutyToWrite < 0) dutyToWrite = 0;
    ledcWrite(AMPLITUDE_PWM_CHANNEL, dutyToWrite);
}

void Generator_Object::convertToOffsetPwm(double value){
    int32_t dutyToWrite = 0;
    if(value > OFFSET_MAX_VOLTAGE) value = OFFSET_MAX_VOLTAGE;
    if(value < OFFSET_MIN_VOLTAGE) value = OFFSET_MIN_VOLTAGE;
    dutyToWrite = (int32_t)(OFFSET_COEF_A*value + OFFSET_COEF_B);
    if(dutyToWrite > 4095) dutyToWrite = 4095;
    if(dutyToWrite < 0) dutyToWrite = 0;
    ledcWrite(OFFSET_PWM_CHANNEL, dutyToWrite);
}

void Generator_Object::SetWaveform(uint8_t wavef){
    if(wavef== 0 || wavef == 1){
        //use AD9833;
        waveform = wavef;
        AD9833.setWaveForm(waveform);
        SetFrequency(frequency);
        AD9833.start();
        SetAmplitude(amplitudeVoltage); //get back useable values, because in PWM mode they might be set to minimum.
        SetOffset(offsetVoltage); //get back useable values, because in PWM mode they might be set to minimum.
        stopPwmOutput();
    }
    else if (wavef == 2){
        waveform = 2;
        AD9833.stop();
        startSqOutput();
        SetAmplitude(amplitudeVoltage); //get back useable values, because in PWM mode they might be set to minimum.
        SetOffset(offsetVoltage); //get back useable values, because in PWM mode they might be set to minimum.
    }
    else {
        //use ESPs PWM;
        waveform = 4;
        AD9833.stop();
        convertToAmplitudePwm(0);
        convertToOffsetPwm(0);
        SetDuty(duty);
        SetFrequency(frequency);
        startPwmOutput();
    }
}

uint8_t Generator_Object::GetWaveform(){
    return waveform;
}

void Generator_Object::SetFrequency(double freq){
    if (waveform!= 4){ // use AD9833
        if (freq > MAX_AD_FREQ_VALUE) freq = MAX_AD_FREQ_VALUE;
        else if (freq < MIN_AD_FREQ_VALUE) freq = MIN_AD_FREQ_VALUE;
        frequency = freq;
        AD9833.setFrequency(frequency, 0);
    }
    else { //use ESP's PWM
        if (freq > MAX_PWMOUT_FREQ_VALUE) freq = MAX_PWMOUT_FREQ_VALUE;
        else if (freq < MIN_PWMOUT_FREQ_VALUE) freq = MIN_PWMOUT_FREQ_VALUE;
        frequency = freq;
        startPwmOutput();
    }
}

double Generator_Object::GetFrequency(){
    return frequency;
}

void Generator_Object::SetOffset(double voltage){
    if (voltage < MIN_OFFSET_VALUE) voltage = MIN_OFFSET_VALUE;
    else if (voltage > MAX_OFFSET_VALUE) voltage = MAX_OFFSET_VALUE;
    offsetVoltage = voltage;
    
    if (waveform!= 4){ // use AD9833
        convertToOffsetPwm(offsetVoltage);
    }
    else { //use ESP's PWM
        convertToOffsetPwm(0);
    }
}

double Generator_Object::GetOffset(){
    return offsetVoltage;
}

void Generator_Object::SetAmplitude(double voltage){
    if (voltage < MIN_AMPLITUDE_VALUE) voltage = MIN_AMPLITUDE_VALUE;
    else if (voltage > MAX_AMPLITUDE_VALUE) voltage = MAX_AMPLITUDE_VALUE;
    amplitudeVoltage = voltage; 

    if (waveform!= 4){ // use AD9833
        
        convertToAmplitudePwm(amplitudeVoltage);
        // set voltage as needed; NEED CALIBRATION TABLE or MATHEMATICAL FUNCTION
    }
    else { //use ESP's PWM
        // set to minimum, but save amplitude to private variable for later use.
        convertToAmplitudePwm(0);
    }
}

double Generator_Object::GetAmplitude(){
    return amplitudeVoltage;
}

void Generator_Object::SetDuty(double value){
    if (value > MAX_DUTY_VALUE) value = MAX_DUTY_VALUE;
    else if (value < MIN_DUTY_VALUE) value = MIN_DUTY_VALUE;
    duty = value; // set value in any case

    if (waveform == 4){
        startPwmOutput();
    }
}

double Generator_Object::GetDuty(){
    return duty;
}

void Generator_Object::SetOutputRelay(bool status){
    relay = status;
    if(relay){
        digitalWrite(OUTPUT_RELAY_PIN, HIGH); //turn on;
    }
    else{
        digitalWrite(OUTPUT_RELAY_PIN, LOW); //turn off;
    }
}

bool Generator_Object::GetOutputRelay(){
    return relay;
}

void Generator_Object::SaveFreqUnit(String value){
    keepFreqUnit = value;
}

String Generator_Object::GetFreqUnit(){
    return keepFreqUnit;
}

void Generator_Object::SaveAmplitudeUnit(String value){
    keepAmpUnit = value;
}

String Generator_Object::GetAmplitudeUnit(){
    return keepAmpUnit;
}

void Generator_Object::SaveOffsetUnit(String value){
    keepOffsUnit = value;
}
    
String Generator_Object::GetOffsetUnit(){
    return keepOffsUnit;
}

void Generator_Object::SaveDutyUnit(String value){
    keepDutyUnit = value;
}

String Generator_Object::GetDutyUnit(){
    return keepDutyUnit;
}