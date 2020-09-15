/**************************************************************************
 * This file is used to calibrate generator's offset and amplitude 
 * values.
 * 
 * To set an output value, the generator needs to know how to convert
 * a voltage value being set up to the actual PWM value which is used to
 * output that voltage.
 * 
 * To convert voltage value being set up to the PWM value, the generator
 * uses mathematical line function, which general for is: 
 * PWM = A*Voltage + B;
 * 
 * PWM is a value which needs to be set up to get a Voltage value.
 * Coefficients A and B needs to be worked out. 
 * 
 * How to find out A and B coefficients:
 * Set desired output voltage (for e.g. 2V). Measure real output voltage.
 * Using default coefficients (see definitions below) find out what actual
 * PWM was set to get the measured voltage. Measure a second point 
 * (for e.g 10V) and calculate PWM value. From these two points it is
 * possible to calculate corrected A and B coefficients (there are a lot 
 * of information how to calculate line coefficients from two points on the
 * internet).
 * 
 * In total there are three sets of coefficients:
 * SINE_TRI_COEF... - used for amplitude setting for sine and triangle waves
 * SQUARE_COEF... - used for square wave amplitude calculation
 * OFFSET_COEF... - used for offset calculation (all waveforms)
 * 
 * Offset values should be measured with minimum waveform amplitude.
 * Although generator's interface uses Vpp (peak to peak) to set output 
 * amplitude, for calculation a real apmlitude value is used (Vpp/2).
 * 
 * The generator has physical limits for setting output values. That limit is
 * defined by possible min/max PWM duty values and gain values which are set 
 * for voltage setting op-amps. Interface should not allow to set such values
 * which are outside of the useable values range.
 * 
 * SINE_TRI_MAX_AMPLITUDE - sets a limit for the maximum allowable amplitude
 * for sine and triangle waves (yet again in Vp (Vpp/2), not Vpp)
 * 
 * SQUARE_MAX_AMPLITUDE sets a limit for the maximum allowable amplitude
 * for square wave (yet again in Vp (Vpp/2), not Vpp)
 * 
 * OFFSET_MAX_VOLTAGE and OFFSET_MIN_VOLTAGE setts minimum and maximum
 * allowed voltages for setting output offset.
 * *************************************************************************/

#ifndef OUTPUT_CALIBRATION_H
#define OUTPUT_CALIBRATION_H


#define SINE_TRI_COEF_A 387
#define SINE_TRI_COEF_B 257

#define SINE_TRI_MAX_AMPLITUDE 10

#define SQUARE_COEF_A 621.5
#define SQUARE_COEF_B 246.7

#define SQUARE_MAX_AMPLITUDE 6


#define OFFSET_COEF_A -248
#define OFFSET_COEF_B 2005.0

#define OFFSET_MAX_VOLTAGE 10
#define OFFSET_MIN_VOLTAGE -10

#endif
