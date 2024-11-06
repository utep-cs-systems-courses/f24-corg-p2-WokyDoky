#include "led.h"  // Assume your LED definitions are here

void configureClocks();
void led2();
void switch_interrupt_handler();

// global state vars that control LED state
char led_on = 0; // 0: red LED, 1: green LED

void main(void) {
  led2();
}

void led2() {
  WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

  configureClocks(); // Set up clocks

  // Set LED pins as outputs and switch pin as input
  P1DIR |= LEDS;      // Set LED pins to output
  P1OUT &= ~LEDS;     // Turn off LEDs initially

  P1DIR &= ~SWITCHES; /* set switches' bits for input */
  P1REN |= SWITCHES;  // Enable resistors for switches
  P1OUT |= SWITCHES;  // Pull-up for switches
  P1IE  |= SWITCHES;  // Enable interrupts from switches

  char p1val = P1IN;

  // Update interrupt edge selection to detect changes
  P1IFG &= ~SWITCHES; // Clear any pending switch interrupts

  __bis_SR_register(LPM0_bits + GIE);  // Enter low power mode with interrupts enabled
}

void configureClocks() {
  BCSCTL1 = CALBC1_1MHZ;  // Set DCO to 1 MHz
  DCOCTL = CALDCO_1MHZ;
}

void switch_interrupt_handler() {
  P1IES |= (P1IN & SWITCHES);  // If switch is up, detect falling edge
  P1IES &= (P1IN | ~SWITCHES); // If switch is down, detect rising edge

  // Toggle LEDs based on the current LED state
  if (led_on) {
    P1OUT |= LED_GREEN;    // Turn on green LED
    P1OUT &= ~LED_RED;     // Turn off red LED
  } else {
    P1OUT |= LED_RED;      // Turn on red LED
    P1OUT &= ~LED_GREEN;   // Turn off green LED
  }
  led_on = !led_on; // Toggle LED state for next button press
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
  if (P1IFG & SWITCHES) {        // Check if switch caused the interrupt
    P1IFG &= ~SWITCHES;        // Clear pending switch interrupts
    switch_interrupt_handler(); // Handle the switch press
  }
}
