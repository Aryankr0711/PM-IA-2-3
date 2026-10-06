/**
 * @file main.ino
 * @brief Fix Issue #1 - Non-blocking timing using millis()
 */

#define PIN_LED 10
#define PIN_BUTTON 2

int mode = 0;
unsigned long lastToggleTime = 0;
bool ledState = LOW;

void setup() {
    Serial.begin(115200);
    pinMode(PIN_BUTTON, INPUT); // Issue #2 still pending
    pinMode(PIN_LED, OUTPUT);
    Serial.println("[FIX #1] Migrated to non-blocking millis() scheduler.");
}

void loop() {
    unsigned long currentMillis = millis();

    // Responsive button sampling (no longer blocked by delay!)
    if (digitalRead(PIN_BUTTON) == HIGH) {
        mode = (mode + 1) % 3;
        Serial.print("Mode changed to: ");
        Serial.println(mode);
    }

    unsigned long interval = (mode == 0) ? 500 : ((mode == 1) ? 100 : 1000);
    if (currentMillis - lastToggleTime >= interval) {
        lastToggleTime = currentMillis;
        ledState = !ledState;
        digitalWrite(PIN_LED, ledState);
    }
}
