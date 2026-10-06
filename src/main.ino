/**
 * @file main.ino
 * @brief Fix Issue #2 - Added INPUT_PULLUP and 50ms software debounce filter
 */

#include "button_handler.h"

#define PIN_LED 10
#define PIN_BUTTON 2

int mode = 0;
unsigned long lastToggleTime = 0;
bool ledState = LOW;
ButtonHandler button(PIN_BUTTON, 50);

void setup() {
    Serial.begin(115200);
    button.begin(); // Configures INPUT_PULLUP & stable debouncing
    pinMode(PIN_LED, OUTPUT);
    Serial.println("[FIX #2] Debounced active-low button initialized.");
}

void loop() {
    unsigned long currentMillis = millis();

    ButtonEvent evt = button.update();
    if (evt == EVENT_SHORT_PRESS) {
        mode = (mode + 1) % 3;
        Serial.print("Clean Debounced Mode Switch: ");
        Serial.println(mode);
    }

    unsigned long interval = (mode == 0) ? 500 : ((mode == 1) ? 100 : 1000);
    if (currentMillis - lastToggleTime >= interval) {
        lastToggleTime = currentMillis;
        ledState = !ledState;
        digitalWrite(PIN_LED, ledState);
    }
}
