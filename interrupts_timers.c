//------------------------------------------------------------------------------
// File Name: interupts_timer.c
// Description: This file contains ISRs for timer interrupts
//              Timer A0_2 is on a 5 msec count
//              Timer A0_1 debounces the switch for 200 msec
//
// Graham Flinchum
// September 2016
// Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

// Globals
extern volatile unsigned int five_msec_count;
volatile unsigned int Time_Sequence;
volatile unsigned int fifty_msec_count;
volatile unsigned int SW1_debounce_count = RESET;
volatile unsigned int SW2_debounce_count = RESET;
extern volatile char SW1_debouncing;
extern volatile char SW2_debouncing;
extern unsigned int right_forward_rate;
extern unsigned int left_forward_rate;
extern unsigned int right_reverse_rate;
extern unsigned int left_reverse_rate;
volatile char update_detectors;
volatile char check_movement = RESET;
volatile char update_display = RESET;
volatile unsigned char usb_character_time_out;
volatile unsigned char iot_character_time_out;

//------------------------------------------------------------------------------
// Function Name: Timer0_A0_ISR
// Description: TimerA0 0 Interrupt handler
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile int five_msec_count;
//  
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void){
  
  // INTERRUPT TEST CODE: BLINKS LCD_BACKLITE EVERY SECOND
//  fifty_msec_count++;                    // increment five_msec_count
//    if(fifty_msec_count == TOGGLE_LCD){
//  P1OUT ^= LCD_BACKLITE;              // toggle LCD every 500msec
//  fifty_msec_count = RESET;            // reset count
//}
//    TA0CCTL0 &= ~CCIE;          // disable interrupt

  check_movement = TRUE;
  
  TA0CCR0 += TA0CCR0_INTERVAL;        // Add Offset to TACCR0
}
//------------------------------------------------------------------------------
// Function Name: Timer0_A1_ISR
// Description: TimerA0 1-2, Overflow Interrupt Vector (TAIV) handler
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile unsigned int SW1_debounce_count;
//          extern volatile unsigned int SW2_debounce_count;
//          volatile char SW1_debouncing;
//          volatile char SW2_debouncing;
//  
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void){
  switch(__even_in_range(TA0IV,TA0IV_TAIFG)){
  case TA0IV_NONE: break;               // No interrupt
  case TA0IV_TACCR1:                    // CCR1 interrupt
    if(SW1_debouncing){
      if(SW1_debounce_count++ >= DEBOUNCED){ // debounce threshold is reached
        P4IFG &= ~SW1;                  // clear flags
        P4IE |= SW1;                    // SW1 interrupt enabled
        SW1_debouncing = FALSE;         // no longer debouncing 
        if(!(SW2_debouncing)){
          TA0CCTL1 &= ~CCIE;            // CCR1 disable interrupt
        }
      }
    }
    if(SW2_debouncing){
      if(SW2_debounce_count++ >= DEBOUNCED){ // debounce threshold is reached
        P4IFG &= ~SW2;                  // clear flags
        P4IE |= SW2;                    // SW2 interrupt enabled
        SW2_debouncing = FALSE;         // no longer debouncing
        if(!(SW1_debouncing)){
          TA0CCTL1 &= ~CCIE;            // CCR1 disable interrupt
        }
      }
    }
    TA0CCR1 += TA0CCR1_INTERVAL;        // Add Offset to TACCR1
    break;
  case TA0IV_TACCR2:                    // CCR2 not used
    Time_Sequence++;
    five_msec_count++;
    if(usb_character_time_out <= ONE_SECOND){
      usb_character_time_out++;
    }
    if(iot_character_time_out <= ONE_SECOND){
      iot_character_time_out++;
    }
      TA0CCR2 += TA0CCR2_INTERVAL;      // Add Offset to TACCR2
    break;
  case TA0IV_TAIFG:                     // overflow
//    ...... Add What you need happen in the interrupt ......
      break;
  default: break;
  }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function Name: Timer1_A0_ISR
// Description: TimerA1 0 Interrupt handler
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile int five_msec_count;
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void){
  // timerA1-0 interrupt service routine used to update display every quarter
  // second with detector values
  
  update_detectors = TRUE;
  update_display = TRUE;
  TA1CCR0 += TA1CCR0_INTERVAL;          // Add Offset to TACCR0 (250 msec)
}
//------------------------------------------------------------------------------
// Function Name: Timer1_A1_ISR
// Description: TimerA1 1-2, Overflow Interrupt Vector (TAIV) handler
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
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void){
  switch(__even_in_range(TA1IV,TA1IV_TAIFG)){
  case  TA1IV_NONE: break;                    // No interrupt
  case  TA1IV_TACCR1:                           // CCR1 used for ???
    // *** add your code here *** 
    TA1CCR1 += TA1CCR1_INTERVAL;     // Add Offset to TACCR1
    break;
  case  TA1IV_TACCR2:                           // CCR2 used for ???
    // *** add your code here *** 
    TA1CCR2 += TA1CCR2_INTERVAL;     // Add Offset to TACCR2
    break;
  case TA1IV_3: break;                    // reserved
  case TA1IV_4: break;                    // reserved
  case TA1IV_5: break;                    // reserved
  case TA1IV_6: break;                    // reserved
  case TA1IV_TAIFG:                           // overflow used for ???
    // *** add your code here *** 
    break;
  default: break; 
  }
}
// timerA1
//=============================================================================

//------------------------------------------------------------------------------
// Function Name: Timer0_B0_ISR
// Description: TimerB0 0 Interrupt handler
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile int five_msec_count;
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
  // timerB0-0 interrupt service routine used ???
  // *** add your code here *** 
  TB0CCR0 += TB0CCR0_INTERVAL;         // Add Offset to TACCR0
}

//------------------------------------------------------------------------------
// Function Name: Timer0_B1_ISR
// Description: TimerB0 1-2, Overflow Interrupt Vector (TAIV) handler
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

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
  switch(__even_in_range(TB0IV,TB0IV_TBIFG)){
  case TB0IV_NONE: break;                    // No interrupt
  case TB0IV_TBCCR1:                           // CCR1 used for ???
    // *** add your code here *** 
    TB0CCR1 += TB0CCR1_INTERVAL;     // Add Offset to TB0CCR1
    break;
  case TB0IV_TBCCR2:                           // CCR2 used for ???
    // *** add your code here *** 
    TB0CCR2 += TB0CCR2_INTERVAL;     // Add Offset to TB0CCR2
    break;
  case TB0IV_3: break;                    // reserved
  case TB0IV_4: break;                    // reserved
  case TB0IV_5: break;                    // reserved
  case TB0IV_6: break;                    // reserved
  case TB0IV_TBIFG:                           // overflow used for ???
    // *** add your code here *** 
    break;
  default: break; 
  }
}
// timerB0
//==============================================================================

//------------------------------------------------------------------------------
// Function Name: Timer1_B0_ISR
// Description: TimerB1 0 Interrupt handler
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile int five_msec_count;
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void){
  
  //TB1CCR0 += WHEEL_PERIOD;          // Add Offset to TB1CCR0
}
//------------------------------------------------------------------------------
// Function Name: Timer1_B1_ISR
// Description: TimerB1 1-2, Overflow Interrupt Vector (TBIV) handler
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile unsigned int SW1_debounce_count;
//          extern volatile unsigned int SW2_debounce_count;
//          volatile char SW1_debouncing;
//          volatile char SW2_debouncing;
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector = TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void){
  switch(__even_in_range(TB1IV,TB1IV_TBIFG)){
  case TB1IV_NONE: break;                    // No interrupt
  case TB1IV_TBCCR1:                           // CCR1 used for ???
    if(TB1CCR1 != right_forward_rate){
      TB1CCR1 = right_forward_rate;     // Add Offset to TB1CCR1
    }
    break;
  case TB1IV_TBCCR2:                           // CCR2 used for ???
    if(TB1CCR2 != left_forward_rate){
      TB1CCR2 = left_forward_rate;     // Add Offset to TB1CCR2
    }
    break;
  case TB1IV_3: break;                    // reserved
  case TB1IV_4: break;                    // reserved
  case TB1IV_5: break;                    // reserved
  case TB1IV_6: break;                    // reserved
  case TB1IV_TBIFG:                       // overflow used for ???
    // *** add your code here *** 
    break;
  default: break; 
  }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function Name: Timer2_B0_ISR
// Description: TimerB2 0 Interrupt handler
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile int five_msec_count;
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector = TIMER2_B0_VECTOR
__interrupt void Timer2_B0_ISR(void){
  
//  TB2CCR0 += WHEEL_PERIOD;        // Add Offset to TB2CCR0
}
//------------------------------------------------------------------------------
// Function Name: Timer2_B1_ISR
// Description: TimerB2 1-2, Overflow Interrupt Vector (TBIV) handler
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile unsigned int SW1_debounce_count;
//          extern volatile unsigned int SW2_debounce_count;
//          volatile char SW1_debouncing;
//          volatile char SW2_debouncing;
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector = TIMER2_B1_VECTOR
__interrupt void TIMER2_B1_ISR(void){
  switch(__even_in_range(TB2IV,TB2IV_TBIFG)){
  case TB2IV_NONE: break;            // No interrupt
  case TB2IV_TBCCR1:                 // CCR1 used for ???
    if(TB2CCR1 != right_reverse_rate){
    TB2CCR1 = right_reverse_rate;     // Add Offset to TBCCR1
    }
    break;
  case TB2IV_TBCCR2:                 // CCR2 used for ???
    if(TB2CCR2 != left_reverse_rate){
      TB2CCR2 = left_reverse_rate;     // Add Offset to TBCCR2
    }
    break;
  case TB2IV_3: break;               // reserved
  case TB2IV_4: break;               // reserved
  case TB2IV_5: break;               // reserved
  case TB2IV_6: break;               // reserved
  case TB2IV_TBIFG:                  // overflow used for ???
    // *** add your code here *** 
    break;
  default: break; 
  }
}
// timerB2
//=============================================================================
