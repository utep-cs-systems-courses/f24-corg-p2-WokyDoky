//
// Created by jdben on 11/5/2024.
//

#include "DoNotLook.h"
#include <msp430.h>
#include "led.h"

void configureClocks();
void led2();
void switch_interrupt_handler();
void enableWDTInterrupts();
void everySecond();

// global state variable to control LED state
char led_on = 0; // 0: red LED, 1: green LED

void main(void) {
    led2();
}

void led2() {
    // WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

    configureClocks(); // Set up clocks
    enableWDTInterrupts(); // Enable Watchdog Timer for LED blinking
    // Set LED pins as outputs and switch pin as input
    P1DIR |= LEDS;      // Set LED pins to output
    P1OUT &= ~LEDS;     // Turn off LEDs initially

    P1DIR &= ~SWITCHES; /* Set switch bits for input */
    P1REN |= SWITCHES;  // Enable resistors for switches
    P1OUT |= SWITCHES;  // Pull-up for switches
    P1IE  |= SWITCHES;  // Enable interrupts for switches
    P1IES |= (P1IN & SWITCHES);

    P1IFG &= ~SWITCHES; // Clear any pending switch interrupts


    __bis_SR_register(LPM0_bits + GIE);  // Enter low power mode with interrupts enabled
}

void configureClocks() {
    BCSCTL1 = CALBC1_1MHZ;  // Set DCO to 1 MHz
    DCOCTL = CALDCO_1MHZ;
}

// Enable the Watchdog Timer interrupt
void enableWDTInterrupts() {
    WDTCTL = WDT_ADLY_1000;  // Set Watchdog Timer interval to ~1 second
    IE1 |= WDTIE;            // Enable Watchdog Timer interrupt
}

// Handle button press to toggle LED state
void switch_interrupt_handler() {
    P1IFG &= ~SWITCHES; // Clear the interrupt flag

    // Toggle LED state on button press
    led_on = !led_on;   // Toggle LED selection (0: red, 1: green)
}

// Port 1 interrupt service routine for handling button presses
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IFG & SWITCHES) {         // Check if switch caused the interrupt
        switch_interrupt_handler(); // Handle the switch press
    }
}

// Blink the currently selected LED every second
void everySecond() {
    if (led_on) {
        P1OUT ^= LED_GREEN;   // Toggle green LED
        P1OUT &= ~LED_RED;    // Ensure red LED is off
    } else {
        P1OUT ^= LED_RED;     // Toggle red LED
        P1OUT &= ~LED_GREEN;  // Ensure green LED is off
    }
}

// Watchdog Timer interrupt service routine for blinking LEDs
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR() {
    everySecond(); // Call LED toggle function every second
}
