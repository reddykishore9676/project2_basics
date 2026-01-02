# Project 2: Sensor Interfacing and Communication Protocols

## 🎯 Objective
This project is an intermediate-level embedded systems exercise focused on implementing and demonstrating three fundamental communication protocols: **UART**, **I2C**, and **SPI**.

## ⚙️ Project Structure
- `uart_tx_rx.c`: UART Communication Code (9600 8N1, TX/RX Echo).
- `i2c_sensor_read.c`: I2C Interface for **MPU6050** (Accel/Gyro).
- `spi_comm_device.c`: SPI Interface for **25LC256 EEPROM**.
- `Wiring_Diagram.png`: Visual guide for hardware connections.
- `Report.pdf`: Comprehensive project report.
- `Serial_Screenshots/`: Verification images of terminal output.

## ✨ Features
| Protocol | Device | Key Functionality |
| :--- | :--- | :--- |
| **UART** | PC Terminal | Periodic "Hello" message and character echo. |
| **I2C** | MPU6050 | Reading 14 bytes of raw motion data. |
| **SPI** | 25LC256 | Full write/read cycle with status polling. |

## 🛠️ Hardware Requirements
- Microcontroller (ARM Cortex-M recommended).
- USB-to-Serial Converter.
- MPU6050 Sensor & 25LC256 EEPROM.

## 🚀 Getting Started
1. Clone the repository.
2. Replace mock HAL functions with your MCU's specific drivers.
3. Compile, flash, and monitor the output via a 9600 baud serial terminal.
