#include "Particle.h"
#include "lcd.h"

using namespace LCD; 

void setup()
{
    Wire.begin(); 
    initLCD(); 
    clearLCD(); 

    Serial.begin(9600);
}

void loop()
{
    updateRow(1, " ECE 435 IS COOL "); 
    delay(1000); 
}
