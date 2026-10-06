---
name: Bug Report
about: Create a report to help us improve embedded firmware reliability
title: "[BUG] "
labels: ["bug", "triage-needed"]
assignees: ""
---

**Problem Description:**
A clear and concise description of what the bug is in the embedded hardware/software.

**Hardware Configuration:**
- Microcontroller: Arduino Uno R3 (ATmega328P)
- Operating Voltage: 5.0V VCC
- Connected Peripherals: Pushbutton on Pin 2, LED on Pin 10, Heartbeat on Pin 9
- Clock Frequency: 16 MHz

**Steps to Reproduce:**
1. Flash firmware version `v...`
2. Connect circuit as per `diagram.json`
3. Press button / apply input `...`
4. Observe failure in logic analyzer or multimeter

**Expected Behavior:**
What the system should do under normal conditions.

**Actual Behavior:**
What actually happened (e.g., system freeze, flickering, overheating).

**Severity & Priority:**
- Severity: [Critical | Major | Moderate | Minor]
- Priority: [P1 - Blocker | P2 - High | P3 - Medium | P4 - Low]

**Root Cause Analysis (5-Why / Fishbone Reference):**
Provide root cause hypothesis or reference to QA document.
