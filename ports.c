//------------------------------------------------------------------------------
// File Name: ports.c
// Description: This file contains port initializations
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------

#include  "macros.h"
#include  "functions.h"
#include  "msp430.h"

void Init_Ports(void){
//------------------------------------------------------------------------------
// Function Name: Init_Port
// Description: This calls the rest of the port initialization functions
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
  Init_Port1();
  Init_Port2();
  Init_Port3();
  Init_Port4();
  Init_PortJ();
}

void Init_Port1(void){
//------------------------------------------------------------------------------
// Function Name: Init_Port1
// Description: Initializes Port 1
//
// V_DETECT_R           (0x01)  // Voltage from Right Detector
// V_DETECT_L           (0x02)  // Voltage from Left Detector
// LCD_BACKLITE         (0x04)  // Control Signal for LCD_BACKLIGHT
// V_THUMB              (0x08)  // Voltage from Thumb Wheel
// SPI_CS_LCD           (0x10)  // LCD Chip Select
// RESET_LCD            (0x20)  // LCD Reset
// SIMO_B               (0x40)  // SPI mode - slave in/master out of USCI_B0
// SOMI_B               (0x80)  // SPI mode - slave out/master in of USCI_B0
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
  P1SEL0 = CLEAR_REGISTER;     // P1 set default as I/0
  P1SEL1 = CLEAR_REGISTER;     // P1 set default as I/0
  P1DIR = INPUT;               // Set P1 direction default to input
  
  P1SEL0 |= V_DETECT_R;         // V_DETECT_R selected
  P1SEL1 |= V_DETECT_R;         // V_DETECT_R selected
  P1SEL0 |= V_DETECT_L;         // V_DETECT_L selected
  P1SEL1 |= V_DETECT_L;         // V_DETECT_L selected
  
  P1SEL0 &= ~LCD_BACKLITE;      // LCD_BACKLITE GPI/O selected
  P1SEL1 &= ~LCD_BACKLITE;      // LCD_BACKLITE GPI/O selected
  P1OUT &= ~LCD_BACKLITE ;       // LCD_BACKLITE Port Pin set low
  P1DIR |= LCD_BACKLITE ;       // Set LCD_BACKLITE direction to output
  
  P1SEL0 |= V_THUMB;            // V_THUMB selected
  P1SEL1 |= V_THUMB;            // V_THUMB selected
  
  P1SEL0 &= ~SPI_CS_LCD;        // SPI_CS_LCD GPI/O selected
  P1SEL1 &= ~SPI_CS_LCD;        // SPI_CS_LCD GPI/O selected
  P1OUT |= SPI_CS_LCD;          // SPI_CS_LCD Port Pin set high
  P1DIR |= SPI_CS_LCD;          // Set SPI_CS_LCD output direction
  
  P1SEL0 &= ~RESET_LCD;         // RESET_LCD GPI/O selected
  P1SEL1 &= ~RESET_LCD;         // RESET_LCD GPI/O selected
  P1OUT &= ~RESET_LCD;          // RESET_LCD Port Pin set low
  P1DIR |= RESET_LCD;           // Set RESET_LCD output direction
  
  P1SEL0 &= ~SIMO_B;            // SIMO_B selected
  P1SEL1 |= SIMO_B;             // SIMO_B selected
  P1DIR |= SIMO_B;              // SIMO_B set to Output
  
  P1SEL0 &= ~SOMI_B;            // SOMI_B is used on the LCD
  P1SEL1 |= SOMI_B;             // SOMI_B is used on the LCD
  P1OUT |= SOMI_B;              // SOMI_B Port Pin set for Pull-up
  P1DIR &= ~SOMI_B;             // SOMI_B set to Input
  P1REN |= SOMI_B;              // Enable pullup resistor
//------------------------------------------------------------------------------
}

void Init_Port2(void){
//------------------------------------------------------------------------------
// Function Name: Init_Port2
// Description: Initializes Port 2
//
// Port 2 Pins
// USB_TXD              (0x01) //
// USB_RXD              (0x02) //
// SPI_SCK              (0x04) //
// P2_3                 (0x08) //
// P2_4                 (0x10) //
// CPU_TXD              (0x20) //
// CPU_RXD              (0x40) //
// P2_7                 (0x80) //
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
  P2SEL0 = CLEAR_REGISTER;      // P2 set default as I/O
  P2SEL1 = CLEAR_REGISTER;
  P2DIR = INPUT;                // P2 direction set default as input
  
  P2SEL0 &= ~USB_TXD;           // Set pin function to UCA0TXD/UCA0SIMO
  P2SEL1 |= USB_TXD;
  P2OUT &= ~USB_TXD;            // pin set low
  
  P2SEL0 &= ~USB_RXD;           // Set pin function to UCA0RXD/UCA0SOMI
  P2SEL1 |= USB_RXD;
  P2OUT &= ~USB_RXD;            // pin set low
  P2REN &= ~USB_RXD;            // pulldown resistor enabled
  
  P2SEL0 &= ~SPI_SCK;           // Set pin function to UCB0CLK
  P2SEL1 |= SPI_SCK;
  P2OUT |= SPI_SCK;             // pin set high
  
  P2SEL0 &= ~P2_3;              // Set P2_3 to I/O
  P2SEL1 &= ~P2_3;
  P2DIR &= ~P2_3;               // Set direction to input
  P2OUT &= ~P2_3;               // pin set low
  P2REN &= ~P2_3;               // pulldown resistor enabled
  
  P2SEL0 &= ~P2_4;              // Set P2_4 to I/O
  P2SEL1 &= ~P2_4;
  P2DIR &= ~P2_4;               // Set direction to input
  P2OUT &= ~P2_4;               // pin set low
  P2REN &= ~P2_4;               //pulldown resistor enabled
  
  P2SEL0 &= ~CPU_TXD;           // Set pin function to UCA1TXD/UCA1SIMO
  P2SEL1 |= CPU_TXD;
  P2OUT &= ~CPU_TXD;            // pin set low
  
  P2SEL0 &= ~CPU_RXD;           // Set pin function to UCA1RXD/UCA1SOMI
  P2SEL1 |= CPU_RXD;
  P2OUT &= ~CPU_RXD;            // pin set low
  P2REN &= ~CPU_RXD;            // pulldown resistor enabled

  
  P2SEL0 &= ~P2_7;              // Set P2_7 to I/O
  P2SEL1 &= ~P2_7;
  P2DIR &= ~P2_7;               // Set direction to input
  P2OUT &= ~P2_7;               // pin set low
  P2REN &= ~P2_7;               // pulldown resistor enabled
}

void Init_Port3(void){
//------------------------------------------------------------------------------
// Function Name: Init_Port3
// Description: Initializes Port 3
//
// Port 3 Pins
// X                    (0x01) // 
// Y                    (0x02) // 
// P3Z                  (0x04) // 
// IR_LED               (0x08) // 
// R_FORWARD            (0x10) // 
// L_FORWARD            (0x20) // 
// R_REVERSE            (0x40) // 
// L_REVERSE            (0x80) // 
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
  P3SEL0 = CLEAR_REGISTER;       // P3 set default as I/O
  P3SEL1 = CLEAR_REGISTER;       // P3 set default as I/O
  P3DIR = INPUT;                 // Set P3 direction default as input
  
  P3SEL0 &= ~X;                 // X set as I/O
  P3SEL1 &= ~X;
  P3DIR &= ~X;                  // X direction set to input
  P3OUT &= ~X;                  // pin set low
  P3REN &= ~X;                  // pulldown resistor enabled
  
  P3SEL0 &= ~Y;                 // Y set as I/O
  P3SEL1 &= ~Y;
  P3DIR &= ~Y;                  // Y direction set to input
  P3OUT &= ~Y;                  // pin set low
  P3REN &= ~Y;                  // pulldown resistor enabled
  
  P3SEL0 &= ~P3Z;                 // Z set as I/O
  P3SEL1 &= ~P3Z;
  P3DIR &= ~P3Z;                  // Z direction set to input
  P3OUT &= ~P3Z;                  // pin set low
  P3REN &= ~P3Z;                  // pulldown resistor enabled
  
  P3SEL0 &= ~IR_LED;            // IR_LED set as I/O
  P3SEL1 &= ~IR_LED;
  P3DIR |= IR_LED;              // IR_LED direction set to output
  P3OUT &= ~IR_LED;             // pin set low
  
  P3SEL0 |= R_FORWARD;         // R_FORWARD set as TB1.1
  P3SEL1 &= ~R_FORWARD;
  P3DIR |= R_FORWARD;           // R_FORWARD direction set to output
  
  P3SEL0 |= L_FORWARD;         // L_FORWARD set as TB1.2
  P3SEL1 &= ~L_FORWARD;
  P3DIR |= L_FORWARD;           // L_FORWARD direction set to output
  
  P3SEL0 |= R_REVERSE;         // R_REVERSE set as TB2.1
  P3SEL1 &= ~R_REVERSE;
  P3DIR |= R_REVERSE;           // R_REVERSE direction set to output
  
  P3SEL0 |= L_REVERSE;         // L_REVERSE set as TB2.2
  P3SEL1 &= ~L_REVERSE;
  P3DIR |= L_REVERSE;           // L_REVERSE direction set to output
}

void Init_Port4(void){
//------------------------------------------------------------------------------
// Function Name: Init_Port4
// Description: Initializes Port 4
//
// Port 4 Pins
// SW1                  (0x01)  // Switch 1
// SW2                  (0x02)  // Switch 2
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
  P4SEL0 = CLEAR_REGISTER;      // P4 set default as I/O
  P4SEL1 = CLEAR_REGISTER;      // P4 set default as I/O
  P4DIR = INPUT;                // Set P4 direction default to input
  P4OUT = LOW;                  // Set P4 pins low
// SW1
  P4SEL0 &= ~SW1;               // SW1 set as I/O
  P4SEL1 &= ~SW1;               // SW1 set as I/O
  P4OUT |= SW1;                 // Configure pullup resistor
  P4DIR &= ~SW1;                // Direction = input
  P4REN |= SW1;                 // Enable pullup resistor
  P4IES |= SW1;                 // Sw1 Hi/Lo edge interrupt
  P4IFG &= ~SW1;                // IFG SW1 cleared
  P4IE |= SW1;                  // SW1 interrupt enabled
// SW2
  P4SEL0 &= ~SW2;               // SW2 set as I/O
  P4SEL1 &= ~SW2;               // SW2 set as I/O
  P4OUT |= SW2;                 // Configure pullup resistor
  P4DIR &= ~SW2;                // Direction = input
  P4REN |= SW2;                 // Enable pullup resistor
  P4IES |= SW2;                 // SW2 Hi/Lo edge interrupt
  P4IFG &= ~SW2;                // IFG SW2 cleared
  P4IE |= SW2;                  // SW2 interrupt enabled
//------------------------------------------------------------------------------
}

void Init_PortJ(void){
//------------------------------------------------------------------------------
// Function Name: Init_PortJ
// Description: Initializes Port J
//
// Port J Pins
// IOT_WAKEUP           (0x01) // 
// IOT_FACTORY          (0x02) // 
// IOT_STA_MINIAP       (0x04) // 
// IOT_RESET            (0x08) // 
// XINR                 (0x10) //
// XOUTR                (0x20) //
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
  PJSEL0 = CLEAR_REGISTER;       // PJ set default as I/0
  PJSEL1 = CLEAR_REGISTER;       // PJ set default as I/0
  PJDIR = INPUT;                 // Set PJ direction to input

  PJSEL0 &= ~IOT_WAKEUP;        // Set IOT_WAKEUP as I/O
  PJSEL1 &= ~IOT_WAKEUP;        // Set IOT_WAKEUP as I/O
  PJOUT &= ~IOT_WAKEUP;         // IOT_WAKEUP set low
  PJDIR |= IOT_WAKEUP;          // Set IOT_WAKEUP direction to output
  
  PJSEL0 &= ~IOT_FACTORY;       // Set IOT_FACTORY as I/O
  PJSEL1 &= ~IOT_FACTORY;       // Set IOT_FACTORY as I/O
  PJOUT &= ~IOT_FACTORY;        // IOT_FACTORY set low
  PJDIR |= IOT_FACTORY;         // Set PJ Pin 2 direction to output
  
  PJSEL0 &= ~IOT_STA_MINIAP;    // Set IOT_STA_MINIAP as I/O
  PJSEL1 &= ~IOT_STA_MINIAP;    // Set IOT_STA_MINIAP as I/O
  PJOUT |= IOT_STA_MINIAP;     // IOT_STA_MINIAP set high
  PJDIR |= IOT_STA_MINIAP;      // Set IOT_STA_MINIAP direction to output
  
  PJSEL0 &= ~IOT_RESET;         // Set IOT_RESET as I/O
  PJSEL1 &= ~IOT_RESET;         // Set IOT_RESET as I/O
  PJOUT &= ~IOT_RESET;          // IOT_RESET set low
  PJDIR |= IOT_RESET;           // Set PJ Pin 4 direction to output

// XT1 Setup
// PJSEL0 |= XINR;
// PJSEL0 |= XOUTR;
//------------------------------------------------------------------------------

}