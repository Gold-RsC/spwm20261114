#include "oled.h"
#include <string.h>
#include "font.h"
#include<stdarg.h>
#include<stdio.h>
#define numof(x) (sizeof(x)/sizeof(x[0]))

static uint8_t buffer[OLED_BUFFER_SIZE];

#define OLED_SETDISPLAYON 0xAF
#define OLED_SETDISPLAYOFF 0xAE
#define OLED_MEMORYMODE 0x20
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR 0x22
#define OLED_SETSTARTLINE 0x40
#define OLED_DISPLAYALLON_RESUME 0xA4
#define OLED_DISPLAYALLON 0xA5
#define OLED_NORMALDISPLAY 0xA6
#define OLED_INVERTDISPLAY 0xA7
#define OLED_SETMULTIPLEX 0xA8
#define OLED_SETCONTRAST 0x81
#define OLED_CHARGEPUMP 0x8D
#define OLED_EXTERNALVCC 0x1
#define OLED_SWITCHCAPVCC 0x2
#define OLED_SEGREMAP 0xA0
#define OLED_COMSCANDEC 0xC8
#define OLED_COMSCANINC 0xC0
#define OLED_SETDISPLAYOFFSET 0xD3
#define OLED_SETDISPLAYCLOCKDIV 0xD5
#define OLED_SETPRECHARGE 0xD9
#define OLED_SETCOMPINS 0xDA

void OLED_SendCmd(uint8_t cmd){
    uint8_t data[2] = {0x00, cmd};
    HAL_I2C_Master_Transmit(HI2C_ADDR, OLED_ADDR, data, 2, HAL_MAX_DELAY);
}

void OLED_Init(void){
    OLED_SendCmd(OLED_SETDISPLAYOFF);
    OLED_SendCmd(OLED_SETDISPLAYCLOCKDIV);
    OLED_SendCmd(0x80);
    OLED_SendCmd(OLED_SETMULTIPLEX);
    OLED_SendCmd(0x3F);
    OLED_SendCmd(OLED_SETDISPLAYOFFSET);
    OLED_SendCmd(0x00);
    OLED_SendCmd(OLED_SETSTARTLINE | 0x00);
    OLED_SendCmd(OLED_CHARGEPUMP);
    OLED_SendCmd(0x14);
    OLED_SendCmd(OLED_MEMORYMODE);
    OLED_SendCmd(0x00);
    OLED_SendCmd(OLED_SEGREMAP | 0x01);
    OLED_SendCmd(OLED_COMSCANDEC);
    OLED_SendCmd(OLED_SETCOMPINS);
    OLED_SendCmd(0x12);
    OLED_SendCmd(OLED_SETCONTRAST);
    OLED_SendCmd(0xCF);
    OLED_SendCmd(OLED_SETPRECHARGE);
    OLED_SendCmd(0xF1);
    OLED_SendCmd(OLED_DISPLAYALLON_RESUME);
    OLED_SendCmd(OLED_NORMALDISPLAY);
    OLED_SendCmd(OLED_SETDISPLAYON);
}


void OLED_Clear_all(void){
    memset(buffer, 0x00, OLED_BUFFER_SIZE);
}
void OLED_Clear_px(uint8_t x, uint8_t y, uint8_t width, uint8_t height){
    for (size_t i = x; i < x + width; ++i)
    {
        for (size_t j = y; j < y + height; j++)
        {
            if (i < OLED_WIDTH && j < OLED_HEIGHT)
            {
                buffer[i + j / 8 * OLED_WIDTH] &= ~(1 << (j % 8));
            }
        }
    }
}

void OLED_Update(void){
    for(int page=0;page<OLED_PAGE_NUM;page++){
        OLED_SendCmd(0xB0+page);
        OLED_SendCmd(0x00);
        OLED_SendCmd(0x10);
        static uint8_t page_data[OLED_WIDTH+1]={0x40};
        memcpy(page_data+1,buffer+page*OLED_WIDTH,OLED_WIDTH);
        HAL_I2C_Master_Transmit(HI2C_ADDR, OLED_ADDR, page_data, sizeof(page_data), HAL_MAX_DELAY);
    }
    
}

void OLED_Invert_px(uint8_t x,uint8_t y,uint8_t width,uint8_t height){
    for (size_t i = x; i < x + width; ++i)
    {
        for (size_t j = y; j < y + height; j++)
        {
            if (i < OLED_WIDTH && j < OLED_HEIGHT)
            {
                buffer[i + j / 8 * OLED_WIDTH] ^= (1 << (j % 8));
            }
        }
    }
}

void OLED_Add_Picture_px(const uint8_t *map, uint8_t x, uint8_t y, uint8_t width, uint8_t height){
    for (size_t i = x; i < x + width; ++i)
    {
        for (size_t j = y; j < y + height; j++)
        {
            if (i < OLED_WIDTH && j < OLED_HEIGHT)
            {
                buffer[i + j / 8 * OLED_WIDTH] |= map[(i - x) + (j - y) / 8 * width];
            }
        }
    }
}


void OLED_Add_Rectangle_px(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color){
    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            if (i >= 0 && i < OLED_WIDTH && j >= 0 && j < OLED_HEIGHT)
            {
                if (color)
                {
                    buffer[i + (j / 8) * OLED_WIDTH] |= (1 << (j % 8));
                }
                else
                {
                    buffer[i + (j / 8) * OLED_WIDTH] &= ~(1 << (j % 8));
                }
            }
        }
    }
}


void OLED_Add_Char_16x8_px(uint8_t byte,uint8_t x, uint8_t y)
{
    size_t idx = find_char_idx(byte);
    if (idx < numof(font_char_16x16))
    {
        OLED_Add_Picture_px(font_char_16x16[idx], x, y, 16, 16);
    }
    else {
        OLED_Clear_px(x, y, 16, 8);
    }
}

void OLED_Add_String_16x8_px(const char *str,uint8_t x, uint8_t y)
{
    while (*str)
    {
        OLED_Add_Char_16x8_px(*(str++), x, y);
        x += 8;
    }
}

void OLED_Print_16x8_px(uint8_t x,uint8_t y,const char*format,...){
    char str_buf[OLED_WIDTH/8];
    va_list args;
    va_start(args, format);
	size_t len=vsnprintf(str_buf, OLED_WIDTH/8, format, args);
	va_end(args);

    if(len*8>=OLED_WIDTH){
        str_buf[OLED_WIDTH/8-1]='\0';
    }
    OLED_Add_String_16x8_px(str_buf, x, y);
}
