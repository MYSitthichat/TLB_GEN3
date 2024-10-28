#include <Arduino.h>

// Limit Switch Pins (using digital pins)
const int limit1 = 11;
const int limit2 = 12;
const int limit3 = 13; 
// Microswitch Pin (using digital pins)
const int microswitch = 18;

void setup()
{
    Serial.begin(9600);
    pinMode(limit1, INPUT_PULLUP); // Enable internal pull-up resistors
    pinMode(limit2, INPUT_PULLUP);
    pinMode(limit3, INPUT_PULLUP);
    pinMode(microswitch, INPUT_PULLUP);
}

void loop()
{
    int limit1State = digitalRead(limit1);
    int limit2State = digitalRead(limit2);
    int limit3State = digitalRead(limit3);
    int microswitchState = digitalRead(microswitch);

    // Print the switch states (0 for closed, 1 for open with pull-up resistors)
    Serial.print("Limit1: ");
    Serial.print(limit1State);
    Serial.print(" Limit2: ");
    Serial.print(limit2State);
    Serial.print(" Limit3: ");
    Serial.print(limit3State);
    Serial.print(" Microswitch: ");
    Serial.println(microswitchState);

    delay(1000);
}
