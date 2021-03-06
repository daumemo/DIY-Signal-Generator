# DIY-Signal-Generator

![diy-generator-image](https://daumemo.com/wp-content/uploads/2020/09/DIY-signal-generator-finished-front-1.jpg)

A device, used for generating electrical signals. More infromation: https://daumemo.com/finished-diy-portable-wi-fi-function-generator-part-17

Repository includes all files: firmware code, schematics, PCB files.
STL files for the enclosure can be found here: https://www.thingiverse.com/thing:4596954

Device's specification:
- 3.5 inch IPS LCD with capacitive touch panel
- 2200 mAh Li-Po battery
- ESP-32 MCU
- AD9833 for sine and triangle waveform generation (up to 25 MHz)
- Square wave and PWM generated by the MCU (up to 5 MHz)
- The device can be controlled through LCD or WEB interface
- For WEB control, the device can connect to a Wi-Fi or it can create its own Wi-Fi Access Point
- Output signal’s offset range: -8V … +8V
- Output signal’s amplitude range: 0 … 9V (sine, triangle), 0 … 5.5V (square). PWM output: 3V.
- Battery charging time: <2 hours
- Working time (Wi-Fi on, output relay ON, maximum LCD brightness): around 6 hours
- USB Type C port for charging, 2 SMA ports for signal output, 1 ON/OFF switch, LED charging indicator