/**
 *  Library for BQ24295 by Texas Instruments
 *  Used with an ESP32 with Arduino I2C library
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 *  daumemo.com
 * */

#ifndef BQ24295_H
#define BQ24295_H

#include <Arduino.h>

class BQ24295_IC {
    private:
        
    public:
    /**
     * class initialisation
     * sets up I2C connection to the IC 
     * 
     * @param i2c_sda pin number for SDA
     * @param i2c_sck pin number for SCK
     * @param clockSpeed I2C clock speed in Hz
     */
    BQ24295_IC(int i2c_sda, int i2c_sck, int clockSpeed);
    BQ24295_IC(void);

    /**
     * Reads a particular register from the IC
     * 
     * @param registerToRead register number to read (from 0x00 to 0x0A)
     * @return register value
     */
    uint8_t getRegister(uint8_t registerToRead);

    /**
     * Writes a 8 bit value to the particular register
     * 
     * @param valueToWrite value which needs to be written to a register
     * @param registerToWrite register number to which a value should be written
     */
    void setRegister(uint8_t valueToWrite, uint8_t registerToWrite);

    /**
     * Reads part number of the IC
     * 
     * @return string 'bq24295' or 'unknown'
     */
    String getPartNumber();

    /**
     * Reads IC's revision number
     * 
     * @return revision number
     */
    uint8_t getRevisionNumber();

    /**
     * Reads fault register and combines all faults to one human readable string
     * 
     * @return string of all faults aggregated together or 'No faults'
     */
    String getFaults();

    /**
     * Reads Ic's status and combines to a singular string
     * 
     * @return string with all statuses listed
     */
    String getStatus();

    /**
     * Reads specific status bit(s) from the status register
     * 
     * @param bitIndex value from 0 to 5 
     */
    String getStatus(uint8_t bitIndex);

    /**
     * Forces input source detection
     */
    void forceInputSourceDetect();

    /**
     * Safety Timer Setting during Input DPM and Thermal Regulation
     * 
     * @param value if true timer is slowed down by 2. Default: true
     */
    void setSlowSafetyTimer(bool value);

    /**
     * Enables or disables battery FET. When it is OFF, battery is cut off from the circuit and no charge or discharge are possible.
     * 
     * @param value if true, FET is disabled. Default: FET is enabled (false)
     */
    void disableBatfet(bool value);

    /**
     * Enables or disables some interrupts. Note, that some interrupts are always enabled.
     * For more information, please refer to the datasheet.
     * 
     * @param chrg_fault if true, interrupt on charge fault is enabled. Default: enabled (true)
     * @param bat_fault if true, interrupt on battery fault is enabled. Default: enabled (true)
     */
    void setInterrupts(bool chrg_fault, bool bat_fault);

    /**
     * Sets boost voltage. 4bit value. Offset is 4.55V. 
     * Bit values from LSB to MSB repectively are: 64mV, 128mV, 256mV, 512mV.
     * The boost voltage range is [4.55 ... 5.51] V.
     * 
     * @param value 4bit value in the range [0..0xF]. Default: 5.125 V (0b1001)
     */
    void setBoostVoltage(uint8_t value);

    /**
     * Set Boost Mode (BHOT) temperature monitor threshold voltage to disable boost mode.
     * Please refer to the datasheet for more information.
     * 
     * @param value 2bit value [0 .. 3]. Default: Vbhot0 1 (0)
     */
    void setBHot(uint8_t value);

    /**
     * Sets thermal regulation threshold.
     * Possible values:
     * 0b00 - 60 degrees celcius
     * 0b01 - 80 degrees celcius
     * 0b10 - 100 degrees celcius
     * 0b11 - 120 degrees celcius
     * 
     * @param value 2bit value [0 ... 3]. Default: 120 degrees (0b11)
     */
    void setThermalRegulationThreshold(uint8_t value);

    /**
     * Enables or disables charge termination.
     * The device terminates a charge cycle when the battery voltage is above recharge threshold,
     * and the current is below termination current.
     * 
     * @param value if true, charge termination is enabled. Default: enabled (true)
     */
    void enableChargeTermination(bool value);

    /**
     * Sets I2C watchdog timer which changes IC's working mode from Host to Default
     * 
     * @param value 2 bit watchdog timer, possible values: 0 - timer is disabled, 1 - 40s, 2 - 80s, 3 - 160s. Default: 40s (1)
     */
    void setI2cWatchdogTimer(uint8_t value);

    /**
     * Enables or disables fast charge safety timer.
     * 
     * @param value if true, fast charge safety timer is enabled. Default: enabled (true)
     */
    void enableSafetyChargingTimer(bool value);

    /**
     * Sets fast charge safety timer 
     * 
     * @param value 2 bit timer value: 0 - 5 hrs, 1 - 8 hrs, 2 - 12 hrs, 3 - 20 hrs. Default: 12 Hrs (2)
     */
    void setFastChargeTimer(uint8_t value);

    /**
     * Sets charge voltage limit. Offset 3.504 V. Range: 3.504 - 4.4 V.
     * Bit 5 - 512 mV
     * Bit 4 - 256 mV
     * Bit 3 - 128 mV
     * Bit 2 - 64 mV
     * Bit 1 - 32 mV
     * Bit 0 - 16 mV
     * 
     * @param value 6 bit voltage value. Default: 4.208 V (0b101100)
     */
    void setChargeVoltageLimit(uint8_t value);

    /**
     * Sets low battery voltage. Used as threshold between pre-charge and fast charge.
     * 
     * @param value 1 bit value. 0 - 2.8V, 1 - 3.0V. Default 3.0 V (1)
     */
    void setBatteryLowVoltage(uint8_t value);

    /**
     * Set battery recharge termination voltage.
     * The device terminates a charge cycle when the battery voltage is above recharge threshold, 
     * and the current is below termination current.
     * 
     * @param value 1 bit, 0 - 100 mV, 1 - 300 mV. Default: 100 mV (0)
     */
    void setBattRechargeThreshold(uint8_t value);

    /**
     * Sets pre-charge current. Offset: 128 mA. Range: 128 - 2048 mA.
     * Bit 3 - 1024 mA
     * Bit 2 - 512 mA
     * Bit 1 - 256 mA
     * Bit 0 - 128 mA
     * 
     * @param value 4 bit cuurent value. Default: 256 mA (0b0001)
     */
    void setPreChargeCurrent(uint8_t value);

    /**
     * Sets termination current limit. Offset: 128 mA. Range: 128 - 2048 mA.
     * Bit 3 - 1024 mA
     * Bit 2 - 512 mA
     * Bit 1 - 256 mA
     * Bit 0 - 128 mA
     * 
     * @param value 4 bit cuurent value. Default: 256 mA (0b0001)
     */
    void setTerminationCurrent(uint8_t value);

    /**
     * Sets fast charge current limit. Offset: 512 mA. Range: 512 - 3008 mA.
     * Bit 5 - 2048 mA
     * Bit 4 - 1024 mA
     * Bit 3 - 512 mA
     * Bit 2 - 256 mA
     * Bit 1 - 128 mA
     * Bit 0 - 64 mA
     * 
     * @param value 6 bit current value. Default: 1024 mA (0b001000)
     */
    void setFastChargeCurrent(uint8_t value);

    /**
     * Sets battery cold threshold. Please refer to the datasheet.
     * Vbcold0 - -10 degrees celcius with 103AT thermistor
     * Vbcold1 - -20 degrees celcius with 103AT thermistor
     * 
     * @param value 1bit, 0 - Vbcold0, 1 - Vbcold1. Default: Vbcold0 (0)
     */
    void setBCold(uint8_t value);

    /**
     * Forces slower charge and precharge currents.
     * 
     * When 0 - Fast charge and pre-cahrge currents are as set in registers.
     * When 1 - Fast charge is 20 and pre-charge is 50 percent as set in registers.
     * 
     * @param value 1 bit. Default: 0. 
     */
    void force20pctCharge(uint8_t value);

    /**
     * Resets IC's registers to default values. Does not put IC to default mode.
     */
    void resetRegisters();

    /**
     * Resets I2C watchdog timer.
     */
    void resetI2cTimer();

    /**
     * Enables or disables OTG.
     * 
     * @param value if true - enables OTG. Default: enabled (true)
     */
    void enableOTG(bool value);

    /**
     * Enables or disables battery charging.
     * 
     * @param value if true - charging is enabled. Default: enabled (true)
     */
    void enableCharging(uint8_t value);

    /**
     * Sets minimum system voltage limit. Offset: 3.0 V. Range: 3.0 - 3.7 V.
     * Bit 2 - 0.4 V
     * Bit 1 - 0.2 V
     * Bit 0 - 0.1 V
     * 
     * @param value 3 bit value for system voltage. Default: 3.5V (0b101)
     */
    void setMinimumSystemVoltage(uint8_t value);

    /**
     * Enables or disables low power (HIZ) mode. In this mode system is powered from the battery.
     * For more information please refer to the datasheet.
     * 
     * @param value if true, HIZ is enabled. Default: disabled (false)
     */
    void enableLowPowerMode(bool value);

    /**
     * Sets input voltage limit. If input voltage goes below this limit,
     * charging current is automatically lowered.
     * Offset: 3.88 V. Range: 3.88 - 5.08 V
     * Bit 3 - 640 mV
     * Bit 2 - 320 mV
     * Bit 1 - 160 mV
     * Bit 0 - 80 mV
     * 
     * @param value 4 bit voltage value. Default: 4.76 V (0b1011)
     */
    void setInputVoltageLimit(uint8_t value);

    /**
     * Sets maximum allowed input current. 
     * 0b000 - 100mA, 0b001 - 150mA, 0b010 - 500mA,
     * 0b011 - 900mA, 0b100 - 1A, 0b101 - 1.5A, 
     * 0b110 - 2A, 0b111 - 3A.
     * 
     * Default _SDP: 500 mA (010)
     * Default DCP/_CDP: 3 A (101)
     * Default Divider 1 and 2: 2 A (110)
     * Default Divider 3: 1 A (100)
     * 
     * @param value 3bit input current setting. 
     */
    void setInputCurrentLimit(uint8_t value);

};

#endif