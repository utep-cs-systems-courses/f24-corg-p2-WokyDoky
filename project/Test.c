#include <msp430.h>
#include "buzzer.h"

#define LED_RED BIT0               // P1.0
#define LED_GREEN BIT6             // P1.6

volatile char mode = 0; // 0: off, 1: green LED & buzzer tone 1, 2: red LED & buzzer tone 2, 3: both LEDs & buzzer tone 3

void configureClocks() {
    BCSCTL1 = CALBC1_1MHZ;       // Set DCO to 1 MHz
    DCOCTL = CALDCO_1MHZ;
}

void buzzer_init() {
    P2SEL2 &= ~(BIT6 | BIT7);    // Clear P2SEL2 bits 6 and 7
    P2SEL &= ~BIT7;              // Clear P2SEL.7
    P2SEL |= BIT6;               // Set P2SEL.6 for Timer A output
    P2DIR |= BIT6;               // Set P2.6 direction to output

    TA0CTL = TASSEL_2 + MC_1;    // Use SMCLK, up mode
    TA0CCR0 = 1000;              // Set period
    TA0CCTL1 = OUTMOD_7;         // Set to reset/set output mode
}

void buzzer_set_period(short cycles) {
    TA0CCR0 = cycles;            // Set the period of the buzzer tone
    TA0CCR1 = cycles >> 1;       // Set duty cycle to 50%
}

void configureSwitches() {
    P2DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3); // Set P2.0, P2.1, P2.2, and P2.3 as inputs
    P2REN |=  (BIT0 | BIT1 | BIT2 | BIT3);  // Enable pull-up/pull-down resistors on P2.0, P2.1, P2.2, and P2.3
    P2OUT |=  (BIT0 | BIT1 | BIT2 | BIT3);  // Configure pull-up resistors on P2.0, P2.1, P2.2, and P2.3
    P2IE  |=  (BIT0 | BIT1 | BIT2 | BIT3);   // Enable interrupts on P2.0, P2.1, P2.2, and P2.3
    P2IES |=  (BIT0 | BIT1 | BIT2 | BIT3);  // Set interrupt to trigger on high-to-low transition (button press)
    P2IFG &= ~(BIT0 | BIT1 | BIT2 | BIT3); // Clear interrupt flags
}

void configureLEDs() {
    P1DIR |= (LED_RED | LED_GREEN); // Set LED pins to output
    P1OUT &= ~(LED_RED | LED_GREEN); // Turn off both LEDs initially
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;    // Stop the Watchdog Timer
    configureClocks();
    buzzer_init();
    configureSwitches();
    configureLEDs();

    __bis_SR_register(GIE); // Enable global interrupts
}

void stopAll() {
    mode = 0;                    // Reset mode
    buzzer_set_period(0);        // Stop buzzer
    P1OUT &= ~(LED_RED | LED_GREEN); // Turn off both LEDs
}

// Port 2 interrupt service routine for handling button presses
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
    __delay_cycles(100000);       // Debounce delay

    if (P2IFG & BIT0) {          // Check if P2.0 caused the interrupt
        mode = 1;                // Set mode for green LED and tone 1
        P1OUT = LED_GREEN;       // Turn on green LED
        buzzer_set_period(1000); // Set buzzer to 2kHz tone
        P2IFG &= ~BIT0;          // Clear interrupt flag for P2.0
    } else if (P2IFG & BIT1) {   // Check if P2.1 caused the interrupt
        mode = 2;                // Set mode for red LED and tone 2
        P1OUT = LED_RED;         // Turn on red LED
        buzzer_set_period(1500); // Set buzzer to a different tone (e.g., 1.33kHz)
        P2IFG &= ~BIT1;          // Clear interrupt flag for P2.1
    } else if (P2IFG & BIT2) {   // Check if P2.2 caused the interrupt
        mode = 3;                // Set mode for both LEDs and tone 3
        P1OUT = (LED_GREEN | LED_RED); // Turn on both LEDs
        buzzer_set_period(2000); // Set buzzer to another tone
        P2IFG &= ~BIT2;          // Clear interrupt flag for P2.2
    } else if (P2IFG & BIT3) {   // Check if P2.3 caused the interrupt
        stopAll();               // Stop all buzzing and turn off LEDs
        P2IFG &= ~BIT3;          // Clear interrupt flag for P2.3
    }
}
