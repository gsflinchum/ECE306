//------------------------------------------------------------------------------
// File Name: interupts_adc.c
// Description: This file contains the ISRs for serial communication interrupts
//
// Graham Flinchum
// October 2016
// Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

// Globals
extern volatile unsigned int usb_rx_ring_wr;
extern volatile unsigned char USB_Char_Rx[SMALL_RING_SIZE];
extern volatile unsigned int iot_rx_ring_wr;
extern volatile unsigned char IOT_Char_Rx[SMALL_RING_SIZE];
volatile char UCA0_ready_to_transmit = RESET;
volatile char UCA1_ready_to_transmit = RESET;
extern char transmit_to_pc_ok;

//------------------------------------------------------------------------------
// Function Name: USCI_A0_ISR
// Description: Interrupt for UCA0 serial communication
//
// Passed: No variable passed
// Locals: unsigned int temp
// Returned: No values returned
// Globals: extern volatile unsigned int UCA0_rx_ring_wr;
//          extern volatile char UCA0_Char_Rx[SMALL_RING_SIZE];
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void){
  unsigned int temp;
  switch(__even_in_range(UCA0IV,CASE_8)){
  case NONE: // Vector 0 - no interrupt
    break;
  case USCI_UART_UCRXIFG: // Vector 2 – RXIFG
    // code for Receive
    transmit_to_pc_ok = TRUE;
    temp = usb_rx_ring_wr;
    USB_Char_Rx[temp] = UCA0RXBUF; // RX -> USB_Char_Rx character
    if (++usb_rx_ring_wr >= (SMALL_RING_SIZE)){
      usb_rx_ring_wr = BEGINNING; // Circular buffer back to beginning
    }
    break;
  case USCI_UART_UCTXIFG: // Vector 4 – TXIFG
    // Code for Transmit
    break;
  default: break;
  }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function Name: USCI_A1_ISR
// Description: Interrupt for UCA1 serial communication
//
// Passed: No variable passed
// Locals: unsigned int temp
// Returned: No values returned
// Globals: extern volatile unsigned int UCA1_rx_ring_wr;
//          extern volatile char UCA1_Char_Rx[SMALL_RING_SIZE];
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void){
  unsigned int temp;
  switch(__even_in_range(UCA1IV,CASE_8)){
  case NONE: // Vector 0 - no interrupt
    break;
  case USCI_UART_UCRXIFG: // Vector 2 – RXIFG
    // code for Receive
    temp = iot_rx_ring_wr;
    IOT_Char_Rx[temp] = UCA1RXBUF; // RX -> USB_Char_Rx character
    outchar(IOT_Char_Rx[temp]);
    if (++iot_rx_ring_wr >= (SMALL_RING_SIZE)){
      iot_rx_ring_wr = BEGINNING; // Circular buffer back to beginning
    }
    break;
  case USCI_UART_UCTXIFG: // Vector 4 – TXIFG
    // Code for Transmit
    UCA1_ready_to_transmit = TRUE;
    break;
  default: break;
  }
}
//------------------------------------------------------------------------------
