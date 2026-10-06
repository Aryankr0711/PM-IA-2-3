---
name: QA Verification Test Case
about: Document formal verification testing for hardware and firmware fixes
title: "[QA-TEST] "
labels: ["qa-verified", "testing"]
assignees: ""
---

### Test Case Overview
- **Test ID:** TC-EMB-xxx
- **Module:** [Timing | GPIO | Debounce | Watchdog | Serial]
- **Target Release:** v1.0.0

### Test Prerequisites
1. Hardware setup verified against electrical schematic.
2. Serial monitor configured to 115200 baud.
3. Oscilloscope / Logic Analyzer probe attached to Pin 10 / Pin 2.

### Verification Procedure
| Step | Action | Expected Result | Pass / Fail |
|---|---|---|---|
| 1 | Power on MCU | Heartbeat blips every 1000ms | PASS |
| 2 | Depress button for 20ms | No unintended double-trigger | PASS |
| 3 | Continuous button hold | State switches once without jitter | PASS |
| 4 | Trigger simulated hang | Watchdog resets MCU within 2.0s | PASS |
