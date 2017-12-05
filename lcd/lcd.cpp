
#include "lcd.h"

char buf[128]; 

void LCD::updateLCD() 
{
    sendCommand(COLUMN); 
    sendCommand(0x00); 
    sendCommand(0x7F); 

    sendCommand(PAGE); 
    sendCommand(0x00); 
    sendCommand(0x07); 

    for (byte y = 0; y < 8; y++){
        for (byte x = 0; x < 16; x++){

            Wire.beginTransmission(LCD_ADDR); 
            Wire.write(DRAW); 

            for (byte col = 0; col < 8; col++){
                Wire.write(getCharCol(buf[(y*16)+x], col));
            }
            
            Wire.endTransmission(); 
        }
    }
}

void LCD::updateRow(byte row, char *str) 
{
    if (row < 0 || row > 8) return;
    
    // Copy string to the buffer
    byte offset = 16*row; 
    byte i; 
    for (i = 0; i < 16; i++) {
        if (str[i] == '\0') break;

        buf[offset + i] = str[i]; 
    }

    // Clear out the rest of the row
    while (i <16) {
        buf[offset + i] = ' '; 
        i++; 
    }
    
    sendCommand(COLUMN); 
    sendCommand(0x00); 
    sendCommand(0x7F); 

    sendCommand(PAGE); 
    sendCommand(row); 
    sendCommand(row); 
    
    for (byte x = 0; x < 16; x++){

        Wire.beginTransmission(LCD_ADDR); 
        Wire.write(DRAW); 

        for (byte col = 0; col < 8; col++){
            Wire.write(getCharCol(buf[offset + x], col));
        }

        Wire.endTransmission(); 
    }
}

void LCD::clearLCD() 
{
    for (byte i = 0; i < 128; i++) {
        buf[i] = ' '; 
    }

    updateLCD(); 
}

void LCD::initLCD() 
{
    // Turn off LCD 
    sendCommand(DEVICE_OFF); 

    sendCommand(DISPLAY_CLOCK_DIV, 0xF0);
    sendCommand(MULTIPLEX, 0x3F); 
    sendCommand(DISPLAY_OFFSET, 0x08); 

    sendCommand(START_LINE); 

    sendCommand(CHARGE_PUMP, 0x14); 
    sendCommand(MEMORY_MODE, 0x00); 
    sendCommand(SEG_REMAP | 0x01); 

    sendCommand(COM_SCAN_DEC); 

    sendCommand(COM_PINS, 0x12); 
    sendCommand(CONTRAST, 0xCF); 
    sendCommand(PRECHARGE, 0x11); 
    sendCommand(COM_DETECT, 0x40); 

    sendCommand(DISPLAY_ALL_ON_RESUME); 
    sendCommand(NORMAL_DISPLAY); 
    sendCommand(DEVICE_ON); 

    delay(1000); 
}

void LCD::sendCommand(byte command)
{
    Wire.beginTransmission(LCD_ADDR); 
    Wire.write(0x00); 
    Wire.write(command); 
    Wire.endTransmission(); 
}

void LCD::sendCommand(byte command, byte value)
{
    Wire.beginTransmission(LCD_ADDR); 
    Wire.write(0x00); 
    Wire.write(command); 
    Wire.write(value); 
    Wire.endTransmission(); 
}

const byte cA[8] = {
    0b00000000,
    0b01110000,
    0b00011100,
    0b00010010,
    0b00010010,
    0b00011100,
    0b01110000,
    0b00000000
};

const byte cB[8] = {
    0b00000000,
    0b01111110,
    0b01001010,
    0b01001010,
    0b01001010,
    0b01001010,
    0b00110100,
    0b00000000
}; 

const byte cC[8] = {
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000
}; 

const byte cD[8] = {
    0b00000000,
    0b01111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
    0b00000000
}; 
const byte cE[8] = {
    0b00000000,
    0b01111110,
    0b01001010,
    0b01001010,
    0b01001010,
    0b01001010,
    0b01001010,
    0b00000000
}; 
const byte cF[8] = {
    0b00000000,
    0b01111110,
    0b00001010,
    0b00001010,
    0b00001010,
    0b00001010,
    0b00001010,
    0b00000000
}; 
const byte cG[8] = {
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01010010,
    0b01010010,
    0b01110010,
    0b00000000
}; 
const byte cH[8] = {
    0b00000000,
    0b01111110,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b01111110,
    0b00000000
}; 
const byte cI[8] = {
    0b00000000,
    0b01000010,
    0b01000010,
    0b01111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000
}; 
const byte cJ[8] = {
    0b00000000,
    0b00110010,
    0b01000010,
    0b01000010,
    0b00111110,
    0b00000010,
    0b00000010,
    0b00000000
}; 
const byte cK[8] = {
    0b00000000,
    0b01111110,
    0b00001000,
    0b00001000,
    0b00010100,
    0b00100010,
    0b01000000,
    0b00000000
}; 
const byte cL[8] = {
    0b00000000,
    0b01111110,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000
}; 
const byte cM[8] = {
    0b00000000,
    0b01111110,
    0b00000100,
    0b00001000,
    0b00001000,
    0b00000100,
    0b01111110,
    0b00000000
}; 
const byte cN[8] = {
    0b00000000,
    0b01111110,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01111110,
    0b00000000
}; 
const byte cO[8] = {
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
    0b00000000
}; 
const byte cP[8] = {
    0b00000000,
    0b01111110,
    0b00010010,
    0b00010010,
    0b00010010,
    0b00001010,
    0b00000100,
    0b00000000
}; 
const byte cQ[8] = {
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01010010,
    0b01100010,
    0b01011100,
    0b00000000
};
const byte cR[8] = {
    0b00000000,
    0b01111110,
    0b00010010,
    0b00010010,
    0b00010010,
    0b00101010,
    0b01000100,
    0b00000000
}; 
const byte cS[8] = {
    0b00000000,
    0b01001100,
    0b01001010,
    0b01001010,
    0b01001010,
    0b01001010,
    0b00110000,
    0b00000000
}; 
const byte cT[8] = {
    0b00000000,
    0b00000010,
    0b00000010,
    0b01111110,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000000
}; 
const byte cU[8] = {
    0b00000000,
    0b00111110,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00111110,
    0b00000000
}; 
const byte cV[8] = {
    0b00000000,
    0b00011110,
    0b00100000,
    0b01000000,
    0b01000000,
    0b00100000,
    0b00011110,
    0b00000000
}; 
const byte cW[8] = {
    0b00000000,
    0b00111110,
    0b01000000,
    0b00110000,
    0b00110000,
    0b01000000,
    0b00111110,
    0b00000000
}; 
const byte cX[8] = {
    0b00000000,
    0b01000010,
    0b00100100,
    0b00011000,
    0b00011000,
    0b00100100,
    0b01000010,
    0b00000000
}; 
const byte cY[8] = {
    0b00000000,
    0b00000110,
    0b00001000,
    0b00001000,
    0b01111000,
    0b00001000,
    0b00000110,
    0b00000000
}; 
const byte cZ[8] = {
    0b00000000,
    0b01100010,
    0b01100010,
    0b01010010,
    0b01001010,
    0b01000110,
    0b01000110,
    0b00000000
}; 
const byte c0[8] = {
    0b00000000,
    0b00111100,
    0b01100010,
    0b01010010,
    0b01001010,
    0b01000110,
    0b00111100,
    0b00000000
}; 
const byte c1[8] = {
    0b00000000,
    0b01000000,
    0b01000100,
    0b01000010,
    0b01111110,
    0b01000000,
    0b01000000,
    0b00000000
}; 
const byte c2[8] = {
    0b00000000,
    0b01000100,
    0b01100010,
    0b01010010,
    0b01010010,
    0b01001010,
    0b01000100,
    0b00000000
}; 
const byte c3[8] = {
    0b00000000,
    0b01001010,
    0b01001010,
    0b01001010,
    0b01001010,
    0b01001010,
    0b00111100,
    0b00000000
}; 
const byte c4[8] = {
    0b00000000,
    0b00011110,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b01111110,
    0b00000000
}; 
const byte c5[8] = {
    0b00000000,
    0b01001110,
    0b01001010,
    0b01001010,
    0b01001010,
    0b01001010,
    0b00110010,
    0b00000000
}; 
const byte c6[8] = {
    0b00000000,
    0b00111100,
    0b01010010,
    0b01001010,
    0b01001010,
    0b01001010,
    0b00110010,
    0b00000000
}; 
const byte c7[8] = {
    0b00000000,
    0b01000010,
    0b00100010,
    0b00010010,
    0b00001010,
    0b00000110,
    0b00000110,
    0b00000000
}; 
const byte c8[8] = {
    0b00000000,
    0b00111100,
    0b01001010,
    0b01001010,
    0b01001010,
    0b01001010,
    0b00111100,
    0b00000000
}; 
const byte c9[8] = {
    0b00000000,
    0b00001100,
    0b00010010,
    0b00010010,
    0b00010010,
    0b00010010,
    0b01111100,
    0b00000000
}; 

// Gets the column for a given character
byte LCD::getCharCol(byte c, byte col){
    switch(c){
        case 'A':
        case 'a':
            return cA[col];
        case 'B':
        case 'b':
            return cB[col]; 
        case 'C':
        case 'c':
            return cC[col]; 
        case 'D':
        case 'd':
            return cD[col]; 
        case 'E':
        case 'e':
            return cE[col]; 
        case 'F':
        case 'f':
            return cF[col]; 
        case 'G':
        case 'g':
            return cG[col]; 
        case 'H':
        case 'h':
            return cH[col]; 
        case 'I':
        case 'i':
            return cI[col]; 
        case 'J':
        case 'j':
            return cJ[col]; 
        case 'K':
        case 'k':
            return cK[col]; 
        case 'L':
        case 'l':
            return cL[col]; 
        case 'M':
        case 'm':
            return cM[col]; 
        case 'N':
        case 'n':
            return cN[col]; 
        case 'O':
        case 'o':
            return cO[col]; 
        case 'P':
        case 'p':
            return cP[col]; 
        case 'Q':
        case 'q':
            return cQ[col]; 
        case 'R':
        case 'r':
            return cR[col]; 
        case 'S':
        case 's':
            return cS[col]; 
        case 'T':
        case 't':
            return cT[col]; 
        case 'U':
        case 'u':
            return cU[col]; 
        case 'V':
        case 'v':
            return cV[col]; 
        case 'W':
        case 'w':
            return cW[col]; 
        case 'X':
        case 'x':
            return cX[col]; 
        case 'Y':
        case 'y':
            return cY[col]; 
        case 'Z':
        case 'z':
            return cZ[col]; 
        case '0':
            return c0[col]; 
        case '1':
            return c1[col]; 
        case '2':
            return c2[col]; 
        case '3':
            return c3[col]; 
        case '4':
            return c4[col]; 
        case '5':
            return c5[col]; 
        case '6':
            return c6[col]; 
        case '7':
            return c7[col]; 
        case '8':
            return c8[col]; 
        case '9':
            return c9[col]; 
        break;
    }
    
    return 0;
}
