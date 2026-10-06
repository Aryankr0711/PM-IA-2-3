# Root Cause Analysis (RCA) - Embedded Firmware & Hardware Quality Assurance

This document outlines the detailed Root Cause Analysis (RCA) conducted for all logged Quality Assurance defects in the **Smart Embedded LED Controller** project. Each defect has been evaluated using both the **5-Why Method** and the **Fishbone (Ishikawa) Framework** to establish preventive measures and permanent corrective actions.

---

## Issue #1: [FIRMWARE-PERF] Blocking `delay()` Starves Microcontroller Loop Execution

- **Issue ID:** `#1`
- **Severity:** High
- **Priority:** P1 (Critical User Experience & Responsiveness Blocker)
- **Component:** `main.ino` / Timing Subsystem

### 1. 5-Why Analysis
1. **Why is the mode-switch pushbutton unresponsive when pressed?**
   Because the microcontroller does not detect the button press during the LED blinking cycle.
2. **Why does the microcontroller fail to detect the button press?**
   Because the program execution is halted inside `delay(1000)` functions during LED ON and OFF states.
3. **Why is `delay()` being used for blink intervals?**
   Because the initial legacy prototype relied on simple linear synchronous delays taught in basic introductory tutorials.
4. **Why is linear synchronous delay unsuitable for interactive embedded systems?**
   Because `delay()` keeps the MCU in a tight NOP loop, preventing the polling of input pins, sensors, and communication peripherals.
5. **Root Cause:**
   Absence of an asynchronous, non-blocking cooperative scheduling mechanism using system clock timestamps (`millis()`).

### 2. Fishbone (Ishikawa) Diagram
```
  Methods                        Software Architecture
    │                                     │
    ├─ Synchronous polling                ├─ Blocking delay() implementation
    ├─ No timer interrupt                 └─ Monolithic super-loop
    │                                     │
    ├─────────────────────────────────────┴──────────► [SYMPTOM: 1000ms UI
    │                                     │             Input Latency]
    ├─ Microchip ATmega328P               ├─ Novice example copy-paste
    └─ Single 16MHz Core (no RTOS)        └─ Lack of QA responsiveness metrics
    │                                     │
  Hardware / Environment                Developer Process
```

### 3. Permanent Corrective Action
- Replaced all calls to `delay()` with an asynchronous state machine tracked via `millis() - lastTimestamp >= interval`.
- Loop execution frequency increased from **1 Hz** to **>65,000 Hz**, achieving sub-millisecond input responsiveness.

---

## Issue #2: [HARDWARE-SIG] Floating GPIO Pin & Switch Contact Bounce Induces Spurious Triggers

- **Issue ID:** `#2`
- **Severity:** Critical
- **Priority:** P1 (Functional Failure)
- **Component:** `button_handler.cpp` / Electrical Hardware

### 1. 5-Why Analysis
1. **Why does the LED mode toggle erratically when the button is not pressed or lightly touched?**
   Because Digital Pin 2 transitions unpredictably between `HIGH` and `LOW` states.
2. **Why does Digital Pin 2 transition without a firm button press?**
   Because the input pin is left floating with infinite impedance when the switch contact is open.
3. **Why is the input pin floating?**
   Because the physical schematic omitted an external pull-down/pull-up resistor, and the MCU internal pull-up resistor was not activated in firmware.
4. **Why does a single physical press cause multi-state skipping?**
   Because physical metallic switch contacts exhibit mechanical bounce (oscillating make-and-break) for 5 to 25 ms upon actuation.
5. **Root Cause:**
   Co-existence of a floating high-impedance input state and lack of software-based edge-settling hysteresis (debouncing).

### 2. Fishbone (Ishikawa) Diagram
```
  Hardware Electronics                 Firmware Logic
    │                                     │
    ├─ Omission of pull-up resistor       ├─ Default pinMode(pin, INPUT)
    ├─ High ambient EMI pickup            └─ Direct digitalRead() trigger
    │                                     │
    ├─────────────────────────────────────┴──────────► [SYMPTOM: Erratic Mode
    │                                     │             Switching & Jitter]
    ├─ Mechanical contact spring-back     ├─ Breadboard stray capacitance
    └─ Switch contact oxidation           └─ Rapid loop execution sampling bounce
    │                                     │
  Switch Mechanics                      Physical Prototyping
```

### 3. Permanent Corrective Action
- Configured GPIO Pin 2 with `INPUT_PULLUP`, connecting the internal 20kΩ–50kΩ pull-up resistor to 5V VCC.
- Implemented a 50ms temporal hysteresis window in `ButtonHandler::update()` to discard transient oscillations.

---

## Issue #3: [ELECTRICAL-SAFETY] Direct LED Drive Exceeds ATmega328P Absolute Maximum GPIO Rating

- **Issue ID:** `#3`
- **Severity:** High
- **Priority:** P2 (Hardware Reliability & Thermal Damage Risk)
- **Component:** Electrical Hardware Interface / Pin 10

### 1. 5-Why Analysis
1. **Why did the microcontroller become excessively warm during continuous operation?**
   Because excessive current was being drawn through GPIO Pin 10.
2. **Why was excessive current flowing through Pin 10?**
   Because an external high-brightness LED was wired directly between Pin 10 and Ground without a series current-limiting resistor.
3. **Why did this cause overcurrent?**
   Because an LED is a diode with an exponential I-V curve; once forward voltage $V_f$ (~2.0V) is exceeded, dynamic resistance approaches zero.
4. **How much current was drawn?**
   $$I = \frac{V_{CC} - V_f}{R_{internal}} = \frac{5.0V - 2.0V}{\approx 25\Omega} \approx 120\text{ mA}$$
   This exceeds the ATmega328P Absolute Maximum Rating of **40.0 mA** per I/O pin by 300%.
5. **Root Cause:**
   Failure to apply Ohm's Law and manufacturer electrical absolute maximum ratings during breadboard wiring and schematic design.

### 2. Fishbone (Ishikawa) Diagram
```
  Component Specifications             Circuit Design
    │                                     │
    ├─ ATmega328P max rating: 40mA        ├─ Missing series current limiter
    ├─ LED forward voltage: 2.0V          └─ Direct breadboard jumper connection
    │                                     │
    ├─────────────────────────────────────┴──────────► [SYMPTOM: Pin Latchup &
    │                                     │             Thermal Overstress]
    ├─ Assumption that 5V pins are safe   ├─ Absence of hardware peer review
    └─ Visual testing without DMM         └─ Omission of electrical BoM checks
    │                                     │
  Human Assumptions                     Quality Assurance
```

### 3. Permanent Corrective Action
- Inserted a **220Ω (1/4W) series metal-film resistor**:
  $$I_{LED} = \frac{5.0V - 2.0V}{220\Omega} = \frac{3.0V}{220\Omega} \approx 13.63\text{ mA}$$
- This operates the LED safely within the conservative 20mA continuous continuous limit while maintaining crisp luminosity.

---

## Issue #4: [SYSTEM-RELIABILITY] Microcontroller Lockup Due to High-Voltage Switching / EMI Without Watchdog

- **Issue ID:** `#4`
- **Severity:** Major
- **Priority:** P2 (System Availability & Fault Tolerance)
- **Component:** Firmware Architecture / System Supervisor

### 1. 5-Why Analysis
1. **Why did the system freeze permanently during inductive load switching in nearby lab equipment?**
   Because the program counter (PC) leaped to an invalid memory location or entered an unhandled exception state.
2. **Why did the program counter leap?**
   Because electrical transient bursts (EMI / voltage dip) corrupted the instruction register or bus lines.
3. **Why did the system remain dead indefinitely until a manual power cycle?**
   Because there was no supervisory watchdog circuit monitoring microcontroller execution liveness.
4. **Why was no watchdog active?**
   Because the firmware initialized without configuring the internal ATmega328P Watchdog Timer (WDT).
5. **Root Cause:**
   Lack of an autonomous hardware supervisor and fail-safe recovery architecture in the embedded software design.

### 2. Fishbone (Ishikawa) Diagram
```
  Power & Environment                  Hardware Supervisor
    │                                     │
    ├─ Industrial EMI spikes              ├─ Internal WDT disabled by default
    ├─ VCC supply ripple                  └─ Brown-out Detection (BOD) unset
    │                                     │
    ├─────────────────────────────────────┴──────────► [SYMPTOM: Silent MCU Freeze
    │                                     │             Requiring Hard Reboot]
    ├─ Infinite loop trapping             ├─ Assumption of clean benchtop power
    └─ Missing heartbeat telemetry        └─ No automated health check in loop
    │                                     │
  Firmware Architecture                 Design Validation
```

### 3. Permanent Corrective Action
- Integrated `<avr/wdt.h>` with a 2000ms watchdog timeout (`wdt_enable(WDTO_2S)`).
- Placed `wdt_reset()` at the apex of the main loop. If an execution lockup exceeds 2.0s, the hardware automatically resets the CPU, restoring normal operation within 15 milliseconds.
