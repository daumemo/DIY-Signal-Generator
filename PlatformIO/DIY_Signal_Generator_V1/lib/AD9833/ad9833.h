/**
 *  Library for AD9833 by Analog Devices
 *  Used with ESP32 without any SPI library
 *  Only regular IO pins are used and Arduino.h library
 * 
 *  daumemo.com
 * */

#ifndef AD9833_H
#define AD9833_H

#define AD9833_SCK_PIN 1
#define AD9833_FSYNC_PIN 5
#define AD9833_DATA_PIN 3

#include <Arduino.h>

class AD9833_IC {
    private:
    void writeRawData(uint16_t data);
    uint16_t controlRegisterValue;
    uint32_t freqRegRaw[2];
    uint16_t phaseRegRaw[2];

    double freqReg[2];
    double phaseReg[2];

    int pSCK,pFSYNC,pDATA;
    
    public:
    AD9833_IC(int pinSCK, int pinFSYNC, int pinDATA);
    AD9833_IC();

    /**
     * Sets Frequency(Hz) in selected frequency register (0 or 1)
     * 
     * @param hz frquency value in Hz
     * @param reg selected register value, 0 or 1
     */
    uint32_t setFrequency(double hz, uint8_t reg);

    /**
     * Sets phase in a selected phase register (0 or 1)
     * 
     * @param phase phase value in degrees
     * @param reg selected register value, 0 or 1
     * @return exact value which was written to the frquency register
     */
    uint32_t setPhase(double phase, uint8_t reg);

    /**
     * Sets frquency register which value is used for output
     * 
     * @param reg selected register value, 0 or 1
     */
    void setFrequencyRegister(uint8_t reg);

    /**
     * Sets phase register which value is used for output
     * 
     * @param reg selected register value, 0 or 1
     */
    void setPhaseRegister(uint8_t reg);

    /**
     * Sets output signal's waveform
     * 
     * @param waveform 0-sine, 1-triangle, 2-square, 3-square/2
     */
    void setWaveForm(uint8_t waveform);

    /**
     * Start waveform generation
     * 
     * @param waveform 0-sine, 1-triangle, 2-square, 3-square/2
     * @param freqRegist used frequency register, 0 or 1
     * @param phaseRegist used phase register, 0 or 1
     */
    void start(uint8_t waveform, uint8_t freqRegist, uint8_t phaseRegist);

    /**
     * Start waveform generation with predefined values
     */
    void start();

    /**
     * Stops signal generation (puts IC to sleep and reset states)
     */
    void stop();

    /**
     * Enables or disables chip's reset, puts IC to sleep
     * 
     * @param status enables(true) or disables(false) reset
     */
    void reset(bool status);

    /**
     * Writes 16 bit of data directly to the AD9833 chip - it is up to the user to write a correct value
     * 
     * @param data 16bit of RAW data
     */
    void directRegWrite(uint16_t data);

    /**
     * @param reg from which phase register to return the value, 0 or 1
     * @return phase value which was written by the user
     */
    double getPhase(uint8_t reg);

    /**
     * @param reg from which frequency register to return the value, 0 or 1
     * @return frequency value which was written by the user
     */
    double getFrequency(uint8_t reg);

    /**
     * @param reg from which phase register to return the value, 0 or 1
     * @return RAW integer phase value which was written to IC
     */
    uint16_t getPhaseRaw(uint8_t reg);

    /**
     * @param reg from which frequency register to return the value, 0 or 1
     * @return RAW integer frequency value which was written to IC
     */
    uint32_t getFreqRaw(uint8_t reg);

    /**
     * @return RAW integer control register value which was written to IC
     */
    uint16_t getControlRegister();
};


#endif