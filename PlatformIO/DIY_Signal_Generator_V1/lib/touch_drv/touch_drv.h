#ifndef TOUCH_DRV_H
#define TOUCH_DRV_H

#define TOUCH_I2C_ADD 0x38
#define TOUCH_REG_XL 0x04
#define TOUCH_REG_XH 0x03
#define TOUCH_REG_YL 0x06
#define TOUCH_REG_YH 0x05
#define TOUCH_INT_PIN 35

int getTouchInterruptPinValue();
int readTouchReg(int reg);
int getTouchPointX();
int getTouchPointY();
void touchInit();

#endif