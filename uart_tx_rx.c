/**
 * @file uart_tx_rx.c
 * @brief UART Communication Implementation
 * 
 * This code initializes UART, sends a message every 1 second, 
 * and echoes back any received characters.
 */

#include <stdint.h>

// Mock definitions for demonstration (Target: Generic ARM Cortex-M)
#define UART_BASE 0x4000C000
#define UART_DR   (*(volatile uint32_t *)(UART_BASE + 0x00))
#define UART_FR   (*(volatile uint32_t *)(UART_BASE + 0x18))
#define UART_IBRD (*(volatile uint32_t *)(UART_BASE + 0x24))
#define UART_FBRD (*(volatile uint32_t *)(UART_BASE + 0x28))
#define UART_LCRH (*(volatile uint32_t *)(UART_BASE + 0x2C))
#define UART_CTL  (*(volatile uint32_t *)(UART_BASE + 0x30))

#define UART_FR_TXFF (1 << 5) // Transmit FIFO full
#define UART_FR_RXFE (1 << 4) // Receive FIFO empty

/**
 * @brief Initialize UART with 9600 baud rate, 8N1
 */
void UART_Init(void) {
    // 1. Disable UART
    UART_CTL = 0;
    
    // 2. Set Baud Rate (Assuming 16MHz clock, 9600 baud)
    // BRD = 16,000,000 / (16 * 9600) = 104.166
    UART_IBRD = 104;
    UART_FBRD = 11;
    
    // 3. Line Control: 8-bit, no parity, 1 stop bit, FIFO enabled
    UART_LCRH = (0x3 << 5) | (1 << 4);
    
    // 4. Enable UART, TX, and RX
    UART_CTL = (1 << 0) | (1 << 8) | (1 << 9);
}

/**
 * @brief Send a single character over UART
 */
void UART_Transmit(char c) {
    while (UART_FR & UART_FR_TXFF);
    UART_DR = c;
}

/**
 * @brief Send a string over UART
 */
void UART_Print(const char* str) {
    while (*str) {
        UART_Transmit(*str++);
    }
}

/**
 * @brief Receive a single character from UART
 */
char UART_Receive(void) {
    while (UART_FR & UART_FR_RXFE);
    return (char)(UART_DR & 0xFF);
}

/**
 * @brief Simple delay function
 */
void Delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 1000; i++);
}

int main(void) {
    UART_Init();
    
    while (1) {
        // Task: Send "Hello from MCU" every 1s
        UART_Print("Hello from MCU\r\n");
        
        // Task: RX Echo (Check if data is available)
        // In a real implementation, this would often be interrupt-driven
        // For this task, we check for a short window to simulate non-blocking
        for(int i = 0; i < 100; i++) {
            if (!(UART_FR & UART_FR_RXFE)) {
                char received = UART_Receive();
                UART_Print("Echo: ");
                UART_Transmit(received);
                UART_Print("\r\n");
            }
            Delay_ms(10);
        }
    }
    
    return 0;
}
