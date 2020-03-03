//------------------------------------------------------------------------------
// File Name: interupts_ports.c
// Description: This file contains ISRs for interrupts from the ports
//
// Graham Flinchum
// September 2016
// Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

// Globals
volatile char switch_pressed = RESET;
extern volatile unsigned int SW1_debounce_count;
extern volatile unsigned int SW2_debounce_count;
volatile char SW1_debouncing = RESET;
volatile char SW2_debouncing = RESET;

//------------------------------------------------------------------------------
// Function Name: switch_interrupt
// Description: This is the ISR for when one of the switches is pressed.
//              Pressing a swtich toggles LCD_BACKLITE and displays which switch
//              was pressed.
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: volatile char switch_pressed;
//          extern volatile unsigned int SW1_debounce_count;
//          extern volatile unsigned int SW2_debounce_count;
//          volatile char SW1_debouncing;
//          volatile char SW2_debouncing;
//  
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector = PORT4_VECTOR
__interrupt void switch_interrupt(void){
  switch(P4IFG){
  // Switch 1
  case SW1:
    P4IE &= ~SW1;               // SW1 interrupt disabled
    SW1_debouncing = TRUE;      // SW1 debouncing
    SW1_debounce_count = RESET; // reset debounce counter
    switch_pressed = SW1;       // SW1 pressed
    break;
  // Switch 2
  case SW2:
    P4IE &= ~SW2;               // SW2 interrupt disabled
    SW2_debouncing = TRUE;      // SW2 debouncing
    SW2_debounce_count = RESET; // reset debounce counter
    switch_pressed = SW2;       // SW2 pressed
    break;
  case BOTH:
    P4IE &= ~SW2;               // SW2 interrupt disabled
    SW2_debouncing = TRUE;      // SW2 debouncing
    SW2_debounce_count = RESET; // reset debounce counter
    switch_pressed = SW2;       // SW2 pressed
    break;
  default: break;
  }
  TA0CCTL1 &= ~CCIFG;         // clear interrupt flags
  TA0CCTL1 |= CCIE;           // CCR1 enable interrupt

}