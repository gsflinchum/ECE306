//------------------------------------------------------------------------------
// File Name: main.c
// Description: This file contains the Main Routine - "While" Operating System
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

// Global Variables

volatile unsigned int five_msec_count;
extern char display_line_1[CHAR_PER_LINE];
extern char display_line_2[CHAR_PER_LINE];
extern char display_line_3[CHAR_PER_LINE];
extern char display_line_4[CHAR_PER_LINE];
extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;
char posL1;
char posL2;
char posL3;
char posL4;
char size_count;
char big;
extern volatile char switch_pressed;
extern unsigned int right_forward_rate;
extern unsigned int left_forward_rate;
extern unsigned int right_reverse_rate;
extern unsigned int left_reverse_rate;
extern volatile unsigned int ADC_Thumb;
extern volatile unsigned int ADC_left;
extern volatile unsigned int ADC_right;
extern char adc_char[ARRAY_SIZE_6];
extern volatile char update_detectors;
char black_configured = RESET;
char white_configured = RESET;
extern volatile char check_movement;
extern unsigned int ADC_left_black_avg;
extern unsigned int ADC_right_black_avg;
char state = WIFI;
extern volatile char update_display;
int temp = RESET;
extern volatile char UCA0_ready_to_transmit;
extern volatile char UCA1_ready_to_transmit;
extern volatile unsigned int usb_rx_ring_wr;
extern unsigned int usb_rx_ring_rd;
extern volatile unsigned char USB_Char_Rx[SMALL_RING_SIZE];
extern volatile unsigned int iot_rx_ring_wr;
extern unsigned int iot_rx_ring_rd;
extern volatile unsigned char IOT_Char_Rx[SMALL_RING_SIZE];
char received_data[ARRAY_SIZE_6];
char sent_data[ARRAY_SIZE_6];
int index = RESET;
char ready_transmit = RESET;
unsigned int received_number = RESET;
unsigned int sent_number = RESET;
char first_bit = TRUE;
char transmit_to_pc_ok = FALSE;
extern volatile unsigned int Time_Sequence;
char asynch_cmd_received = FALSE;
extern volatile unsigned char current_iot_character;
char wind_num[ARRAY_SIZE_3];
extern int black_min;

void main(void){
//------------------------------------------------------------------------------
// Function Name: Main
//-+-*]****** Description: This is the main routine for the program. Execution of code starts here.
//              The operating system is Back Ground Fore Ground.
//
// Passed: no vaiables passed
// Locals: no local variables
// Returned: no variables returned
// Globals: volatile unsigned int five_msec_count;
//          extern char display_line_1[CHAR_PER_LINE];
//          extern char display_line_2[CHAR_PER_LINE];
//          extern char display_line_3[CHAR_PER_LINE];
//          extern char display_line_4[CHAR_PER_LINE];
//          extern char *display_1;
//          extern char *display_2;
//          extern char *display_3;
//          extern char *display_4;
//          char posL1;
//          char posL2;
//          char posL3;
//          char posL4;
//          char size_count;
//          char big;
//          extern volatile char switch_pressed;
//          extern char adc_char[FOUR_DIGITS];
//          extern unsigned int right_forward_rate;
//          extern unsigned int left_forward_rate;
//          extern unsigned int right_reverse_rate;
//          extern unsigned int left_reverse_rate;
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------

  Init_Ports();                             // Initialize Ports
  Init_Clocks();                            // Initialize Clock System
  Init_Conditions();

  Init_Timers();                            // Initialize Timers
  
  TA0CCTL2 &= ~CCIFG;           // clear interrupt flags
  TA0CCTL2 |= CCIE;             // CCR2 enable interrupt for Time_Sequence
  
  Five_msec_Delay(QUARTER_SECOND);          // 250 msec delay for the clock to settle

  Init_LCD();                               // Initialize LCD
  Init_ADC();
  Init_PWM();
  
  TA1CCTL0 &= ~CCIFG;           // clear interrupt flags
  TA1CCTL0 |= CCIE;             // A1 CCR0 enable interrupt
  
  ADC_Process();
  
  TA0CCTL0 &= ~CCIFG;           // clear interrupt flags
  TA0CCTL0 |= CCIE;             // A0 CCR0 enable interrupt for detecting movement
  
  
  Init_Serial_UCA0(BAUD_9600);  // initialize serial ports for 9600 baud
  Init_Serial_UCA1(BAUD_9600);
  
  Five_msec_Delay(FIVE_MSEC);
  PJOUT |= IOT_RESET;

  P3OUT |= IR_LED;
  
//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------  
  while(ALWAYS) {                          // Can the Operating system run
    
    switch(state){
    case WIFI:
      iot_menu();
      break;
    case FINDING_LINE:
      display_4 = "INTERCEPT ";
      posL4 = INDENT_0;
      lcd_out(display_4, LCD_HOME_L4, posL4);
      spin_ccwise(THIRTY_DEGREES);
      right_forward(FIFTY_PERCENT);
      left_forward(FORTY_PERCENT);
      Five_msec_Delay(FIVE_SECONDS);
      stop();
      spin_cwise(NINETY_DEGREES);
      spin_cwise(FORTY_FIVE_DEGREES);
      right_forward(FIFTY_PERCENT);
      left_forward(FORTY_PERCENT);
      Five_msec_Delay(FOUR_SECONDS);
      while((ADC_left < black_min) || (ADC_right < black_min)){}
      state = PAUSING;
      stop();
      break;
    case PAUSING:
      Five_msec_Delay(ONE_SECOND);
      state = FOLLOWING;
      display_4 = "FOLLOWING";
      posL4 = INDENT_0;
      lcd_out(display_4, LCD_HOME_L4, posL4);
      break;
    case FOLLOWING:
      if(check_movement){                // check car's movement every 10msec
        detect_movement();
      }
      iot_menu();
      break;
    case EXITING:
      stop();
      forward(ONE_HUNDRED_PERCENT);
      Five_msec_Delay(TWO_SECONDS);
      stop();
      state = WIFI;
      break;
    default:break;
    }
    
    if(switch_pressed){
      Switches_Process();               // Check for switch state change
    }
    if(update_detectors){               // Check to update display
      Display_Detectors();
    }
    if(update_display){
      update_display = RESET;
      Display_Process();
    }
  }
}
//------------------------------------------------------------------------------