#ifndef __OLED_H__
#define __OLED_H__

#include "i2c.h"

#define OLED_ADDR 0x78

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define HI2C_ADDR &hi2c1

#define OLED_BUFFER_SIZE (OLED_WIDTH*OLED_HEIGHT/8)
#define OLED_PAGE_NUM (OLED_HEIGHT/8)
void OLED_Init(void);
void OLED_SendCmd(uint8_t cmd);
void OLED_Update(void);

void OLED_Clear_all(void);
void OLED_Clear_px(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

void OLED_Add_Rectangle_px(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
void OLED_Add_Picture_px(const uint8_t *picture, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

void OLED_Add_Char_16x8_px(uint8_t byte,uint8_t x, uint8_t y);
void OLED_Add_String_16x8_px(const char *str,uint8_t x, uint8_t y);

void OLED_Print_16x8_px(uint8_t x,uint8_t y,const char*str,...);



#endif
