/**
 * @file PM_IA_2_LED_Controller.ino
 * @brief Complete Standalone Arduino Sketch for PM-IA Activity 2
 * @project Smart Multi-Mode LED Embedded Controller
 * @author Aryan Kumar (https://github.com/Aryankr0711)
 * @repo https://github.com/Aryankr0711/PM-IA-2-3.git
 */

#include "config.h"
#include "button_handler.h"
#include "led_controller.h"
#include <avr/wdt.h>

ButtonHandler modeButton(PIN_MODE_BUTTON, BUTTON_DEBOUNCE_DELAY_MS);
LEDController ledEngine(PIN_STATUS_LED, PIN_EXTERNAL_LED, PIN_HEARTBEAT_LED);

unsigned long loopCounter = 0;
unsigned long lastTelemetryTime = 0;
const unsigned long TELEMETRY_INTERVAL_MS = 2000;

const char* getStateName(SystemState state) {
    switch (state) {
        case STATE_NORMAL_BLINK:   return "NORMAL_BLINK (1.0 Hz)";
        case STATE_FAST_ALERT:     return "FAST_ALERT (5.0 Hz)";
        case STATE_SOS_BEACON:     return "SOS_BEACON (Morse Code)";
        case STATE_BREATHING_PWM:  return "BREATHING_PWM (Sine Fading)";
        default:                   return "UNKNOWN_STATE";
    }
}

void printSystemBanner() {
    Serial.println(F("\n======================================================="));
    Serial.println(F("  SMART FAULT-TOLERANT EMBEDDED LED CONTROLLER         "));
    Serial.println(F("  Project Management Activity 2 - QA & Git Workflow    "));
    Serial.println(F("  Author: Aryan Kumar | Repo: Aryankr0711/PM-IA-2-3    "));
    Serial.println(F("======================================================="));
    Serial.println(F("[INFO] Architecture: Non-Blocking FSM Engine"));
    Serial.println(F("[INFO] Watchdog Timer: Enabled (Timeout: 2000 ms)"));
    Serial.println(F("[INFO] Debounce Filter: 50 ms Active-Low"));
    Serial.println(F("[INFO] GPIO Pinout:"));
    Serial.println(F("       - Status LED:     Pin 13 (Built-in)"));
    Serial.println(F("       - External LED:   Pin 10 (PWM via 220R)"));
    Serial.println(F("       - Mode Button:    Pin 2  (Internal Pull-up)"));
    Serial.println(F("       - Heartbeat LED:  Pin 9  (PWM Pulse)"));
    Serial.println(F("-------------------------------------------------------"));
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial && millis() < 1000);

    printSystemBanner();
    modeButton.begin();
    ledEngine.begin();

    wdt_enable(WDTO_2S);
    Serial.println(F("[BOOT] AVR Watchdog Timer armed (2000ms). System stable."));
    Serial.print(F("[BOOT] Initial State: "));
    Serial.println(getStateName(ledEngine.getState()));
}

void loop() {
    wdt_reset();

    ButtonEvent btnEvent = modeButton.update();
    if (btnEvent == EVENT_SHORT_PRESS) {
        ledEngine.nextState();
        Serial.print(F("[BUTTON] Short Press -> "));
        Serial.println(getStateName(ledEngine.getState()));
    } else if (btnEvent == EVENT_LONG_PRESS) {
        ledEngine.setState(STATE_NORMAL_BLINK);
        Serial.println(F("[BUTTON] Long Press -> Reset to STATE_NORMAL_BLINK"));
    }

    if (Serial.available() > 0) {
        char cmd = Serial.read();
        if (cmd == 'm' || cmd == 'M') {
            ledEngine.nextState();
            Serial.print(F("[CMD] Mode -> "));
            Serial.println(getStateName(ledEngine.getState()));
        } else if (cmd == 'r' || cmd == 'R') {
            ledEngine.setState(STATE_NORMAL_BLINK);
            Serial.println(F("[CMD] Reset"));
        } else if (cmd == 'w' || cmd == 'W') {
            Serial.println(F("[TEST] Simulating lockup for Watchdog test..."));
            while (true) {}
        }
    }

    ledEngine.update();

    loopCounter++;
    unsigned long currentMillis = millis();
    if (currentMillis - lastTelemetryTime >= TELEMETRY_INTERVAL_MS) {
        float loopsPerSec = (float)loopCounter / ((currentMillis - lastTelemetryTime) / 1000.0f);
        Serial.print(F("[TELEMETRY] Frequency: "));
        Serial.print(loopsPerSec, 0);
        Serial.print(F(" Hz | Mode: "));
        Serial.println(getStateName(ledEngine.getState()));

        loopCounter = 0;
        lastTelemetryTime = currentMillis;
    }
}
