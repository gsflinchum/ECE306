//------------------------------------------------------------------------------
// File Name: interupts_adc.c
// Description: This file contains the ISR for ADC conversion
//
// Graham Flinchum
// September 2016
// Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

// Globals
volatile unsigned int ADC_Thumb;
volatile unsigned int ADC_left;
volatile unsigned int ADC_right;
volatile unsigned int previous_ADC_right;
volatile unsigned int previous_ADC_left;
volatile unsigned int oldest_ADC_right;
volatile unsigned int oldest_ADC_left;
char channel;

//------------------------------------------------------------------------------
// Function Name: ADC10_VECTOR
// Description: ADC10 interrupt service routine
//              ADC_Right_Detector; // A00 ADC10INCH_0 - P1.0
//              ADC_Left_Detector; // A01 ADC10INCH_1 - P1.1
//              ADC_Thumb; // A03 ADC10INCH_3 - P1.3
//              ADC_Temp; // A10 ADC10INCH_10 – Temperature REF module
//              ADC_Bat; // A11 ADC10INCH_11 - Internal
//
// Passed: No variable passed
// Locals: No local variables
// Returned: No values returned
// Globals: No global varia
//  
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void){
  switch(__even_in_range(ADC10IV,ADC10IV_ADC10IFG)) {
  case ADC10IV_NONE: break; // No interrupt
  case ADC10IV_ADC10OVIFG: break; // conversion result overflow
  case ADC10IV_ADC10TOVIFG: break; // conversion time overflow
  case ADC10IV_ADC10HIIFG: break; // ADC10HI
  case ADC10IV_ADC10LOIFG: break; // ADC10LO
  case ADC10IV_ADC10INIFG: break; // ADC10IN
  case ADC10IV_ADC10IFG:
    ADC10CTL0 &= ~ADC10ENC; // Turn off the ENC bit of the ADC10CTL0
    switch (channel){
    case ADC10INCH_0:
      channel = ADC10INCH_1;    // change channel variable to A1
      ADC10MCTL0 = ADC10INCH_1; // Next channel A1
      oldest_ADC_left = previous_ADC_left;
      previous_ADC_left = ADC_left;
      ADC_left = ADC10MEM0; // Current Channel result for A0
      break;
    case ADC10INCH_1:
      channel = ADC10INCH_3;    // change channel variable to A3
      ADC10MCTL0 = ADC10INCH_3; // Next channel A3
      oldest_ADC_right = previous_ADC_right;
      previous_ADC_right = ADC_right;
      ADC_right = ADC10MEM0; // Current Channel result for A1
      break;
    case ADC10INCH_3:
      channel = ADC10INCH_0;    //change channel variable to A0
      ADC10MCTL0 = ADC10INCH_0; // Next channel A0
      ADC_Thumb = ADC10MEM0; // Current Channel result for A3
      break;
    default: break;
    }
    ADC10CTL0 |= ADC10ENC + ADC10SC; // Sampling and conversion start
    break;
  default: break;
  }
}
//------------------------------------------------------------------------------