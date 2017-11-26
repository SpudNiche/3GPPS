// Author: Liz Demin, Nicholas LaJoie, Kent Seneres
// ECE 435 - Final Project
// November 26, 2017 
// Description: Raspberry Pi subscribes to Electron ping-pong data being sent over 3G
// File: pi.ino (Pi side)

#include "Particle.h"

// Define Indicator LEDs
#define GREEN_LED D0
#define RED_LED D1

// Global Variables
int status = 0;         // 0 = No data, 1 = Data received
String p_data = "00";   // Variable that will store the incoming data

// Function Prototypes
void general_init();    // General initializations

// Initializations
void setup()
{
    general_init();
    Particle.subscribe("3GPPS-Electron", dataHandler);  // Subscribe to Electron data
    Particle.variable("3GPPS-Data", p_data);              // Variable is easily displayed across the Particle cloud
}

// While (1)
void loop()
{
    // Update status LEDs
    switch (status) {
        case 0:
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, LOW);
            break;
        case 1:
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, HIGH);
            delay(2000);
            case = 0;
            break;
        default:
            break;
    }

    // TODO
}

// General setup function
void general_init()
{
    // Green LED
    pinMode(GREEN_LED, OUTPUT);
    digitalWrite(GREEN_LED, LOW);

    // Red LED
    pinMode(RED_LED, OUTPUT);
    digitalWrite(RED_LED, HIGH);

    status = 0; // Status assumes data is not being received
}

// Ping pong data handler
void dataHandler(const String event, const String data)
{
    p_data = data; // Store the incoming data in our local variable
    // TODO: interpret the data and store the winner and loser IDs in variables that will be used to update a database
}
