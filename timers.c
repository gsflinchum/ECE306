//------------------------------------------------------------------------------
// File Name: timers.c
// Description: This file contains timer configurations
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
extern volatile unsigned int five_msec_count;
unsigned int right_forward_rate;
unsigned int left_forward_rate;
unsigned int right_reverse_rate;
unsigned int left_reverse_rate;

void Init_Timers(void){
//------------------------------------------------------------------------------
// Function Name: Init_Timers
// Description: Timer Configurations - calls other timer initialization functions
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: No global variables
//  
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  Init_Timer_A0(); //
  Init_Timer_A1(); //
//  Init_Timer_B0(); //
  Init_Timer_B1(); //
  Init_Timer_B2();   //  Required for provided compiled code to work
//------------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
// Function Name: Init_Timer_A0
// Description: Timer A0 initialization sets up both A0_0 and A0_1-A0_2
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: No global variables
//  
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void Init_Timer_A0(void) {
  TA0CTL = TASSEL__SMCLK;       // SMCLK source
  TA0CTL |= TACLR;              // Resets TA0R, clock divider, count direction
  TA0CTL |= MC__CONTINOUS;      // Continuous up
  TA0CTL |= ID__2;              // Divide clock by 2
  TA0CTL &= ~TAIE;              // Disable Overflow Interrupt
  TA0CTL &= ~TAIFG;             // Clear Overflow Interrupt flag
  TA0EX0 = TAIDEX_7;            // Divide clock by an additional 8
  TA0CCR0 = TA0CCR0_INTERVAL;   // CCR0
//  TA0CCTL0 |= CCIE;           // CCR0 enable interrupt
  TA0CCR1 = TA0CCR1_INTERVAL;   // CCR1
//TA0CCTL1 |= CCIE;             // CCR1 enable interrupt
  TA0CCR2 = TA0CCR2_INTERVAL;   // CCR2
//TA0CCTL2 |= CCIE;             // CCR2 enable interrupt
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function Name: Init_Timer_A1
// Description: Timer A1 initialization sets up both A1_0 and A1_1-A1_2
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: No global variables
//  
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void Init_Timer_A1(void) {
  TA1CTL = TASSEL__SMCLK;       // SMCLK source
  TA1CTL |= TACLR;              // Resets TA0R, clock divider, count direction
  TA1CTL |= MC__CONTINOUS;      // Continuous up
  TA1CTL |= ID__4;              // Divide clock by 4
  TA1CTL &= ~TAIE;              // Disable Overflow Interrupt
  TA1CTL &= ~TAIFG;             // Clear Overflow Interrupt flag
  TA1EX0 = TAIDEX_7;            // Divide clock by an additional 8
  TA1CCR0 = TA0CCR0_INTERVAL;   // CCR0
//TA1CCTL0 |= CCIE;             // CCR0 enable interrupt
  TA1CCR1 = TA1CCR1_INTERVAL;   // CCR1
//TA1CCTL1 |= CCIE;             // CCR1 enable interrupt
  TA1CCR2 = TA1CCR2_INTERVAL;   // CCR2
//TA1CCTL2 |= CCIE;             // CCR2 enable interrupt
}
//------------------------------------------------------------------------------

void Init_Timer_B1(void) {
//------------------------------------------------------------------------------
// Function Name: Init_Timer_B1
// Description: Timer B1 initialization.
//              SMCLK source, up count mode, PWM forward
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: No global variables
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
TB1CTL = TBSSEL__SMCLK;         // SMCLK
TB1CTL |= MC_1;                 // Up Mode
TB1CTL |= TBCLR;                // Clear TB1R

right_forward_rate = OFF;       // Set Right Forward Off
left_forward_rate = OFF;        // Set Left Forward Off

TB1CCR0 = WHEEL_PERIOD;         // PWM Period
TB1CCTL1 = OUTMOD_7;            // CCR1 reset/set
TB1CCR1 = right_forward_rate;   // P3.4 Right Forward PWM duty cycle
TB1CCTL2 = OUTMOD_7;            // CCR2 reset/set
TB1CCR2 = left_forward_rate;    // P3.5 Left Forward PWM duty cycle
//------------------------------------------------------------------------------
}

void Init_Timer_B2(void) {
//------------------------------------------------------------------------------
// Function Name: Init_Timer_B2
// Description: Timer B2 initialization.
//              SMCLK source, up count mode, PWM reverse
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: No global variables
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
TB2CTL = TBSSEL__SMCLK;         // SMCLK
TB2CTL |= MC_1;                 // Up Mode
TB2CTL |= TBCLR;                // Clear TB2R

right_reverse_rate = OFF;       // Set Right Forward Off
left_reverse_rate = OFF;        // Set Right Reverse Off

TB2CCR0 = WHEEL_PERIOD;         // PWM Period
TB2CCTL1 = OUTMOD_7;            // CCR1 reset/set
TB2CCR1 = right_reverse_rate;   // P3.6 Right Reverse PWM duty cycle
TB2CCTL2 = OUTMOD_7;            // CCR2 reset/set
TB2CCR2 = left_reverse_rate;    // P3.7 LEft Reverse PWM duty cycle
//------------------------------------------------------------------------------
}

/*void Timer_code(void){
//------------------------------------------------------------------------------
// Function Name: Timer_Code
// Description: Timer A0 interrupt code
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile unsigned int Time_Sequence;
//          extern volatile char one_time;
//          extern volatile unsigned int five_msec_count;
//  
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  Time_Sequence++;
  one_time = TRUE;
  if (five_msec_count < ONE_THOUSAND){
    five_msec_count++;
  }
//------------------------------------------------------------------------------
}*/

void Five_msec_Delay(unsigned int fivemsec){
//------------------------------------------------------------------------------
// Function Name: Five_msec_Delay
// Description: Each count passed is at least x times 5 milliseconds
//
// Passed: unsigned int fivemsec
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile unsigned int five_msec_count;
//  
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  five_msec_count = RESET;
  while(fivemsec > five_msec_count);
//------------------------------------------------------------------------------
}