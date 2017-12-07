// Author: Nicholas LaJoie
// Test program to get the Electron responding

#include "Particle.h"

#define LED D7
#define BUTTON D0

int digVal;

void setup()
{
    pinMode(LED, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);

    Particle.function("led", ledToggle);
    Particle.variable("digVal", &digVal, INT);

    digitalWrite(LED, LOW);
}

void loop()
{
    digVal = digitalRead(BUTTON);
    delay(100);
}

int ledToggle(String command) {
    if (command == "on") {
        digitalWrite(LED, HIGH);
        return 1;
    }
    else if (command == "off") {
        digitalWrite(LED, LOW);
        return 0;
    }
    else {
        return -1;
    }
}
