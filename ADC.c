//------------------------------------------------------------------------------
// File Name: ADC.c
// Description: This file contains ADC configurations and functions
//
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

// Globals
char adc_char[ARRAY_SIZE_6];
extern volatile unsigned int ADC_Thumb;
extern volatile unsigned int ADC_left;
extern volatile unsigned int ADC_right;
extern volatile unsigned int previous_ADC_right;
extern volatile unsigned int previous_ADC_left;
extern volatile unsigned int oldest_ADC_right;
extern volatile unsigned int oldest_ADC_left;
unsigned int left_detect;
unsigned int right_detect;
unsigned int last_left_detect;
unsigned int last_right_detect;
unsigned int oldest_left_detect;
unsigned int oldest_right_detect;
extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;
extern char posL1;
extern char posL2;
extern char posL3;
extern char posL4;
unsigned int moving_left = RESET;
unsigned int moving_right = RESET;
extern volatile char update_detectors;
unsigned int ADC_left_black_avg;
unsigned int ADC_right_black_avg;
unsigned int ADC_left_white_avg;
unsigned int ADC_right_white_avg;
char ADC_left_higher;
char ADC_right_higher;
unsigned int ADC_black_difference;
unsigned int ADC_white_difference;
extern char black_configured;
extern char white_configured;
extern volatile char check_movement;
extern volatile char switch_pressed;
char configuring = FALSE;
char configured = FALSE;
char line_side = RESET;
int black_min;
int white_max;
int gray;
int reverse_count;
int left;
int right;

//------------------------------------------------------------------------------
// Function Name: Init_ADC
// Description: Configure ADC10_B
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
void Init_ADC(void){
  ADC10CTL0 = RESET_STATE;      // Clear ADC10CTL0
  ADC10CTL0 |= ADC10SHT_2;      // 16 ADC clocks
  ADC10CTL0 &= ~ADC10MSC;       // Single Sequence
  ADC10CTL0 |= ADC10ON;         // ADC ON - Core Enabled
  
  ADC10CTL1 = RESET_STATE;      // Clear ADC10CTL1
  ADC10CTL1 |= ADC10SHS_0;      // ADC10SC bit
  ADC10CTL1 |= ADC10SHP;        // SAMPCON signal sourced from sampling timer
  ADC10CTL1 &= ~ADC10ISSH;      // The sample-input signal is not inverted.
  ADC10CTL1 |= ADC10DIV_0;      // ADC10_B clock divider – Divide by 1.
  ADC10CTL1 |= ADC10SSEL_0;     // MODCLK
  ADC10CTL1 |= ADC10CONSEQ_0;   // Single-channel, single-conversion
  
  ADC10CTL2 = RESET_STATE;      // Clear ADC10CTL2
  ADC10CTL2 |= ADC10DIV_0;      // Pre-divide by 1
  ADC10CTL2 |= ADC10RES;        // 10-bit resolution
  ADC10CTL2 &= ~ADC10DF;        // Binary unsigned
  ADC10CTL2 &= ~ADC10SR;        // supports up to approximately 200 ksps
  
  ADC10MCTL0 = RESET_STATE;     // Clear ADC10MCTL0
  ADC10MCTL0 |= ADC10SREF_0;    // V(R+) = AVCC and V(R-) = AVSS
  ADC10MCTL0 |= ADC10INCH_0;    // Channel A0 Right Detector
  ADC10IE |= ADC10IE0;          // Enable ADC conversion complete interrupt
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function Name: ADC_Process
// Description: Start ADC Conversion
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
void ADC_Process(void){
  while (ADC10CTL1 & BUSY); // Wait if ADC10 core is active
  ADC10CTL0 |= ADC10ENC + ADC10SC; // Sampling and conversion start
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function Name: HEXtoBCD
// Description: Hex to BCD Conversion
//              Convert a Hex number to a BCD for display on an LCD or monitor
//
// Passed: int hex_value
// Locals: int value
// Returned: No values returned
// Globals: char adc_char[4]
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void HEXtoBCD(int hex_value){
  int value;
  adc_char[TEN_THOUSANDS] = '0';
  if (hex_value > TEN_THOUSAND){
    hex_value = hex_value - TEN_THOUSAND;
    adc_char[TEN_THOUSANDS] = '1';
  }
  value = RESET;
  adc_char[THOUSANDS] = '0';
  if (hex_value > ONE_THOUSAND){
    hex_value = hex_value - ONE_THOUSAND;
    adc_char[THOUSANDS] = '1';
  }
  value = RESET;
  if(hex_value < ONE_HUNDRED){
    adc_char[HUNDREDS] = '0';
  }
  while (hex_value > NINETY_NINE){
    hex_value = hex_value - ONE_HUNDRED;
    value++;
    adc_char[HUNDREDS] = ASCII_0 + value;
  }
  value = RESET;
  if(hex_value < TEN_DECIMAL){
    adc_char[TENS] = '0';
  }
  while (hex_value > NINE_DECIMAL){
    hex_value = hex_value - TEN_DECIMAL;
    value++;
    adc_char[TENS] = ASCII_0 + value;
  }
  adc_char[ONES] = ASCII_0 + hex_value;
  adc_char[NULL_CHAR] = '\0';
}
//------------------------------------------------------------------------------
  
//------------------------------------------------------------------------------
// Function Name: Display_Detectors
// Description: Calls function to convert the values from the left and right
//              detectors to binary and displays them on the LCD.
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile unsigned int ADC_left;
//          extern volatile unsigned int ADC_right;
//          extern char adc_char[FOUR_DIGITS];
//          extern char *display_2;
//          extern char *display_3;
//          extern char posL2;
//          extern char posL3;
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void Display_Detectors(void){
  update_detectors = FALSE;
  HEXtoBCD(ADC_left);
  display_2 = adc_char;
  posL2 = INDENT_3;
  lcd_out(display_2, LCD_HOME_L2, posL2);
  HEXtoBCD(ADC_right);
  display_3 = adc_char;
  posL3 = INDENT_3;
  lcd_out(display_3, LCD_HOME_L3, posL3);
}

//------------------------------------------------------------------------------
// Function Name: detect_movement
// Description: Checks to see if car is moving left->right or right->left
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: extern volatile unsigned int ADC_left;
//          extern volatile unsigned int ADC_right;
//          extern char adc_char[FOUR_DIGITS];
//          extern char *display_2;
//          extern char *display_3;
//          extern char posL2;
//          extern char posL3;
//          extern volatile char check_movement;
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void detect_movement(void){
  check_movement = RESET;
  oldest_left_detect = oldest_ADC_left;
  oldest_right_detect = oldest_ADC_right;
  left_detect = ADC_left;
  right_detect = ADC_right;
  last_right_detect = previous_ADC_right;
  last_left_detect = previous_ADC_left;
  
  if((left_detect > white_max) && (right_detect < white_max)){
    reverse_count = RESET;
    right = RESET;
    right_forward(FORTY_PERCENT);
    left_reverse(TWENTY_PERCENT);
    line_side = LEFT;
    left++;
    if(left > STUCK){
      left = RESET;
      forward(FORTY_PERCENT);
    }
    return;
  }
  else if((left_detect < white_max) && (right_detect < white_max)){
    left = RESET;
    right = RESET;
    reverse(FORTY_PERCENT);
    reverse_count++;
    if(reverse_count > STUCK){
      reverse_count = RESET;
      forward(FORTY_PERCENT);
    }
    return;
  }
  
  else if((right_detect > white_max) && (left_detect < white_max)){
    left = RESET;
    reverse_count = RESET;
    right_reverse(TWENTY_PERCENT);
    left_forward(THIRTY_PERCENT);
    line_side = RIGHT;
    right++;
    if(right > STUCK){
      right = RESET;
      forward(FORTY_PERCENT);
    }
    return;
  }
  else if((right_detect > white_max) &&(left_detect > white_max)){
    right_forward(FORTY_PERCENT);
    left_forward(THIRTY_PERCENT);
    return;
  }
  return;
}

//------------------------------------------------------------------------------
// Function Name: Black_Configuration
// Description: Configures average ADC values for when the detectors are reading
//              black in the current environment
//
// Passed: No variable passed
// Locals: int count
// Returned: No values returned
// Globals: unsigned int ADC_left_black_avg;
//          unsigned int ADC_right_black_avg;
//          char ADC_left_higher;
//          char ADC_right_higher;
//          unsigned int ADC_difference;
//          extern char black_configured;
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void Black_Configuration(void){
  ADC_left_higher = RESET;
  ADC_right_higher = RESET;
  ADC_left_black_avg = RESET;
  ADC_right_black_avg = RESET;
  for(int count = RESET; count < AVG_COUNT; count++){
    ADC_left_black_avg += ADC_left;
    ADC_right_black_avg += ADC_right;
  }
  ADC_left_black_avg >>= DIVIDE_BY_SIXTEEN;
  ADC_right_black_avg >>= DIVIDE_BY_SIXTEEN;
  if(ADC_left_black_avg > ADC_right_black_avg){
    ADC_left_higher = TRUE;
    black_min = ADC_right_black_avg - LEEWAY;
    ADC_black_difference = ADC_left_black_avg - ADC_right_black_avg;
  }
  if(ADC_right_black_avg > ADC_left_black_avg){
    ADC_right_higher = TRUE;
    black_min = ADC_left_black_avg - LEEWAY;
    ADC_black_difference = ADC_right_black_avg - ADC_left_black_avg;
  }
  black_configured = TRUE;
}

//------------------------------------------------------------------------------
// Function Name: White_Configuration
// Description: Configures average ADC values for when the detectors are reading
//              white in the current environment
//
// Passed: No variable passed
// Locals: int count
// Returned: No values returned
// Globals: unsigned int ADC_left_white_avg;
//          unsigned int ADC_right_white_avg;
//          char ADC_left_higher;
//          char ADC_right_higher;
//          unsigned int ADC_difference;
//          extern char white_configured;
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void White_Configuration(void){
  ADC_left_higher = RESET;
  ADC_right_higher = RESET;
  ADC_left_white_avg = RESET;
  ADC_right_white_avg = RESET;
  for(int count = RESET; count < AVG_COUNT; count++){
    ADC_left_white_avg += ADC_left;
    ADC_right_white_avg += ADC_right;
  }
  ADC_left_white_avg >>= DIVIDE_BY_SIXTEEN;
  ADC_right_white_avg >>= DIVIDE_BY_SIXTEEN;
  if(ADC_left_white_avg > ADC_right_white_avg){
    ADC_left_higher = TRUE;
    white_max = ADC_left_white_avg + LEEWAY;
    ADC_white_difference = ADC_left_white_avg - ADC_right_white_avg;
  }
  if(ADC_right_white_avg > ADC_left_white_avg){
    ADC_right_higher = TRUE;
    white_max = ADC_right_white_avg + LEEWAY;
    ADC_white_difference = ADC_right_white_avg - ADC_left_white_avg;
  }
  white_configured = TRUE;
}


//------------------------------------------------------------------------------
// Function Name: Configure
// Description: Configures avg ADC values for when the detectors are reading 
//              in the current environment.  Displays a menu which is controlled
//              by the thumb wheel and has two options, selectable by pressing
//              SW 2
//
// Passed: No variable passed
// Locals: int count
// Returned: No values returned
// Globals: extern char *display_1;
//          extern char *display_2;
//          extern char *display_3;
//          extern char posL1;
//          extern char posL2;
//          extern char posL3;
//          extern volatile unsigned int ADC_Thumb;
//          extern volatile char update_detectors;
//          extern volatile char switch_pressed;
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void Configure(void){
  configuring = TRUE;
  ClrDisplay();
  while(!(black_configured) || !(white_configured)){
    if(ADC_Thumb > MENU_DIVIDE){
      display_1 = "Black";
      posL1 = INDENT_2;
      display_2 = "Config.";
      posL2 = INDENT_1;
      if(black_configured){
        HEXtoBCD(ADC_left_black_avg);
        display_2 = adc_char;
        posL2 = INDENT_3;
        lcd_out(display_2, LCD_HOME_L2, posL2);
      }
      else{
        display_3 = "Press SW2";
        posL3 = INDENT_0;
      }
    }
    if(ADC_Thumb < MENU_DIVIDE){
      display_1 = "White";
      posL1 = INDENT_2;
      display_2 = "Config.";
      posL2 = INDENT_1;
      if(white_configured){
        HEXtoBCD(ADC_left_white_avg);
        display_2 = adc_char;
        posL2 = INDENT_3;
        lcd_out(display_2, LCD_HOME_L2, posL2);
      }
      else{
        display_3 = "Press SW2";
        posL3 = INDENT_0;
      }
    }
    if(switch_pressed){                 // check for switch press
      Switches_Process();
    }
    if(update_detectors){               // update display every 250 msec
      update_detectors = RESET;
      Display_Process();
    }
  }
  ClrDisplay();
  if(P3IN & IR_LED){
    display_1 = "LED ON ";
  }
  if(!(P3IN & IR_LED)){
    display_1 = "LED OFF";
  }
  posL1 = INDENT_2;
  lcd_out(display_1, LCD_HOME_L1, posL1);
  
  configured = TRUE;
  gray = (black_min + white_max);
  gray >>= DIVIDE_BY_TWO;
}