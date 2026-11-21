#ifndef __FONT_H__
#define __FONT_H__
#include<stdint.h>
#include<stddef.h>
extern const uint8_t font_char_16x16[69][32];

size_t find_char_idx(uint8_t byte);

#endif
