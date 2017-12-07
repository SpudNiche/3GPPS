// Author: Liz Demin, Nicholas LaJoie, Kent Seneres
// ECE 435 - Final Project
// November 25, 2017
// Description: Particle Electron uses multiple buttons and LEDs to send ping-pong data over 3G to a Raspberry Pi
// File: cool.ino (Electron side)

#include "Particle.h"
#include "lcd.h"

using namespace LCD; 

// Define Push-button Pins
#define LEFT C4  // Push-button to move left
#define RIGHT C5 // Push-button to move right

// Define Indicator LEDs
#define GREEN_LED D7 // Indicates data has been sent
#define RED_LED D6   // Indicates device is waiting for input

#define MAX_PLAYERS 9

#define LCD_WINNER_X 12
#define LCD_WINNER_Y 3
#define LCD_LOSER_X 12
#define LCD_LOSER_Y 4
#define LCD_MARKING_X 14

// Global Variables
bool left_pressing = false;    // Enable button boolean
bool right_pressing = false;   // Encode button boolean

byte winner;           // Hex value ID for winning player
byte loser;            // Hex value ID for losing player
byte current_id; 

byte state = 0;	
enum {
    INPUT_WINNER = 0,
    INPUT_LOSER,
    PUBLISH
};

struct press_data {             // Timing data for button interupt routine
    unsigned int lastPress;
    unsigned int lastRelease;
    unsigned int previousLastPress;
    unsigned int previousLastRelease;
    unsigned int pressLength;
};

// Button press data initialization
struct press_data left = {0,0,0,0,0};
struct press_data right = {0,0,0,0,0};

// Function Prototypes
void general_init();        // General initializations
int leftHandler();       
int rightHandler();     

void showIntro(); 
void updateDisplay(); 

// Initializations
void setup()
{
    // Set everything up
    general_init();

    // Initialize i2c and lcd
    Wire.begin(); 
    initLCD(); 
    clearLCD(); 

    delay(1000);

    showIntro(); 

    // Turn on Red LED to indicate device is waiting for input
    digitalWrite(RED_LED, HIGH);
}

// While (1)
void loop()
{
    // Update the LEDs based on the state
    switch (state) {
        case INPUT_WINNER:
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, LOW);
            break;
        case INPUT_LOSER:
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, LOW);
            break;
        case PUBLISH:
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, HIGH);

            updateRow(7, "Sending data ***");  

            // Send the data!
            Particle.publish("3GPPS-Electron", String(winner) + String(loser), PUBLIC);

            // Reset data values
            current_id = 0; 
            winner = 0;
            loser = 0;

            delay(2000);

            updateDisplay(); 
            updateRow(7, ""); 

            digitalWrite(GREEN_LED, LOW);
            state = INPUT_WINNER;
            break;
        default:
            break;
    }

    // Check for input
    int check_left = leftHandler();
    int check_right = rightHandler();

    // Process LEFT Button Press
    if (left.previousLastRelease != left.lastRelease) { // Button Pressed
        // Process the timing and do some housekeeping
        left.pressLength = left.lastRelease - left.lastPress; // Determine press length
        Serial.println("left Press took " + String(left.pressLength) + " ms"); // Print
        left.previousLastRelease = left.lastRelease; // Update

        // Hold left button for 3 seconds to go to next step
        // Steps: input winner id -> input loser id -> publish data
        if (left.pressLength > 3000) {
            if (state == INPUT_WINNER) {
                winner = current_id; 
                current_id = 0; 

                state = INPUT_LOSER;
            } else if (state  == INPUT_LOSER) {
                loser = current_id; 
                current_id = 0; 

                state = PUBLISH; 
            } 

        } else {
            if (current_id == 0) {
                current_id = MAX_PLAYERS; 
            }
            current_id = current_id - 1; 
            updateDisplay(); 
        }
    }

    // Process RIGHT Button Press
    if (right.previousLastRelease != right.lastRelease) { // Button pressed
        // Process the button press and do some housekeeping
        right.pressLength = right.lastRelease - right.lastPress; 			    // Determine length of press
        Serial.println("right Press took " + String(right.pressLength) + " ms");  // Print how long the press lasted
        right.previousLastRelease = right.lastRelease;                        	// Update the previousLastRelease value

        current_id = (current_id + 1) % MAX_PLAYERS;  
        updateDisplay(); 
    }
}

void updateDisplay() 
{
    switch (state) {
    case INPUT_WINNER: 
        updateChar(LCD_WINNER_X, LCD_WINNER_Y, current_id + '0'); 
        updateChar(LCD_MARKING_X, LCD_WINNER_Y, '*'); 
        updateChar(LCD_MARKING_X, LCD_LOSER_Y, ' '); 
        break;
    case INPUT_LOSER: 
        updateChar(LCD_LOSER_X, LCD_LOSER_Y, current_id + '0'); 
        updateChar(LCD_MARKING_X, LCD_WINNER_Y, ' '); 
        updateChar(LCD_MARKING_X, LCD_LOSER_Y, '*'); 
        break;
    case PUBLISH:
        updateChar(LCD_WINNER_X, LCD_WINNER_Y, current_id + '0'); 
        updateChar(LCD_LOSER_X, LCD_LOSER_Y, current_id + '0'); 
        updateChar(LCD_MARKING_X, LCD_WINNER_Y, '*'); 
        updateChar(LCD_MARKING_X, LCD_LOSER_Y, ' '); 
    }
}

void showIntro()
{
    updateRow(0, "   PING PONG    "); 
    updateRow(1, "   SUPERSTARS   "); 

    updateRow(LCD_WINNER_Y, "Winner ID: "); 
    updateRow(LCD_LOSER_Y, "Loser  ID: "); 
}

// Functions

// General setup function
void general_init()
{
    // Pin Initializations
    pinMode(LEFT, INPUT_PULLUP);
    pinMode(RIGHT, INPUT_PULLUP);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);

    current_id = 0; 

    // Initialize serial communication for debugging
    Serial.begin(9600);
}

// Function to handle left button presses
int leftHandler()
{
    // 0 = not pressing, 1 = still pressing, 2 = just stopped pressing, 3 = just started pressing

    // If button is low, it's being pressed
    if (digitalRead(LEFT) == LOW && left_pressing == false) // 3
    {
        left_pressing = true;         // The button just started being pressed, so let's note that
        left.lastPress = millis();    // Get timestamp of "last press"
        return 3;                       // Exit function
    }

    // If the button is no longer pressed, but "pressing" says it is, we need to change that
    if (digitalRead(LEFT) == HIGH && left_pressing == true) // 2
    {
        left_pressing = false;        // Button is no longer pressed!
        left.lastRelease = millis();  // Mark "last release"
        return 2;                       // Exit function
    }

    // Else 1
    if (left_pressing == true) // 1
    {
        return 1;                       // Hang out while the button is pressed
    }

    // Else 0
    else // 0
    {
        return 0;                       // Otherwise, do nothing
    }
}

// Function to handle right button presses
int rightHandler()
{
    // 0 = not pressing, 1 = still pressing, 2 = just stopped pressing, 3 = just started pressing

    // If button is low, it's being pressed
    if (digitalRead(RIGHT) == LOW && right_pressing == false) // 3
    {
        right_pressing = true;         // The button just started being pressed, so let's note that
        right.lastPress = millis();    // Get timestamp of "last press"
        return 3;                       // Exit function
    }

    // If the button is no longer pressed, but "pressing" says it is, we need to change that
    if (digitalRead(RIGHT) == HIGH && right_pressing == true) // 2
    {
        right_pressing = false;        // Button is no longer pressed!
        right.lastRelease = millis();  // Mark "last release"
        return 2;                       // Exit function
    }

    // Else 1
    if (right_pressing == true) // 1
    {
        return 1;                       // Hang out while the button is pressed
    }

    // Else 0
    else // 0
    {
        return 0;                       // Otherwise, do nothing
    }
}

// Credits:

// PUSH-BUTTON HANDLERS
    // Code credit to "nrobinson2000" from the Particle Forums, we added comments to walk through the code
    // Link: https://community.particle.io/t/best-method-to-trigger-only-one-event-when-a-button-is-pressed/29121/8
