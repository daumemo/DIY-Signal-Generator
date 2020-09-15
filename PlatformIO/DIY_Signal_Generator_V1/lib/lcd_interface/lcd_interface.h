#include "../lvgl/src/lvgl.h"
#include "../TFT_eSPI/TFT_eSPI.h"
#include "generator.h"

#ifndef LCD_INTERFACE_H
#define LCD_INTERFACE_H

#define COLOR_BACKGROUND 0x1084
#define COLOR_TOUCH_POINT TFT_BLUE

void interface_init();
void interface_update();
void interface_update_output();
void interface_update_battery();

#endif