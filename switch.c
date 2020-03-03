//------------------------------------------------------------------------------
// File Name: switch.c
// Description: This file contains switch assignments
//
// Graham Flinchum
// September 2016
// Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;
extern char posL1;
extern char posL2;
extern char posL3;
extern char posL4;
extern volatile char switch_pressed;
extern volatile unsigned int ADC_Thumb;
extern char configuring;
extern char configured;
extern char state;
extern volatile char update_display;
extern int index;
extern char received_data[ARRAY_SIZE_9];
extern char ready_transmit;
char baud_9600[ARRAY_SIZE_6] = "9,600";
char baud_115200[ARRAY_SIZE_8] = "115,200";
extern unsigned int sent_number;
char baud_rate = BAUD_9600;

void Switches_Process(void){
//------------------------------------------------------------------------------
// Function Name: Switches_Proccess
// Description: 
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no values returned
// Globals: extern char *display_1;
//          extern char *display_2;
//          extern char *display_3;
//          extern char *display_4;
//          char posL1;
//          char posL2;
//          char posL3;
//          char posL4;
//          extern volatile char switch_pressed;
//          extern volatile unsigned int ADC_Thumb;
//          extern char configuring;
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  switch(switch_pressed){
  case SW1:
    switch_pressed = FALSE;
    P3OUT ^= IR_LED;
    if(P3IN & IR_LED){
      display_1 = "LED ON ";
    }
    if(!(P3IN & IR_LED)){
      display_1 = "LED OFF";
    }
    posL1 = INDENT_2;
    lcd_out(display_1, LCD_HOME_L1, posL1);
    break;
  case SW2:
    switch_pressed = FALSE;
    if(configured == FALSE){
      if(configuring == FALSE){
        Configure();
      }
      if(ADC_Thumb < MENU_DIVIDE){
        White_Configuration();
      }
      if(ADC_Thumb > MENU_DIVIDE){
        Black_Configuration();
      }
    }
    else if(!state){
      state = WIFI;
      PJOUT &= ~IOT_RESET;          // Hardware reset for the module
      Five_msec_Delay(HUNDRED_MSEC); // Delay 100 msec
      PJOUT |= IOT_RESET;           // Bring module back online
    }
    break;
  default: break;
  }
//------------------------------------------------------------------------------
}

void Display_Process(void){
//------------------------------------------------------------------------------
// Function Name: Display_Proccess
// Description: This function is the process for displaying the character arrays
//              on the LCD.  It clears the LCD and displays arrays.
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no values returned
// Globals: extern char *display_1;
//          extern char *display_2;
//          extern char *display_3;
//          extern char *display_4;
//          char posL1;
//          char posL2;
//          char posL3;
//          char posL4;
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  ClrDisplay();
  lcd_out(display_1, LCD_HOME_L1, posL1);
  lcd_out(display_2, LCD_HOME_L2, posL2);
  lcd_out(display_3, LCD_HOME_L3, posL3);
  lcd_out(display_4, LCD_HOME_L4, posL4);
}