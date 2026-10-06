# MIT ACADEMY OF ENGINEERING, ALANDI (D), PUNE
### DEPARTMENT OF ELECTRONICS & TELECOMMUNICATION ENGINEERING
**Academic Year:** 2026 – 2027 | **Semester:** VII | **Class:** BTech | **Division:** C  
**Course Code & Name:** 2307476T - Project Management  
**Activity No:** Activity 2  
**Format No:** ACAD/DI/11B | **Rev No:** 01 | **Rev Date:** 01/07/2025  

---

## ACTIVITY DETAILS
- **Name of Activity:** GitHub-Based QA Documentation and Problem Solving based on Electronics/Cross Domain Projects
- **Course Outcome:** CO2 - Implementation of Collaborative tools
- **RBT Level:** Level 3 (Apply)
- **Marks Allocated:** 15 Marks
- **Student Name:** Aryan Kumar
- **Course Teacher:** Dr. Ashish Mulajkar

---

### Pre-Reflection of Activity
In traditional academic electronics projects, firmware development often proceeds without formal version control or defect tracking. Minor code changes are saved under arbitrary file names, and hardware bugs (such as bouncing switches, floating inputs, and thermal overcurrent) are often fixed with informal hacks rather than systematic root cause analysis. Utilizing collaborative platforms like GitHub provides structured traceability, branch isolation, and transparent documentation essential for high-reliability embedded system development.

---

## SECTION A: GitHub Repository Link
- **Repository URL:** [https://github.com/Aryankr0711/PM-IA-2-3.git](https://github.com/Aryankr0711/PM-IA-2-3.git)
- **Branch:** `main`
- **Simulation Manifests:** `diagram.json` (Wokwi Virtual Hardware Schematic) and `wokwi.toml`

---

## SECTION B: Brief Description of Code

### 1. Hardware Architecture
The target hardware system is an **Industrial-Grade, Fault-Tolerant Multi-Mode LED Controller** implemented on the Microchip ATmega328P (Arduino Uno R3) running at 16 MHz.

- **Status LED:** Digital Pin 13
- **External Multi-Pattern LED:** Digital Pin 10 (Hardware PWM-driven via Timer 1)
- **Diagnostic Heartbeat LED:** Digital Pin 9 (PWM-driven alive beacon)
- **Mode Toggle Pushbutton:** Digital Pin 2 (External interrupt pin INT0 with active-low input pull-up)
- **Current-Limiting Protection:** 220 $\Omega$ series resistor limiting current to $I = (5.0\text{V} - 2.0\text{V}) / 220\Omega \approx 13.63\text{ mA}$

### 2. Software Architecture & Finite State Machine (FSM)
The software is engineered with a strict **non-blocking cooperative scheduling architecture** based on `millis()` timestamp tracking. The execution flow cycles through 4 states:
1. `STATE_NORMAL_BLINK`: 1.0 Hz square wave (500 ms ON / 500 ms OFF).
2. `STATE_FAST_ALERT`: 5.0 Hz high-frequency warning blink (100 ms ON / 100 ms OFF).
3. `STATE_SOS_BEACON`: Standard Morse code S.O.S. optical beacon (`... --- ...`).
4. `STATE_BREATHING_PWM`: Sinusoidal PWM duty cycle modulation (0 to 255) for smooth fading.

### 3. Safety & Diagnostic Subsystems
- **Hardware Watchdog Timer (AVR WDT):** Armed with a 2.0s timeout window (`wdt_enable(WDTO_2S)`) and kicked via `wdt_reset()` in each loop cycle.
- **Serial Telemetry:** Outputs loop frequency (exceeding 60,000 Hz) and operational state telemetry over UART at 115,200 baud.

---

## SECTION C: QA Issues Logged & Root Cause Analysis

### Summary of Logged Defects
Four realistic embedded defects were logged, tracked, and systematically resolved:

| Issue ID | Defect Title | Category | Severity | Priority | RCA Methodology | Resolution Status |
|---|---|---|---|---|---|---|
| **#1** | Blocking `delay()` Starves CPU & Introduces 1000ms Latency | Firmware Architecture | High | P1 | 5-Why & Fishbone | Resolved via PR #5 |
| **#2** | Floating Input Pin & Switch Contact Bounce Induces Spurious Triggers | Hardware / Signal | Critical | P1 | 5-Why & Fishbone | Resolved via PR #6 |
| **#3** | Direct LED Drive Exceeds ATmega328P Absolute Max GPIO Rating | Electrical Safety | High | P2 | 5-Why & Fishbone | Resolved via PR #7 |
| **#4** | Microcontroller Lockup Due to High-Voltage Switching / EMI Without Watchdog | System Reliability | Major | P2 | 5-Why & Fishbone | Resolved via PR #8 |

### Detailed 5-Why and Fishbone RCA Summaries
- **Issue #1 (Blocking Delay):** 5-Why analysis revealed that legacy linear tutorials utilize synchronous `delay()`, which blocks the MCU from polling inputs. Solved by migrating to non-blocking elapsed time checks.
- **Issue #2 (Floating Pin & Bounce):** Contact bounce caused multiple transitions in <20ms while floating input picked up electrostatic noise. Solved by enabling `INPUT_PULLUP` and a 50ms software settling filter.
- **Issue #3 (Overcurrent):** Direct LED connection drew ~120 mA, exceeding the ATmega328P 40 mA absolute maximum rating. Solved by inserting a 220 $\Omega$ series resistor ($I \approx 13.6\text{ mA}$).
- **Issue #4 (EMI Hang):** Power transients caused program counter corruption. Solved by enabling the internal AVR Watchdog Timer (2.0s timeout) to autonomously reboot the system upon hang.

---

## SECTION D: Collaboration Summary
- **Branching Workflow:** Feature branch strategy (`bugfix/issue-1-nonblocking-timer`, `bugfix/issue-2-button-debouncing`, `bugfix/issue-3-current-limiting-resistor`, `bugfix/issue-4-watchdog-timer-integration`).
- **Code Review:** Each pull request included test logs, scope/multimeter measurements, and formal sign-offs.
- **Traceability:** Pull requests used GitHub auto-closing keywords (`Closes #1`, `Closes #2`, etc.) to preserve a complete historical audit trail.

---

## SECTION E: Learning Outcome
- **Mapping to CO2:** Demonstrated complete mastery of collaborative tools (Git, GitHub Issues, PR workflows, Kanban boards) applied to electronics and cross-domain embedded engineering.
- **Bloom's Taxonomy:** Level 3 (Apply).
- **Core Insights:**
  1. Learned the vital importance of hardware-software co-design in diagnosing embedded faults.
  2. Applied structured Root Cause Analysis (5-Why & Fishbone) rather than superficial symptom patching.
  3. Gained hands-on experience in defensive embedded programming (Watchdog timers, non-blocking schedulers, and electrical current budgeting).

---

## SECTION F: Moodle Upload Checklist
- [x] Activity Header & Academic Details verified
- [x] Repository link active and public: `https://github.com/Aryankr0711/PM-IA-2-3.git`
- [x] Source code, schematics, and configuration files organized in repository
- [x] 4+ QA issues logged with full RCA and resolution documentation
- [x] Report prepared in accordance with format ACAD/DI/11B

---

## SECTION G: Project Planning and Tracking
- **Agile Kanban Methodology:** Tracked tasks across Backlog, In Progress, In Review, and Done.
- **Milestone Timeline:** Completed in 4 planned phases (Planning, Prototyping & QA Logging, Solution Branching & PRs, Final Verification & Reporting).
- **Requirements Traceability Matrix:** Every functional requirement directly maps to a source code file, a QA issue, and a verified test case.

---

### Post-Reflection of Activity
Using GitHub as an integrated development, QA, and project management environment transformed an ordinary embedded coding exercise into an industry-grade quality assurance process. The combination of issue templates, pull request reviews, and systematic root cause analysis ensured that all electrical and timing defects were eliminated permanently with full audit traceability.
