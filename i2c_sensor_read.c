/**
 * @file i2c_sensor_read.c
 * @brief I2C Interface for MPU6050 Accelerometer/Gyroscope
 * 
 * This code initializes I2C, reads raw data from MPU6050 registers,
 * and prints the values via UART.
 */

#include <stdint.h>
#include <stdio.h>

// MPU6050 I2C Address
#define MPU6050_ADDR 0x68

// MPU6050 Register Map
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43

// Mock I2C Functions (Target: Generic HAL-style)
void I2C_Init(void) {
    /* Initialize I2C Peripheral: 100kHz Standard Mode */
}

void I2C_Write(uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    /* Send Start -> Dev Addr (W) -> Reg Addr -> Data -> Stop */
}

void I2C_Read(uint8_t dev_addr, uint8_t reg_addr, uint8_t* buffer, uint16_t size) {
    /* Send Start -> Dev Addr (W) -> Reg Addr -> Repeated Start -> Dev Addr (R) -> Read N bytes -> Stop */
}

// UART Print function (reused from uart_tx_rx.c)
extern void UART_Print(const char* str);

/**
 * @brief Initialize MPU6050
 */
void MPU6050_Init(void) {
    I2C_Init();
    // Wake up MPU6050 (write 0 to PWR_MGMT_1)
    I2C_Write(MPU6050_ADDR, PWR_MGMT_1, 0x00);
}

/**
 * @brief Read and Print Sensor Data
 */
void MPU6050_Read_Data(void) {
    uint8_t data[14];
    int16_t ax, ay, az, gx, gy, gz;
    char msg[100];

    // Read 14 bytes starting from ACCEL_XOUT_H
    I2C_Read(MPU6050_ADDR, ACCEL_XOUT_H, data, 14);

    // Combine High and Low bytes
    ax = (int16_t)((data[0] << 8) | data[1]);
    ay = (int16_t)((data[2] << 8) | data[3]);
    az = (int16_t)((data[4] << 8) | data[5]);
    
    gx = (int16_t)((data[8] << 8) | data[9]);
    gy = (int16_t)((data[10] << 8) | data[11]);
    gz = (int16_t)((data[12] << 8) | data[13]);

    // Format and Print via UART
    sprintf(msg, "Accel: X=%d Y=%d Z=%d | Gyro: X=%d Y=%d Z=%d\r\n", ax, ay, az, gx, gy, gz);
    UART_Print(msg);
}

int main(void) {
    MPU6050_Init();
    
    while (1) {
        MPU6050_Read_Data();
        // Delay 500ms
        for (volatile uint32_t i = 0; i < 500000; i++);
    }
    
    return 0;
}
