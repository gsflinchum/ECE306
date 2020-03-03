//------------------------------------------------------------------------------
// File Name: init.c
// Description: This file contains initial condition
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

extern char display_line_1[CHAR_PER_LINE];
extern char display_line_2[CHAR_PER_LINE];
extern char display_line_3[CHAR_PER_LINE];
extern char display_line_4[CHAR_PER_LINE];
extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;
extern unsigned int right_forward_rate;
extern unsigned int left_forward_rate;
extern unsigned int right_reverse_rate;
extern unsigned int left_reverse_rate;
volatile unsigned int usb_rx_ring_wr;
unsigned int usb_rx_ring_rd;
volatile unsigned char USB_Char_Rx[SMALL_RING_SIZE];
volatile unsigned int iot_rx_ring_wr;
unsigned int iot_rx_ring_rd;
volatile unsigned char IOT_Char_Rx[SMALL_RING_SIZE];

void Init_Conditions(void){
//------------------------------------------------------------------------------
// Function Name: Init_Conditions
// Description: Initializations Configurations
//              Interrupts are disabled by default, enable them.
//              Sets display line pointers to point to the arrays holding the 
//              info to be displayed.
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern char display_line_1[CHAR_PER_LINE];
//          extern char display_line_2[CHAR_PER_LINE];
//          extern char display_line_3[CHAR_PER_LINE];
//          extern char display_line_4[CHAR_PER_LINE];
//          extern char *display_1;
//          extern char *display_2;
//          extern char *display_3;
//          extern char *display_4;
//  
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  enable_interrupts();
  display_1 = &display_line_1[FIRST_CHAR];
  display_2 = &display_line_2[FIRST_CHAR];
  display_3 = &display_line_3[FIRST_CHAR];
  display_4 = &display_line_4[FIRST_CHAR];

//------------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
// Function Name: Init_PWM
// Description: Initializes PWM interrupts.
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern unsigned int right_forward_rate;
//          extern unsigned int left_forward_rate;
//          extern unsigned int right_reverse_rate;
//          extern unsigned int left_reverse_rate;
//  
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void Init_PWM(void){
  TB1CCTL0 &= ~CCIFG;           // Enable interrupts for motors forward
  TB1CCTL0 |= CCIE;
  TB1CCTL1 &= ~CCIFG;
  TB1CCTL1 |= CCIE; 
  TB1CCTL2 &= ~CCIFG;
  TB1CCTL2 |= CCIE;
  
  TB2CCTL0 &= ~CCIFG;           // Enable interrupts for motors reverse
  TB2CCTL0 |= CCIE;
  TB2CCTL1 &= ~CCIFG;
  TB2CCTL1 |= CCIE; 
  TB2CCTL2 &= ~CCIFG;
  TB2CCTL2 |= CCIE;
}

//----------------------------------------------------------------------------
void Init_Serial_UCA0(int baud_rate){
  int i;
  for(i=RESET; i<SMALL_RING_SIZE; i++){
    USB_Char_Rx[i] = CLEAR;         // USB Rx Buffer
  }
  usb_rx_ring_wr = BEGINNING;
  usb_rx_ring_rd = BEGINNING;
//  for(i=RESET; i<LARGE_RING_SIZE; i++){ // May not use this
//    UCA0_Char_Tx[i] = CLEAR;         // USB Tx Buffer
//  }
//  UCA0_tx_ring_wr = BEGINNING;
//  UCA0_tx_ring_rd = BEGINNING;
  // Configure UART 0
  UCA0CTLW0 = RESET;              // Use word register
  UCA0CTLW0 |= UCSSEL__SMCLK;     // Set SMCLK as fBRCLK
  UCA0CTLW0 |= UCSWRST;           // Set Software reset enable
  switch(baud_rate){
  case BAUD_9600:
    // 9,600 Baud Rate
    // 1. Calculate N = fBRCLK / Baudrate
    //      N = SMCLK / 9,600 => 8,000,000 / 9,600 = 833.3333333
    //      if N > 16 continue with step 3, otherwise with step 2
    // 2. OS16 = 0, UCBRx = INT(N)
    //    continue with step 4
    // 3. OS16 = 1,
    //      UCx     = INT(N/16),
    //              = INT(N/16) = 833.333/16 => 52
    //      UCFx    = INT([(N/16) – INT(N/16)] × 16)
    //              = ([833.333/16-INT(833.333/16)]*16)
    //              = (52.08333333-52)*16
    //              = 0.083*16 = 1
    // 4. UCSx is found by looking up the fractional part of N (= N-INT(N)) in table Table 18-4
    //    Decimal of SMCLK / 8,000,000 / 9,600 = 833.3333333 => 0.333 yields 0x49 [Table]
    // 5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed
    //    TX error (%) RX error (%)
    //    BRCLK     Baudrate     UCOS16  UCBRx   UCFx    UCSx    neg     pos     neg     pos
    //    8000000   9600         1       52      1       0x49    -0.08   0.04    -0.10   0.14
    // UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
    // UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
    UCA0BRW = BRW_9600; // 9,600 Baud
    UCA0MCTLW = MCTLW_9600 ;
    break;
  case BAUD_115200:
    // 115,200 Baud Rate
    // 1. Calculate N = fBRCLK / Baudrate
    //      N = SMCLK / 115,200 => 8,000,000 / 115,200 = 69.4444
    //      if N > 16 continue with step 3, otherwise with step 2
    // 2. OS16 = 0, UCBRx = INT(N)
    //    continue with step 4
    // 3. OS16 = 1,
    //      UCx     = INT(N/16),
    //              = INT(N/16) = 69.4444/16 => 4
    //      UCFx    = INT([(N/16) – INT(N/16)] × 16)
    //              = ([69.4444/16-INT(69.4444/16)]*16)
    //              = (4.34 - 4)*16
    //              = 0.34*16 = 5
    // 4. UCSx is found by looking up the fractional part of N (= N-INT(N)) in table Table 18-4
    //    Decimal of SMCLK / 8,000,000 / 115,200 = 69.4444 => .4444 yields 0x55 [Table]
    // 5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed
    //    TX error (%) RX error (%)
    //    BRCLK     Baudrate     UCOS16  UCBRx   UCFx    UCSx    neg     pos     neg     pos
    //    8000000   115,200         1      4      5      0x55    -0.08   0.04    -0.10   0.14
    // UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
    // UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
    UCA0BRW = BRW_115200; // 115,200 Baud
    UCA0MCTLW = MCTLW_115200 ;
    break;
default: break;
  }
UCA0CTL1 &= ~UCSWRST; // Release from reset
UCA0IE |= UCRXIE; // Enable RX interrupt
}

//----------------------------------------------------------------------------
void Init_Serial_UCA1(int baud_rate){
  int i;
  for(i=RESET; i<SMALL_RING_SIZE; i++){
    IOT_Char_Rx[i] = CLEAR;         // USB Rx Buffer
  }
  iot_rx_ring_wr = BEGINNING;
  iot_rx_ring_rd = BEGINNING;
//  for(i=RESET; i<LARGE_RING_SIZE; i++){ // May not use this
//  UCA1_Char_Tx[i] = CLEAR;         // USB Tx Buffer
//  }
//  UCA1_tx_ring_wr = BEGINNING;
//  UCA1_tx_ring_rd = BEGINNING;
  // Configure UART 0
  UCA1CTLW0 = RESET;              // Use word register
  UCA1CTLW0 |= UCSSEL__SMCLK;     // Set SMCLK as fBRCLK
  UCA1CTLW0 |= UCSWRST;           // Set Software reset enable
  switch(baud_rate){
  case BAUD_9600:
    // 9,600 Baud Rate
    // 1. Calculate N = fBRCLK / Baudrate
    //      N = SMCLK / 9,600 => 8,000,000 / 9,600 = 833.3333333
    //      if N > 16 continue with step 3, otherwise with step 2
    // 2. OS16 = 0, UCBRx = INT(N)
    //    continue with step 4
    // 3. OS16 = 1,
    //      UCx     = INT(N/16),
    //              = INT(N/16) = 833.333/16 => 52
    //      UCFx    = INT([(N/16) – INT(N/16)] × 16)
    //              = ([833.333/16-INT(833.333/16)]*16)
    //              = (52.08333333-52)*16
    //              = 0.083*16 = 1
    // 4. UCSx is found by looking up the fractional part of N (= N-INT(N)) in table Table 18-4
    //    Decimal of SMCLK / 8,000,000 / 9,600 = 833.3333333 => 0.333 yields 0x49 [Table]
    // 5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed
    //    TX error (%) RX error (%)
    //    BRCLK     Baudrate     UCOS16  UCBRx   UCFx    UCSx    neg     pos     neg     pos
    //    8000000   9600         1       52      1       0x49    -0.08   0.04    -0.10   0.14
    // UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
    // UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
    UCA1BRW = BRW_9600; // 9,600 Baud
    UCA1MCTLW = MCTLW_9600 ;
    break;
  case BAUD_115200:
    // 115,200 Baud Rate
    // 1. Calculate N = fBRCLK / Baudrate
    //      N = SMCLK / 115,200 => 8,000,000 / 115,200 = 69.4444
    //      if N > 16 continue with step 3, otherwise with step 2
    // 2. OS16 = 0, UCBRx = INT(N)
    //    continue with step 4
    // 3. OS16 = 1,
    //      UCx     = INT(N/16),
    //              = INT(N/16) = 69.4444/16 => 4
    //      UCFx    = INT([(N/16) – INT(N/16)] × 16)
    //              = ([69.4444/16-INT(69.4444/16)]*16)
    //              = (4.34 - 4)*16
    //              = 0.34*16 = 5
    // 4. UCSx is found by looking up the fractional part of N (= N-INT(N)) in table Table 18-4
    //    Decimal of SMCLK / 8,000,000 / 115,200 = 69.4444 => .4444 yields 0x55 [Table]
    // 5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed
    //    TX error (%) RX error (%)
    //    BRCLK     Baudrate     UCOS16  UCBRx   UCFx    UCSx    neg     pos     neg     pos
    //    8000000   115,200         1      4      5      0x55    -0.08   0.04    -0.10   0.14
    // UCA0MCTLW = UCSx concatenate UCFx concatenate UCOS16;
    // UCA0MCTLW = 0x49 concatenate 1 concatenate 1;
    UCA1BRW = BRW_115200; // 115,200 Baud
    UCA1MCTLW = MCTLW_115200 ;
    break;
  default: break;
  }
  UCA1CTL1 &= ~UCSWRST; // Release from reset
  UCA1IE |= UCRXIE; // Enable RX interrupt
}