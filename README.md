# NUS Future Fabrication Lab — Magnetic Skin for Social Robots

A 6-month research internship (Research Assistant) at the **Future Fabrication Laboratory, National University of Singapore (NUS)**, under the supervision of **Prof. Justin Moon**. The placement centers on designing a **magnetic skin system for social robots** — a flexible electromagnetic surface that can levitate and actuate small permanent magnets using arrays of planar coils, without physical contact.

## About the project

Current social robot surfaces are largely passive: they can register contact but can't actively exert forces, move objects, or change surface properties in response to interaction. A magnetically active skin — a flexible substrate with embedded electromagnetic coil arrays — introduces a new interaction modality: robots that can levitate, move, and actuate small magnets at the skin surface without touching them.

The core research challenge is that existing planar magnetic levitation systems assume a rigid, fixed coil substrate. No prior system demonstrates magnetic levitation on a flexible, skin-like substrate conforming to a non-planar robot body. This repository tracks the code, 3D-printed hardware, and reference material produced while working on that problem.

## System design

- **Type-A / Type-B coil architecture** — Type-A coils are energized continuously and form a static background field (like a virtual Halbach magnet); Type-B coils are energized selectively to levitate/move the magnet locally.
- **16-layer PCB coil array** (15 active coil layers + 1 wiring layer), 2D Halbach layout, split into 4 independently driven quadrants for fault tolerance.
- **Square coils over cylindrical** — FEA showed square coils tile the array area more efficiently, giving higher and more uniform flux density with fewer dead zones.
- **Three-region force model** (centre / side / corner) — boundary effects at the array edges mean a single global force formula doesn't hold across the whole surface.
- **Position sensing** — a Hall-effect sensor array feeds a ML inference pipeline (TensorFlow Lite on-microcontroller) and a Kalman filter to estimate mover position, avoiding the occlusion problems of optical tracking under a skin substrate.
- **Control** — dual-loop: an outer 5-DOF PID loop computes a desired wrench (force + torque) from pose error, which is allocated to per-coil currents via the pseudo-inverse of the wrench–current matrix (K⁺); an inner current loop drives MOSFET/PWM coil drivers. PID must act on the task-space wrench, not raw coil currents, or the system is unstable.
- **VCM (voice coil motor) actuation was evaluated and rejected** — it can levitate its own platform but can't project force onto a separate external magnet.
- **Capacitive touch layer** — a simpler, lower-power fallback interaction mode: 6 touch zones each mapped one-to-one to an actuator, used to validate sensing/actuation wiring independently of the full levitation stack.

**Open problem:** moving from a rigid PCB to a flexible skin substrate breaks the force model, since it assumes fixed coil geometry. Two mitigations are proposed for future work: (1) online geometry estimation via embedded strain/inertial sensors, or (2) robustness-optimized control (H∞ / MPC) tolerant of bounded model error.

## Repository structure

```
├── actuation/         Capacitive touch sensing + actuator driver code (skin validation layer), reference screenshots/recordings
├── magnet-tracking/   Coil-based magnet sensing/tracking firmware, Processing visualizer, 3D print files
├── image/             General reference screenshots for this README
└── README.md
```

### `actuation/`

Arduino code for the capacitive touch-to-actuation validation layer described above:

- `main_code.cpp` — 6-channel capacitive sensor loop with per-channel debounce/toggle logic that drives a mapped actuator pin
- `motor_driver.cpp` — motor driver control logic
- `sensing.cpp` / `ploter_sen.cpp` — sensor read/plotting helpers for tuning thresholds on the Serial Plotter
- `images and recordings/` — screenshots and captures from testing/tuning sessions

### `magnet-tracking/`

Firmware and tooling for coil-based magnet position sensing (the Hall-sensor side of the levitation system):

- `magnet_track.cpp` / `magnet_track2.cpp` — coil sensing + gentle actuation logic, averaging sensor readings over a reporting interval
- `1_node.CPP` / `4_node.cpp` — single-node and multi-node sensing sketches
- `Processing.pde` — Processing sketch for visualizing tracked magnet position
- `3d-printing/` — STL/gcode/3mf files for the coil housings and test prints

## Gallery

<table>
<tr>
<td><img src="image/Screenshot%202026-05-07%20135558.png" width="300"/></td>
<td><img src="image/Screenshot%202026-06-08%20175541.png" width="300"/></td>
<td><img src="image/Screenshot%202026-05-25%20064459.png" width="300"/></td>
</tr>
<tr>
<td><img src="image/Screenshot%202026-05-19%20120602.png" width="300"/></td>
<td><img src="image/Screenshot%202026-06-22%20134915.png" width="300"/></td>
</tr>
</table>

## Status

This repository is under active development as part of an ongoing internship (08/12/2025 – 22/05/2026).
