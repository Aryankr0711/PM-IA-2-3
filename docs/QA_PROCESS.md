# Embedded Quality Assurance (QA) Process & Verification Plan

## 1. Quality Assurance Framework Overview
This project adheres to an industry-standard embedded software and hardware Quality Assurance lifecycle, structured across 4 distinct phases:

```
[Requirement & Schematic Review] ──► [Static Code Analysis] ──► [Hardware-in-the-Loop Testing] ──► [Continuous Peer Review]
               │                                   │                                 │                             │
               ▼                                   ▼                                 ▼                             ▼
       BoM / Pin Safety Verification       Doxygen & C++ Standards           Logic Analyzer & Current Probes       GitHub PR & Issue Tracking
```

---

## 2. Bug Severity & Priority Matrix

| Level | Severity Definition | Priority | Target SLA for Fix |
|---|---|---|---|
| **Critical** | Hardware damage risk, system lockup, floating inputs causing uncontrollable behavior | **P1 (Blocker)** | Immediate (< 4 Hours) |
| **High** | Core functionality impaired, timing latency > 100ms, user input dropped | **P2 (High)** | Within 24 Hours |
| **Medium** | Edge-case glitch, heartbeat jitter under heavy load, diagnostic formatting error | **P3 (Medium)** | Within 3 Days |
| **Low** | Code style inconsistency, non-critical comment typo, minor serial log wording | **P4 (Low)** | Next Release Cycle |

---

## 3. Test Cases & Verification Matrix

| Test ID | Test Scenario | Input Stimulus | Expected Outcome | Verification Tool | Status |
|---|---|---|---|---|---|
| **TC-01** | Non-blocking loop responsiveness | Rapid sequential button pressing | State advances sequentially without missing events | Logic Analyzer (Channel 0) | **PASSED** |
| **TC-02** | Contact debouncing verification | 15ms mechanical contact bounce | Exactly 1 state increment registered | Storage Oscilloscope | **PASSED** |
| **TC-03** | GPIO Current Limit Safety | Continuous HIGH on Pin 10 | Current measured $\le 15\text{ mA}$ | Digital Multimeter (mA range) | **PASSED** |
| **TC-04** | Watchdog Timer Auto-Recovery | Serial command `'w'` (infinite loop) | MCU resets and reboots within $2.0 \pm 0.1\text{ s}$ | Serial Monitor (115200 baud) | **PASSED** |
| **TC-05** | Diagnostic Heartbeat Periodicity | Normal continuous run | Pin 9 pulses 50ms pulse every 1000ms | Frequency Counter | **PASSED** |

---

## 4. GitHub Issue & Collaboration Protocol
1. **Issue Logging:** Every defect discovered during physical prototyping or simulation must be logged using `.github/ISSUE_TEMPLATE/bug_report.md`.
2. **Branching Strategy:** Direct commits to `main` are restricted. Fixes must be authored on branches prefixed with `bugfix/issue-<id>`.
3. **Peer Review:** Every Pull Request requires at least one approving peer review validating both schematic calculations and firmware diffs.
4. **Closing Traceability:** Every PR must reference the closed issue using standard GitHub keywords (e.g., `Resolves #1`, `Closes #2`).
