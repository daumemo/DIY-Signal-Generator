/**
 *  A library which encapsulates everything, what is needed to control generator's output parameters
 * 
 *  daumemo.com
 * */

#ifndef GENERATOR_LIB_H
#define GENERATOR_LIB_H

#define OUTPUT_RELAY_PIN 2
#define OFFSET_PWM_PIN 13
#define OFFSET_PWM_CHANNEL 1
#define AMPLITUDE_PWM_PIN 14
#define AMPLITUDE_PWM_CHANNEL 2
#define PWMOUT_PWM_PIN 4
#define PWMOUT_PWM_CHANNEL 5

#define MAX_OFFSET_VALUE 10
#define MIN_OFFSET_VALUE -10
#define MAX_AMPLITUDE_VALUE 10
#define MIN_AMPLITUDE_VALUE 0
#define MAX_DUTY_VALUE 100
#define MIN_DUTY_VALUE 0

#define MAX_AD_FREQ_VALUE 25000000
#define MIN_AD_FREQ_VALUE 0.1
#define MAX_PWMOUT_FREQ_VALUE 5000000
#define MIN_PWMOUT_FREQ_VALUE 1

#include <Arduino.h>
#include "ad9833.h"

class Generator_Object {
    private:
    uint8_t waveform;
    double frequency;
    double offsetVoltage;
    double amplitudeVoltage;
    double duty;
    bool relay;
    AD9833_IC AD9833;

    String keepFreqUnit = "";
    String keepAmpUnit = "";
    String keepOffsUnit = "V";
    String keepDutyUnit = "%";

    void convertToAmplitudePwm(double value);
    void convertToOffsetPwm(double value);
    void startPwmOutput();
    void stopPwmOutput();
    void startSqOutput();
    void stopSqOutput();

    public:

    /**
     * class initialisation
     * 
     */
    Generator_Object(void);

    void SetWaveform(uint8_t wavef);
    uint8_t GetWaveform();

    void SetFrequency(double freq);
    double GetFrequency();

    void SetOffset(double voltage);
    double GetOffset();

    void SetAmplitude(double voltage);
    double GetAmplitude();

    void SetDuty(double value);
    double GetDuty();

    void SetOutputRelay(bool status);
    bool GetOutputRelay();

    void SaveFreqUnit(String value);
    String GetFreqUnit();

    void SaveAmplitudeUnit(String value);
    String GetAmplitudeUnit();

    void SaveOffsetUnit(String value);
    String GetOffsetUnit();

    void SaveDutyUnit(String value);
    String GetDutyUnit();
};

extern Generator_Object Generator;

#endif