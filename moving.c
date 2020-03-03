//------------------------------------------------------------------------------
// File Name: moving.c
// Description: This file contains functions for car movements.
//
// Graham Flinchum
// September 2016
// Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

// Global Variables
extern char start_moving;
extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;
extern char posL1;
extern char posL2;
extern char posL3;
extern char posL4;
extern unsigned int right_forward_rate;
extern unsigned int left_forward_rate;
extern unsigned int right_reverse_rate;
extern unsigned int left_reverse_rate;

//------------------------------------------------------------------------------
// Function Name: left_forward
// Description: This function turns on the car's left motor going forward.
//
// Passed: int rate
// Locals: no local variables
// Returned: no values returned
// Globals: extern unsigned int left_forward_rate;
//          extern unsigned int left_reverse_rate;
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void left_forward(int rate){
  left_reverse_rate = OFF;
  left_forward_rate = rate;       // turn on left motor forward
}

//------------------------------------------------------------------------------
// Function Name: right_forward
// Description: This function turns on the car's right motor going
//              forward.
//
// Passed: int rate
// Locals: no local variables
// Returned: no values returned
// Globals: extern unsigned int right_forward_rate;
//          extern unsigned int right_reverse_rate;
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void right_forward(int rate){
  right_reverse_rate = OFF;
  right_forward_rate = rate;    // turn on right motor forward
}


//------------------------------------------------------------------------------
// Function Name: forward
// Description: This function turns on the car's left and right motors going
//              forward.
//
// Passed: int count
// Locals: int wheel_count
// Returned: no values returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void forward(int rate){
  right_forward(rate);
  left_forward(rate);
}

//------------------------------------------------------------------------------
// Function Name: left_reverse
// Description: This function turns on the car's left motor going in reverse.
//
// Passed: int rate
// Locals: no local variables
// Returned: no values returned
// Globals: extern unsigned int left_forward_rate;
//          extern unsigned int left_reverse_rate;
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void left_reverse(int rate){
  left_forward_rate = OFF;
  left_reverse_rate = rate;       // turn on left motor in reverse
}


//------------------------------------------------------------------------------
// Function Name: right_reverse
// Description: This function turns on the car's right motor going in reverse.
//
// Passed: int rate
// Locals: no local variables
// Returned: no values returned
// Globals: extern unsigned int right_forward_rate;
//          extern unsigned int right_reverse_rate;
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void right_reverse(int rate){
  right_forward_rate = OFF;
  right_reverse_rate = rate;    // turn on right motor reverse
}

//------------------------------------------------------------------------------
// Function Name: reverse
// Description: This function turns on the car's left and right motors going
//              in reverse.
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no values returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void reverse(int rate){
  right_reverse(rate);
  left_reverse(rate);
}

//------------------------------------------------------------------------------
// Function Name: left_stop
// Description: This function turns off the car's left motor.
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no values returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void left_stop(void){
  left_reverse_rate = OFF;
  left_forward_rate = OFF;
}

//------------------------------------------------------------------------------
// Function Name: right_stop
// Description: This function turns off the car's right motor.
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no values returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void right_stop(void){
  right_reverse_rate = OFF;
  right_forward_rate = OFF;
}

//------------------------------------------------------------------------------
// Function Name: stop
// Description: This function turns off the car's left and right motors.
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no values returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void stop(void){
  right_forward_rate = OFF;
  left_forward_rate = OFF;
  right_reverse_rate = OFF;
  left_reverse_rate = OFF;
}

//------------------------------------------------------------------------------
// Function Name: circle_right
// Description: This function makes the car move in the shape of a circle by
//              turning to the left.
//
// Passed: int count
// Locals: int circles
// Returned: no values returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void circle_left(int count){
  
  return;
}

//------------------------------------------------------------------------------
// Function Name: circle_right
// Description: This function makes the car move in the shape of a circle by
//              turning to the right.
//
// Passed: int count
// Locals: int circles
// Returned: no values returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void circle_right(int count){
  
  return;
}



//------------------------------------------------------------------------------
// Function Name: spin_cwise
// Description: This function makes the car spin clockwise for count x 5 msec
//
// Passed: int time
// Locals: no local variables
// Returned: no values returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void spin_cwise(int time){
  left_forward(ONE_HUNDRED_PERCENT);
  right_reverse(ONE_HUNDRED_PERCENT);
  Five_msec_Delay(time);
  stop();
}

//------------------------------------------------------------------------------
// Function Name: spin_ccwise
// Description: This function makes the car spin counterclockwise for count x 5 msec
//
// Passed: int time
// Locals: no local variables
// Returned: no values returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: October 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
void spin_ccwise(int time){
  left_reverse(ONE_HUNDRED_PERCENT);
  right_forward(ONE_HUNDRED_PERCENT);
  Five_msec_Delay(time);
  stop();
}