# 🚗 Tilt-Aware Vehicle Monitor with Raspberry Pi, MPU6050 & MAX7219

This project implements a tilt detection and display system for a miniature vehicle using Raspberry Pi, MPU6050 IMU, and a MAX7219 7-segment LED module.

The system detects if the vehicle is:
- Going uphill or downhill
- Parked or in motion
- On a flat surface

And displays the vehicle's tilt angle and status in real time. It also warns if the angle exceeds a critical threshold.

---

## 🎯 Project Objectives

1. **Configure MPU6050:**
   - Sample Rate: `200Hz`
   - Enable **Digital Low Pass Filter** with cutoff < `100Hz`
   - Set accelerometer range to `±16g`
   - Set gyroscope range to `±1000 °/s`

2. **Determine Slope Status:**
   - Analyze the X-axis tilt angle
   - Print state to terminal: `"Flat"`, `"Uphill"`, or `"Downhill"`

3. **Display Status on LED Module (MAX7219):**
   - Format: `UP-35.6-D`
     - `UP`/`DN`/`--`: Uphill / Downhill / Flat
     - `35.6`: Tilt angle (1 decimal)
     - `D`/`P`: Driving or Parked
   - If tilt angle exceeds ±40°, flash LED to trigger warning

---

## 🔧 Hardware Requirements

- 🍓 Raspberry Pi (any model with I2C + SPI)
- 🧭 MPU6050 (I2C interface)
- 🔢 MAX7219 7-Segment Display (SPI interface)
- 🚗 Vehicle chassis (for testing slope)
- 🧰 Wires, Breadboard or PCB

---

## 📂 Project Structure

