#pragma once

#include "Particle.h"

#define LCD_ADDR    0x3C
#define DEVICE_ON   0xAF 
#define DEVICE_OFF  0xAE
#define CONTRAST    0x78
#define MULTIPLEX   0xA8
#define START_LINE  0x40
#define CHARGE_PUMP 0x8D
#define MEMORY_MODE 0x20
#define SEG_REMAP   0xA0

#define COM_SCAN_DEC    0xC8
#define COM_PINS    0xDA
#define PRECHARGE   0xD9
#define COM_DETECT  0xD8
#define DISPLAY_ALL_ON_RESUME   0xA4
#define NORMAL_DISPLAY  0xA6
#define INVERT_DISPLAY  0xA7

#define DISPLAY_CLOCK_DIV   0xD5
#define DISPLAY_OFFSET      0xD3

#define COLUMN  0x21
#define PAGE    0x22

#define DRAW    0x40

namespace LCD
{
    void sendCommand(byte command); 
    void sendCommand(byte command, byte value); 

    void initLCD(); 
    void clearLCD(); 
    void updateLCD(); 
    void updateRow(byte row, char *str); 
    void updateChar(byte x, byte y, char c); 

    byte getCharCol(byte c, byte col); 
};
