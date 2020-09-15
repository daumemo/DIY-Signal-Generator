#include <Arduino.h>
#include "ad9833.h"

#define AD_CTRL_B28 13
#define AD_CTRL_HLB 12
#define AD_CTRL_FSELECT 11
#define AD_CTRL_PSELECT 10
#define AD_CTRL_RESET 8
#define AD_CTRL_SLEEP1 7
#define AD_CTRL_SLEEP12 6
#define AD_CTRL_OPBITEN 5
#define AD_CTRL_DIV2 3
#define AD_CTRL_MODE 1

AD9833_IC::AD9833_IC(int pinSCK, int pinFSYNC, int pinDATA){
    pSCK = pinSCK;
    pFSYNC = pinFSYNC;
    pDATA = pinDATA;
    freqReg[0] = 0;
    freqReg[1] = 0;
    phaseReg[0] = 0;
    phaseReg[1] = 0;
    freqRegRaw[0] = 0;
    freqRegRaw[1] = 0;
    phaseRegRaw[0] = 0;
    phaseRegRaw[1] = 0;
    controlRegisterValue = 0;

    pinMode(pSCK, OUTPUT);
    digitalWrite(SCK, HIGH);
    pinMode(pFSYNC, OUTPUT);
    digitalWrite(pFSYNC, HIGH);
    pinMode(pDATA, OUTPUT);
    digitalWrite(pDATA, HIGH);

    stop();
}

AD9833_IC::AD9833_IC(){
    pSCK = AD9833_SCK_PIN;
    pFSYNC = AD9833_FSYNC_PIN;
    pDATA = AD9833_DATA_PIN;
    freqReg[0] = 0;
    freqReg[1] = 0;
    phaseReg[0] = 0;
    phaseReg[1] = 0;
    freqRegRaw[0] = 0;
    freqRegRaw[1] = 0;
    phaseRegRaw[0] = 0;
    phaseRegRaw[1] = 0;
    controlRegisterValue = 0;

    pinMode(pSCK, OUTPUT);
    digitalWrite(SCK, HIGH);
    pinMode(pFSYNC, OUTPUT);
    digitalWrite(pFSYNC, HIGH);
    pinMode(pDATA, OUTPUT);
    digitalWrite(pDATA, HIGH);

    stop();
}

void AD9833_IC::reset(bool status){
    if(status){
        controlRegisterValue |= (1<<AD_CTRL_RESET);
        writeRawData(controlRegisterValue);
    }
    else {
        controlRegisterValue &= ~(1<<AD_CTRL_RESET);
        writeRawData(controlRegisterValue);
    }
}

void AD9833_IC::directRegWrite(uint16_t data){
    writeRawData(data);
}

void AD9833_IC::start(){
    controlRegisterValue &= ~(1<<AD_CTRL_SLEEP1); //disable sleep;
    controlRegisterValue &= ~(1<<AD_CTRL_SLEEP12);
    controlRegisterValue &= ~(1<<AD_CTRL_RESET); //disable reset;
    writeRawData(controlRegisterValue);
}

void AD9833_IC::start(uint8_t waveform, uint8_t freqRegist, uint8_t phaseRegist){
    if(waveform == 0){ //sine
        controlRegisterValue &= ~(1<<AD_CTRL_OPBITEN);
        controlRegisterValue &= ~(1<<AD_CTRL_MODE);
        controlRegisterValue &= ~(1<<AD_CTRL_DIV2);
    } 
    else if (waveform == 1){ //triangle
        controlRegisterValue &= ~(1<<AD_CTRL_OPBITEN);
        controlRegisterValue |= (1<<AD_CTRL_MODE);
        controlRegisterValue &= ~(1<<AD_CTRL_DIV2);
    }
    else if (waveform == 2){ // square
        controlRegisterValue |= (1<<AD_CTRL_OPBITEN);
        controlRegisterValue &= ~(1<<AD_CTRL_MODE);
        controlRegisterValue |= (1<<AD_CTRL_DIV2);
    }
    else { //square/2
        controlRegisterValue |= (1<<AD_CTRL_OPBITEN);
        controlRegisterValue &= ~(1<<AD_CTRL_MODE);
        controlRegisterValue &= ~(1<<AD_CTRL_DIV2);
    }

    if(freqRegist) controlRegisterValue |= (1<<AD_CTRL_FSELECT);
    else controlRegisterValue &= ~(1<<AD_CTRL_FSELECT);

    if(phaseRegist) controlRegisterValue |= (1<<AD_CTRL_PSELECT);
    else controlRegisterValue &= ~(1<<AD_CTRL_PSELECT);
    
    
    controlRegisterValue &= ~(1<<AD_CTRL_SLEEP1); //disable sleep;
    controlRegisterValue &= ~(1<<AD_CTRL_SLEEP12);
    controlRegisterValue &= ~(1<<AD_CTRL_RESET); //disable reset;


    writeRawData(controlRegisterValue);
}

uint32_t AD9833_IC::setFrequency(double hz, uint8_t reg){
    double regVal = (hz/(25000000.0/268435456.0)) + 0.5;
    uint64_t regV = (uint64_t)regVal;
    uint16_t regMSB = (uint16_t)((regV>>14)& 0x3FFF);
    uint16_t regLSB = (uint16_t)(regV & 0x3FFF);

    if(reg){
        regMSB |= 1<<15;
        regLSB |= 1<<15;
    }
    else {
        regMSB |= 1<<14;
        regLSB |= 1<<14;
    }

    writeRawData(controlRegisterValue|(1<<AD_CTRL_B28));
    writeRawData(regLSB);
    writeRawData(regMSB);

    freqRegRaw[reg] = ((((uint32_t)regMSB)<<14) | ((uint32_t)regLSB));
    freqReg[reg] = hz;

    return freqRegRaw[reg];
}

uint32_t AD9833_IC::setPhase(double phase, uint8_t reg){
    uint16_t regV = (uint16_t(phase/(360.0/4096.0))) & 0xFFF;
    if(reg) regV |= (1<<13);
    regV |= 0xC000;
    writeRawData(regV);

    phaseReg[reg] = phase;
    phaseRegRaw[reg] = (uint32_t)regV;
    return phaseRegRaw[reg];
}

void AD9833_IC::setFrequencyRegister(uint8_t reg){
    if(reg) controlRegisterValue |= (1<<AD_CTRL_FSELECT);
    else controlRegisterValue &= ~(1<<AD_CTRL_FSELECT);
    writeRawData(controlRegisterValue);
}

void AD9833_IC::setPhaseRegister(uint8_t reg){
    if(reg) controlRegisterValue |= (1<<AD_CTRL_PSELECT);
    else controlRegisterValue &= ~(1<<AD_CTRL_PSELECT);
    writeRawData(controlRegisterValue);
}

void AD9833_IC::setWaveForm(uint8_t waveform){
    if(waveform == 0){ //sine
        controlRegisterValue &= ~(1<<AD_CTRL_OPBITEN);
        controlRegisterValue &= ~(1<<AD_CTRL_MODE);
        controlRegisterValue &= ~(1<<AD_CTRL_DIV2);
    } 
    else if (waveform == 1){ //triangle
        controlRegisterValue &= ~(1<<AD_CTRL_OPBITEN);
        controlRegisterValue |= (1<<AD_CTRL_MODE);
        controlRegisterValue &= ~(1<<AD_CTRL_DIV2);
    }
    else if (waveform == 2){ // square
        controlRegisterValue |= (1<<AD_CTRL_OPBITEN);
        controlRegisterValue &= ~(1<<AD_CTRL_MODE);
        controlRegisterValue |= (1<<AD_CTRL_DIV2);
    }
    else { //square/2
        controlRegisterValue |= (1<<AD_CTRL_OPBITEN);
        controlRegisterValue &= ~(1<<AD_CTRL_MODE);
        controlRegisterValue &= ~(1<<AD_CTRL_DIV2);
    }
    writeRawData(controlRegisterValue);
}

void AD9833_IC::stop(){
    controlRegisterValue |= (1<<AD_CTRL_SLEEP1);
    controlRegisterValue |= (1<<AD_CTRL_SLEEP12);
    controlRegisterValue |= (1<<AD_CTRL_RESET);
    writeRawData(controlRegisterValue);
}

double AD9833_IC::getPhase(uint8_t reg){
    return phaseReg[reg];
}

double AD9833_IC::getFrequency(uint8_t reg){
    return freqReg[reg];
}

uint16_t AD9833_IC::getPhaseRaw(uint8_t reg){
    return phaseRegRaw[reg];
}

uint32_t AD9833_IC::getFreqRaw(uint8_t reg){
    return freqRegRaw[reg];
}

uint16_t AD9833_IC::getControlRegister(){
    return controlRegisterValue;
}

/** Private functions **/

void AD9833_IC::writeRawData(uint16_t data){
    digitalWrite(pSCK, HIGH);
    digitalWrite(pFSYNC, LOW);
    for(int i = 15; i>=0; i--){
        digitalWrite(pDATA, (data>>i) & 1); 
        digitalWrite(pSCK, LOW);
        digitalWrite(pSCK, HIGH);
    }
    digitalWrite(pFSYNC, HIGH);
}