// Define the hardware addresses manually for MSP430G2553
#define WDTCTL      (*((volatile unsigned int *)0x0120)) // Watchdog Timer Control
#define WDTPW       0x5A00                               // Watchdog password
#define WDTHOLD     0x0080                               // Watchdog hold

// Port 1 Registers
#define P1DIR       (*((volatile unsigned char *)0x0022)) // Port 1 Direction
#define P1OUT       (*((volatile unsigned char *)0x0021)) // Port 1 Output
#define P1IN        (*((volatile unsigned char *)0x0020)) // Port 1 Input
#define P1REN       (*((volatile unsigned char *)0x0027)) // Port 1 Resistor Enable
#define P1IE        (*((volatile unsigned char *)0x0025)) // Port 1 Interrupt Enable
#define P1IFG       (*((volatile unsigned char *)0x0023)) // Port 1 Interrupt Flag
#define P1IES       (*((volatile unsigned char *)0x0024)) // Port 1 Interrupt Edge Select

// Basic Clock System Control Registers
#define BCSCTL1     (*((volatile unsigned char *)0x0057)) // Basic Clock System Control 1
#define DCOCTL      (*((volatile unsigned char *)0x0056)) // DCO Control

// Calibration data for 1 MHz
#define CALBC1_1MHZ (*((volatile unsigned char *)0x10FE)) // 1 MHz calibration constant
#define CALDCO_1MHZ (*((volatile unsigned char *)0x10FF)) // 1 MHz calibration constant

// LED and Switch Pins
#define LEDS        0x41      // Placeholder mask for LED pins (P1.0 and P1.6)
#define SWITCHES    0x08      // Placeholder mask for switch pin (e.g., P1.3)
#define LED_GREEN   0x40      // P1.6 - Green LED
#define LED_RED     0x01      // P1.0 - Red LED

// General Interrupt Enable
#define GIE         0x0008    // General interrupt enable

// Low Power Mode 0 with interrupts
#define LPM0_bits   0x0010
