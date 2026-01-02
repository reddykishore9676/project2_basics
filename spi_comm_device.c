/**
 * @file spi_comm_device.c
 * @brief SPI Interface for 25LC256 EEPROM
 * 
 * This code initializes SPI, writes a byte to the EEPROM,
 * reads it back, and displays the result via UART.
 */

#include <stdint.h>
#include <stdio.h>

// 25LC256 Instructions
#define READ  0x03
#define WRITE 0x02
#define WREN  0x06 // Write Enable
#define RDSR  0x05 // Read Status Register

// Mock SPI Functions
void SPI_Init(void) {
    /* Initialize SPI: Mode 0, 8-bit, MSB First */
}

uint8_t SPI_Transfer(uint8_t data) {
    /* Send data and return received byte */
    return 0; 
}

void CS_Low(void) { /* Pull Chip Select Low */ }
void CS_High(void) { /* Pull Chip Select High */ }

// UART Print function (reused)
extern void UART_Print(const char* str);

/**
 * @brief Write a byte to EEPROM
 */
void EEPROM_WriteByte(uint16_t address, uint8_t data) {
    // 1. Send Write Enable
    CS_Low();
    SPI_Transfer(WREN);
    CS_High();

    // 2. Send Write Command and Address
    CS_Low();
    SPI_Transfer(WRITE);
    SPI_Transfer((uint8_t)(address >> 8));   // High byte
    SPI_Transfer((uint8_t)(address & 0xFF)); // Low byte
    SPI_Transfer(data);
    CS_High();

    // 3. Wait for write cycle to complete (Poll Status Register)
    uint8_t status;
    do {
        CS_Low();
        SPI_Transfer(RDSR);
        status = SPI_Transfer(0xFF);
        CS_High();
    } while (status & 0x01); // WIP bit
}

/**
 * @brief Read a byte from EEPROM
 */
uint8_t EEPROM_ReadByte(uint16_t address) {
    uint8_t data;
    CS_Low();
    SPI_Transfer(READ);
    SPI_Transfer((uint8_t)(address >> 8));
    SPI_Transfer((uint8_t)(address & 0xFF));
    data = SPI_Transfer(0xFF);
    CS_High();
    return data;
}

int main(void) {
    SPI_Init();
    char msg[50];
    uint16_t test_addr = 0x0010;
    uint8_t test_val = 0xAB;

    UART_Print("SPI EEPROM Test Starting...\r\n");

    // Write Data
    EEPROM_WriteByte(test_addr, test_val);
    sprintf(msg, "Written: 0x%02X to 0x%04X\r\n", test_val, test_addr);
    UART_Print(msg);

    // Read Data
    uint8_t read_val = EEPROM_ReadByte(test_addr);
    sprintf(msg, "Read: 0x%02X from 0x%04X\r\n", read_val, test_addr);
    UART_Print(msg);

    if (read_val == test_val) {
        UART_Print("SPI Communication SUCCESS!\r\n");
    } else {
        UART_Print("SPI Communication FAILED!\r\n");
    }

    while (1);
    return 0;
}
