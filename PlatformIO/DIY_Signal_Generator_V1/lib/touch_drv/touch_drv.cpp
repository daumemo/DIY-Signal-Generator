#include "touch_drv.h"
#include "Wire.h"

int getTouchInterruptPinValue()
{
  return digitalRead(TOUCH_INT_PIN);
}

int readTouchReg(int reg)
{
  int data = 0;
  Wire.beginTransmission(TOUCH_I2C_ADD);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(TOUCH_I2C_ADD, 1);
  if (Wire.available())
  {
    data = Wire.read();
  }
  return data;
}

int getTouchPointX()
{
  int XL = 0;
  int XH = 0;

  XH = readTouchReg(TOUCH_REG_XH);
  XL = readTouchReg(TOUCH_REG_XL);

  return ((XH & 0x0F) << 8) | XL;
}

int getTouchPointY()
{
  int YL = 0;
  int YH = 0;

  YH = readTouchReg(TOUCH_REG_YH);
  YL = readTouchReg(TOUCH_REG_YL);

  return ((YH & 0x0F) << 8) | YL;
}

void touchInit()
{
  Wire.begin(15, 0);
  Wire.setClock(40000);
  pinMode(35, INPUT);
}
