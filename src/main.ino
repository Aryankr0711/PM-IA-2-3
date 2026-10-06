/**
 * @file main.ino
 * @brief Initial Baseline Prototype - Simple LED Blinker with Pushbutton
 * @note QA Audit: Contains known defects (blocking delay, floating pin, no current limiter, no WDT)
 */

#define PIN_LED 10
#define PIN_BUTTON 2

int mode = 0;

void setup() {
    Serial.begin(9600);
    // BUG #2: Plain INPUT leaves pin floating without pull-up/pull-down resistor!
    pinMode(PIN_BUTTON, INPUT);
    // BUG #3: Direct LED drive without 220 ohm resistor exceeds 40mA GPIO limit!
    pinMode(PIN_LED, OUTPUT);
    Serial.println("Initial Prototype Booted.");
}

void loop() {
    // BUG #2: Direct read without debouncing causes erratic toggling and multi-switching
    if (digitalRead(PIN_BUTTON) == HIGH) {
        mode = (mode + 1) % 3;
        Serial.print("Mode changed to: ");
        Serial.println(mode);
    }

    // BUG #1: Blocking delay() starves CPU and introduces 1000ms latency on button presses!
    if (mode == 0) {
        digitalWrite(PIN_LED, HIGH);
        delay(500); // Blocking delay
        digitalWrite(PIN_LED, LOW);
        delay(500); // Blocking delay
    } else if (mode == 1) {
        digitalWrite(PIN_LED, HIGH);
        delay(100);
        digitalWrite(PIN_LED, LOW);
        delay(100);
    } else {
        digitalWrite(PIN_LED, HIGH);
        delay(1000);
        digitalWrite(PIN_LED, LOW);
        delay(1000);
    }
    // BUG #4: No Watchdog Timer; any electrical transient freezes loop permanently!
}
