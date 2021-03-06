// Author: Liz Demin, Nicholas LaJoie, Kent Seneres
// ECE 435 - Final Project
// November 25, 2017
// Description: Particle Electron uses multiple buttons and LEDs to send ping-pong data over 3G to a Raspberry Pi
// File: 3gpps.ino (Electron side)

#include "Particle.h"

// Define Push-button Pins
#define ENABLE D0 // Push-button begins/ends transmissions, separates number input
#define ENCODE D1 // Push-button to encode digits between 0-15

// Define Indicator LEDs
#define GREEN_LED D7 // Indicates data has been sent
#define RED_LED D6   // Indicates device is waiting for input

// Define Number LEDs for Binary Representation [LED3, LED2, LED1, LED0]
#define LED0 D2
#define LED1 D3
#define LED2 D4
#define LED3 D5

// Global Variables
bool enable_pressing = false;   // Enable button boolean
bool encode_pressing = false;   // Encode button boolean
int enable_press = 0;           // Enable button presses
int encode_press = 0;           // Encode button presses
unsigned char winner;           // Hex value ID for winning player
unsigned char loser;            // Hex value ID for losing player
int i;                          // Counting variable
int state = 0;			// Tracks the state of transmission (0 = waiting, 1 = entering, 2 = transmitting)

struct press_data {             // Timing data for button interupt routine
    unsigned int lastPress;
    unsigned int lastRelease;
    unsigned int previousLastPress;
    unsigned int previousLastRelease;
    unsigned int pressLength;
};

// Button press data initialization
struct press_data enable = {0,0,0,0,0};
struct press_data encode = {0,0,0,0,0};

// Function Prototypes
void general_init();        // General initializations
int enableHandler();       // Enable button handler
int encodeHandler();       // Encode button handler
void num_LEDs(int number);  // Updates binary LEDs

// Initializations
void setup()
{
    // Set everything up
    general_init();
    
    // Flash all four leds to indicate startup
    num_LEDs(15);
    delay(1000);
    num_LEDs(0);

    // Turn on Red LED to indicate device is waiting for input
    digitalWrite(RED_LED, HIGH);
}

// While (1)
void loop()
{
    // Update the LEDs based on the state
    switch (state) {
        case 0:
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, LOW);
            break;
        case 1:
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, LOW);
            break;
        case 2:
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, HIGH);
            delay(2000);
            digitalWrite(GREEN_LED, LOW);
            state = 0;
            break;
        default:
            break;
    }

    // Check for input
    int check_enable = enableHandler();
    int check_encode = encodeHandler();

    // Process ENABLE Button Press
    if (enable.previousLastRelease != enable.lastRelease) { // Button Pressed
        // Process the timing and do some housekeeping
        enable.pressLength = enable.lastRelease - enable.lastPress; // Determine press length
        Serial.println("Enable Press took " + String(enable.pressLength) + " ms"); // Print
        enable.previousLastRelease = enable.lastRelease; // Update

        /******************************* PROCESS TIMING *******************************/
        // If the enable button was held for more than 3 seconds, restart the entire process
        if (enable.pressLength > 3000) {
            Serial.println("Submission reset");

            // Reset data values
            winner = 0;
            loser = 0;

            // Reset button press values
            enable_press = 0;
            encode_press = 0;

            // Update the state of the transmission
            state = 0;
        } else {
            // Increment enable press count
            enable_press++;

            // Process the enable press count (1, 2, or 3)
            switch (enable_press) {
                case 1:
                    Serial.println("Entering the winner ID...");
                    state = 1;
                    break;
                case 2:
                    // Store the winner
                    winner = encode_press;
                    Serial.println("Winner ID: " + String(winner));
                    encode_press = 0;
                    num_LEDs(encode_press);
                    break;
                case 3:
                    // Store the loser, then send the data
                    loser = encode_press;
                    Serial.println("Loser ID: " + String(loser));
                    encode_press = 0;
                    num_LEDs(encode_press);
                    
                    // Send the data!
                    Particle.publish("3GPPS-Electron", String(winner) + String(loser), PUBLIC);
                    state = 2;

                     // Reset enable presses and winner/loser data
                    enable_press = 0;
                    break;
                default:
                    break;
            }
        }
    }

    // Process ENCODE Button Press
    if (encode.previousLastRelease != encode.lastRelease) { // Button pressed
        // Process the button press and do some housekeeping
        encode.pressLength = encode.lastRelease - encode.lastPress; 			    // Determine length of press
        Serial.println("Encode Press took " + String(encode.pressLength) + " ms");  // Print how long the press lasted
        encode.previousLastRelease = encode.lastRelease;                        	// Update the previousLastRelease value

        // Update encode press data
        if (encode_press == 15) {
                encode_press = 0;
            } else {
                encode_press++;
            }

        // Update LEDs
        num_LEDs(encode_press);
    }
}

// Functions

// General setup function
void general_init()
{
    // Pin Initializations
    pinMode(ENABLE, INPUT_PULLUP);
    pinMode(ENCODE, INPUT_PULLUP);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(LED0, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);

    // Initialize serial communication for debugging
    Serial.begin(9600);
}

// Function to handle enable button presses
int enableHandler()
{
    // 0 = not pressing, 1 = still pressing, 2 = just stopped pressing, 3 = just started pressing

    // If button is low, it's being pressed
    if (digitalRead(ENABLE) == LOW && enable_pressing == false) // 3
    {
        enable_pressing = true;         // The button just started being pressed, so let's note that
        enable.lastPress = millis();    // Get timestamp of "last press"
        return 3;                       // Exit function
    }

    // If the button is no longer pressed, but "pressing" says it is, we need to change that
    if (digitalRead(ENABLE) == HIGH && enable_pressing == true) // 2
    {
        enable_pressing = false;        // Button is no longer pressed!
        enable.lastRelease = millis();  // Mark "last release"
        return 2;                       // Exit function
    }

    // Else 1
    if (enable_pressing == true) // 1
    {
        return 1;                       // Hang out while the button is pressed
    }

    // Else 0
    else // 0
    {
        return 0;                       // Otherwise, do nothing
    }
}

// Function to handle encode button presses
int encodeHandler()
{
    // 0 = not pressing, 1 = still pressing, 2 = just stopped pressing, 3 = just started pressing

    // If button is low, it's being pressed
    if (digitalRead(ENCODE) == LOW && encode_pressing == false) // 3
    {
        encode_pressing = true;         // The button just started being pressed, so let's note that
        encode.lastPress = millis();    // Get timestamp of "last press"
        return 3;                       // Exit function
    }

    // If the button is no longer pressed, but "pressing" says it is, we need to change that
    if (digitalRead(ENCODE) == HIGH && encode_pressing == true) // 2
    {
        encode_pressing = false;        // Button is no longer pressed!
        encode.lastRelease = millis();  // Mark "last release"
        return 2;                       // Exit function
    }

    // Else 1
    if (encode_pressing == true) // 1
    {
        return 1;                       // Hang out while the button is pressed
    }

    // Else 0
    else // 0
    {
        return 0;                       // Otherwise, do nothing
    }
}

// Display 0-15 digit on four LEDs in binary
void num_LEDs(int number)
{
    // If value can be represented in 4 bits, set it, otherwise clear
    if (number < 16 && number > 0) {
        digitalWrite(LED0, (number & 0b0001) ? HIGH : LOW);
        digitalWrite(LED1, (number & 0b0010) ? HIGH : LOW);
        digitalWrite(LED2, (number & 0b0100) ? HIGH : LOW);
        digitalWrite(LED3, (number & 0b1000) ? HIGH : LOW);
    } else {
        digitalWrite(LED0, LOW);
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
    }
}

// Credits:

// PUSH-BUTTON HANDLERS
    // Code credit to "nrobinson2000" from the Particle Forums, we added comments to walk through the code
    // Link: https://community.particle.io/t/best-method-to-trigger-only-one-event-when-a-button-is-pressed/29121/8
