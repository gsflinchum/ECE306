//------------------------------------------------------------------------------
// File name: IOT.c
// Description: This file contains functions for reading commands from the IOT
//              device and sending commands to the device.
//
//
// Graham Flinchum
// November 2016
// Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

//-------------------------------------------------------------------------------
// Globals
extern char menu_selection;
extern volatile unsigned char USB_Char_Rx[SMALL_RING_SIZE];// USB Character
extern volatile unsigned int usb_rx_ring_wr;
extern unsigned int usb_rx_ring_rd;
extern volatile unsigned char IOT_Char_Rx[SMALL_RING_SIZE];// IOT Character
extern volatile unsigned int iot_rx_ring_wr;
extern unsigned int iot_rx_ring_rd;
volatile unsigned char current_usb_character;
extern volatile unsigned char usb_character_time_out;
volatile unsigned char current_iot_character;
extern volatile unsigned char iot_character_time_out;
char ok_to_transmit_to_pc;
char get[ARRAY_SIZE_11] = {"AT+S.GCFG="};
char set[ARRAY_SIZE_11] = {"AT+S.SCFG="};
char macaddr[ARRAY_SIZE_16] = {"nv_wifi_macaddr"};
char module_baud[ARRAY_SIZE_20] = {"console1_speed,9600"};
char save[ARRAY_SIZE_5] = {"AT&W"};
char reset_cmd[ARRAY_SIZE_10] = {"AT+CFUN=1"};
char set_ssid_cmd[ARRAY_SIZE_18] = {"AT+S.SSIDTXT=ncsu"};
char get_ssid_cmd[ARRAY_SIZE_13] = {"AT+S.SSIDTXT"};
char set_host_cmd[ARRAY_SIZE_25] = {"ip_hostname,ECE-306_15_B"};
char get_host_cmd[ARRAY_SIZE_12] = {"ip_hostname"};
char set_ntwrk_priv[ARRAY_SIZE_17] = {"wifi_priv_mode,0"};
char get_ntwrk_priv[ARRAY_SIZE_15] = {"wifi_priv_mode"};
char set_ntwrk[ARRAY_SIZE_12] = {"wifi_mode,1"};
char get_ntwrk[ARRAY_SIZE_10] = {"wifi_mode"};
char check_vars[ARRAY_SIZE_9] = {"AT+S.STS"};
char ipaddr[ARRAY_SIZE_19] = {"AT+S.STS=ip_ipaddr"};
char beacon_loss_thresh[ARRAY_SIZE_28] = {"wifi_beacon_loss_thresh,300"};
char idle_timeout[ARRAY_SIZE_25] = {"wifi_ap_idle_timeout,600"};

char ip_address[ARRAY_SIZE_16];
char ip_first_line[ARRAY_SIZE_9];
char ip_second_line[ARRAY_SIZE_8];
extern char wind_num[ARRAY_SIZE_3];
char command[ARRAY_SIZE_9];

extern char asynch_cmd_received;
extern volatile char update_display;
extern char state;
extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;
extern char posL1;
extern char posL2;
extern char posL3;
extern char posL4;

void menu(void){
//------------------------------------------------------------------------------
// Function Name: menu
// Description: This function checks to see if a '.' has been typed, signaling
//              the beginning of a command.
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no values returned
// Globals: volatile unsigned char current_iot_character;
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
  //------------------------------------------------------------------------------
  grab_iot_character();
  if (current_iot_character != NO_CHARACTER){
    if (current_iot_character == PERIOD){
      outchar(current_iot_character);
      if (wait_for_iot_character()){
        print_CR();
        return;
      }
      menu_program();
    }
  }
//------------------------------------------------------------------------------
}

void grab_usb_character(void){
//------------------------------------------------------------------------------
// Function Name: grab_usb_character
// Description: This function checks to see if a new character has been written
//              to the receive buffer from the PC and, if so, copies it over to
//              current_usb_character.
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no values returned
// Globals: volatile unsigned char current_usb_character;
//          extern volatile unsigned int usb_rx_ring_wr;
//          extern unsigned int usb_rx_ring_rd;
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  if (usb_rx_ring_rd == usb_rx_ring_wr){    // If no character is avalable Return 0x00
    current_usb_character = NO_CHARACTER;
  }else{
    UCA0IE &= ~UCRXIE;                      // Disable USART0 RX interrupt
    current_usb_character = USB_Char_Rx[usb_rx_ring_rd];
    increment_usb_rx_ring();
    UCA0IE |= UCRXIE;                       // Enable USART0 RX interrupt
  }
//------------------------------------------------------------------------------
}

int wait_for_usb_character(void){
//------------------------------------------------------------------------------
// Function Name: wait_for_usb_character
// Description: This function waits for a character to be entered by the PC.
//              It will time out after 1 second.
//              This requires character_time_out be incremented in a timer.
//              In the timer associated, it is a 5msec timer and the code is:
//              if(usb_character_time_out <= 50){
//                usb_character_time_out++;
//              }
//
// Passed: no variables passed
// Locals: no local variables
// Returned: int defined as TRUE or FALSE
// Globals: volatile unsigned char usb_character_time_out;
//          extern volatile unsigned int usb_rx_ring_wr;
//          extern unsigned int usb_rx_ring_rd;
//          volatile unsigned char current_usb_character;
//          extern volatile unsigned char IOT_Char_Rx[SMALL_RING_SIZE];
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  usb_character_time_out = RESET;
  while(usb_character_time_out <= ONE_SECOND){
    if (usb_rx_ring_rd != usb_rx_ring_wr){  // Character has arrived
      UCA0IE &= ~UCRXIE;                    // Disable USART0 RX interrupt
      current_usb_character = USB_Char_Rx[usb_rx_ring_rd];
      increment_usb_rx_ring();
      UCA0IE |= UCRXIE;                     // Enable USART0 RX interrupt
      return FALSE;
    }
    for (int i = TIME_FOR_FLAG; i > RESET; i--);         // Time for flag to set
  }
  return TRUE;
//------------------------------------------------------------------------------
}

void menu_program(void){
//------------------------------------------------------------------------------
// Function Name: menu_program
// Description: This function views the second letter of the command from the
//              terminal window to determine which function to perform.
//              .. returns "I'm here"
//              .M retrieves the MAC address
//              .F sets the IOT serial port baud rate to 115200
//              .S sets the IOT serial port baud rate to 9600
//              .B sets the baud rate of the IOT module to 9600
//              .R resets the IOT module (includes hardware reset)
//              .Z sets SSID
//              .X gets SSID
//              .C sets host name
//              .H gets host name
//              .N sets network privacy mode
//              .P gets network privacy setting
//              .Q sets network type
//              .W gets network type
//              .E saves current settings on IOT module
//              .V checks all variables
//              .I retrieves only the ip address
//              .L adjusts the number of beacon losses required to drop wifi
//              .T adjusts the amount of idle time necessary to drop wifi
//
// Passed: no passed variables
// Locals: no local variables
// Returned: no variables returned
// Globals: char ok_to_transmit_to_pc;
//          char get[ARRAY_SIZE_11] = {"AT+S.GCFG="};
//          char set[ARRAY_SIZE_11] = {"AT+S.SCFG="};
//          char macaddr[ARRAY_SIZE_16] = {"nv_wifi_macaddr"};
//          char module_baud[ARRAY_SIZE_20] = {"console1_speed,9600"};
//          char save[ARRAY_SIZE_5] = {"AT&W"};
//          char reset_cmd[ARRAY_SIZE_10] = {"AT+CFUN=1"};
//          char set_ssid_cmd[ARRAY_SIZE_18] = {"AT+S.SSIDTXT=ncsu"};
//          char get_ssid_cmd[ARRAY_SIZE_13] = {"AT+S.SSIDTXT"};
//          char set_host_cmd[ARRAY_SIZE_25] = {"ip_hostname,ECE-306_15_B"};
//          char get_host_cmd[ARRAY_SIZE_12] = {"ip_hostname"};
//          char set_ntwrk_priv[ARRAY_SIZE_17] = {"wifi_priv_mode,0"};
//          char get_ntwrk_priv[ARRAY_SIZE_15] = {"wifi_priv_mode"};
//          char set_ntwrk[ARRAY_SIZE_12] = {"wifi_mode,1"};
//          char get_ntwrk[ARRAY_SIZE_10] = {"wifi_mode"};
//          char check_vars[ARRAY_SIZE_9] = {"AT+S.STS"};
//          char ipaddr[ARRAY_SIZE_19] = {"AT+S.STS=ip_ipaddr"};
//          char beacon_loss_thresh[ARRAY_SIZE_28] = {"wifi_beacon_loss_thresh,300"};
//          char idle_timeout[ARRAY_SIZE_25] = {"wifi_ap_idle_timeout,600"};
//          volatile unsigned char current_iot_character;
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  outchar(current_iot_character);
  outchar(SPACE);
  switch (current_iot_character) {
  case PERIOD:  // .
    outchar('I');outchar(APOSTROPHE);outchar('m');outchar(SPACE);outchar('H');outchar('e');outchar('r');outchar('e');
    ok_to_transmit_to_pc = TRUE;
    break;
    // get mac address
  case ASCII_M: // M
  case ASCII_m: // m
    get_command();
    send_command(macaddr,ARRAY_SIZE_16);
    break;
    // set IOT serial ports to 115200
  case ASCII_F: // F
  case ASCII_f: // f
    Init_Serial_UCA1(BAUD_115200);
    display_4 = "115,200";
    posL4 = INDENT_1;
    update_display = TRUE;
    break;
    // set IOT serial ports to 9600
  case ASCII_S: // S
  case ASCII_s: // s
    Init_Serial_UCA1(BAUD_9600);
    display_4 = "9,600";
    posL4 = INDENT_2;
    update_display = TRUE;
    break;
    // change baud rate of IOT module to 9600
  case ASCII_B: // B
  case ASCII_b: // b
    set_command();
    send_command(module_baud,ARRAY_SIZE_20);
    Five_msec_Delay(TWO_SECONDS);
    break;
    // reset IOT module
  case ASCII_R: // R
  case ASCII_r: // r
    send_command(reset_cmd,ARRAY_SIZE_10);
    PJOUT &= ~IOT_RESET;          // Hardware reset for the module
    Five_msec_Delay(HUNDRED_MSEC); // Delay 100 msec
    PJOUT |= IOT_RESET;           // Bring module back online
    send_command(ipaddr, ARRAY_SIZE_19);
    display_ip();
    break;
    // set SSID
  case ASCII_Z: // Z
  case ASCII_z: // z
    send_command(set_ssid_cmd,ARRAY_SIZE_18);
    break;
    // get SSID
  case ASCII_X: // X
  case ASCII_x: // x
    send_command(get_ssid_cmd,ARRAY_SIZE_13);
    break;
    // Set host name
  case ASCII_C: // C
  case ASCII_c: // c
    set_command();
    send_command(set_host_cmd,ARRAY_SIZE_25);
    break;
    // get host name
  case ASCII_H: // H
  case ASCII_h: // h
    get_command();
    send_command(get_host_cmd,ARRAY_SIZE_12);
    break;  
    // set network privacy mode
  case ASCII_N: // N
  case ASCII_n: // n
    set_command();
    send_command(set_ntwrk_priv,ARRAY_SIZE_17);
    break;
    // get network privacy mode
  case ASCII_P: // P
  case ASCII_p: // p
    get_command();
    send_command(get_ntwrk_priv,ARRAY_SIZE_15);
    break;
    // set network mode
  case ASCII_Q: // Q
  case ASCII_q: // q
    set_command();
    send_command(set_ntwrk,ARRAY_SIZE_12);
    break;
    // get network mode
  case ASCII_W: // W
  case ASCII_w: // w
    get_command();
    send_command(get_ntwrk,ARRAY_SIZE_10);
    break;
    // save module settings
  case ASCII_E: // E
  case ASCII_e: // e
    send_command(save,ARRAY_SIZE_5);
    Five_msec_Delay(ONE_SECOND);
    break;
    // check variables
  case ASCII_V: // V
  case ASCII_v: // v
    send_command(check_vars, ARRAY_SIZE_9);
    break;
    // fetch IP address
  case ASCII_I: // I
  case ASCII_i: // i
    send_command(ipaddr, ARRAY_SIZE_19);
    get_ip();
    display_ip();
    break;
    // change wifi_beacon_loss_thresh to a higher threshold
  case ASCII_L: // L
  case ASCII_l: // l
    set_command();
    send_command(beacon_loss_thresh,ARRAY_SIZE_28);
    break;
    // change wifi_ap_idle_timeout to a longer period
  case ASCII_T: // T
  case ASCII_t: // t
    set_command();
    send_command(idle_timeout,ARRAY_SIZE_25);
    break;
  } // end of character determination Switch
  current_iot_character = NO_CHARACTER;
  print_CR();
//------------------------------------------------------------------------------
}

void print_CR(void){
//------------------------------------------------------------------------------
// Function Name: print_CR
// Description: Prints a carriage return character and line feed character to
//              the PC.
//
// Passed: no passed variables
// Locals: no local variables
// Returned: no variables returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
// This is a print statement prints a carriage return and line feed.
  outchar(CARRIAGE_RETURN);
  outchar(LINE_FEED);
//------------------------------------------------------------------------------
}

void outchar(char character){
//------------------------------------------------------------------------------
// Function Name: outchar
// Description: Transmits a character to the PC
//
// Passed: char character
// Locals: no local variables
// Returned: no variables returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
    while (!(UCA0IFG & UCTXIFG));           // USCI_A0 TX buffer ready?
    UCA0TXBUF = character;
//------------------------------------------------------------------------------
}

void increment_usb_rx_ring(void){
//------------------------------------------------------------------------------
// Function Name: increment_usb_rx_ring
// Description: This function increments the variable usb_rx_ring_rd that is used
//              to read from the PC receiving buffer.  It is a ring buffer so 
//              it loops back to the beginning once it reaches the end.
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no variables returned
// Globals: extern unsigned int usb_rx_ring_rd;
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  usb_rx_ring_rd++;
  if (usb_rx_ring_rd >= (SMALL_RING_SIZE)){
    usb_rx_ring_rd = BEGINNING;
  }
//------------------------------------------------------------------------------
}

// ====================================
// IOT

void iot_menu(void){
//------------------------------------------------------------------------------
// Function Name: iot_menu
// Description: This function checks to see if a character has been sent through
//              the iot device.
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no values returned
// Globals: volatile unsigned char current_iot_character;
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  grab_iot_character();
  switch(current_iot_character){
  case PLUS:
    asynch_cmd_received = TRUE;
    extract_wind_indication();
    if(wind_num[FIRST_VALUE] != NULL){
      check_WIND_cmd(); // take appropriate action depending on the IOT indication
    }
    break;
  case PERIOD:
    movement_command();
    break;
  default: break;
  }
//------------------------------------------------------------------------------
}

void send_command(char array[], int size){   
//------------------------------------------------------------------------------
// Function Name: send_command
// Description: This function sends any command to the IOT device by sending an
//              array, character by character.  It also prints to the PC the same
//              thing that is being transmitted to the IOT module.
//
// Passed: char array[];
//         int size;
// Locals: int i;
// Returned: no variables returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  int i;
  for(i=RESET;i<(size-DECREMENT);i++){
    outchar(array[i]);
    out_wifi(array[i]);
  }

  out_wifi(CARRIAGE_RETURN);
  print_CR();
//------------------------------------------------------------------------------
}

void set_command(void){
//------------------------------------------------------------------------------
// Function Name: set_command
// Description: This function sends the prefix command to the IOT device for
//              configuring an element on the IOT module by sending a set 
//              array, character by character.  It also prints to the PC the same
//              thing that is being transmitted to the IOT module.
//
// Passed: no passed variables
// Locals: int i;
// Returned: no variables returned
// Globals: char set[ARRAY_SIZE_11] = {"AT+S.SCFG="};
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  int i;
  for(i=RESET;i<(sizeof(set)-DECREMENT);i++){
    outchar(set[i]);
    out_wifi(set[i]);
  }
//------------------------------------------------------------------------------
}

void get_command(void){
//------------------------------------------------------------------------------
// Function Name: get_command
// Description: This function sends the prefix command to the IOT device for it
//              to return the configuration of an element on the IOT module by 
//              sending a set array, character by character.  It also prints to 
//              the PC the same thing that is being transmitted to the IOT module.
//
// Passed: no passed variables
// Locals: int i;
// Returned: no variables returned
// Globals: char get[ARRAY_SIZE_11] = {"AT+S.GCFG="};
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  int i;
  for(i=RESET;i<(sizeof(get)-DECREMENT);i++){
    outchar(get[i]);
    out_wifi(get[i]);
  }
//------------------------------------------------------------------------------
}

void get_ip(void){
//------------------------------------------------------------------------------
// Function Name: get_ip
// Description: This function is called when the IOT device is transmitting the
//              value of ip_addr.  The IP address is stored in array ip_address.
//
// Passed: no passed variables
// Locals: int temp;
// Returned: no variables returned
// Globals: char ip_address[ARRAY_SIZE_16];
//          volatile unsigned char current_iot_character;
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  int temp = RESET;
  current_iot_character = NO_CHARACTER;
  grab_iot_character();
  while((current_iot_character != CARRIAGE_RETURN)){
    while((current_iot_character >= PERIOD) && (current_iot_character <= ASCII_9)){
      ip_address[temp++] = current_iot_character;
      if (wait_for_iot_character()){
        return;
      }
    }
    if (wait_for_iot_character()){
      if((current_iot_character != CARRIAGE_RETURN)) return;
      }
  }
  ip_address[temp] = NULL; 
//------------------------------------------------------------------------------  
}

void display_ip(void){
//------------------------------------------------------------------------------
// Function Name: display_ip
// Description: This function is called after you have captured the value of
//              ip_addr.  The IP address is split in half and stored in two
//              separate arrays so that it can be displayed on lines 3 and 4 of
//              the LCD.
//
// Passed: no passed variables
// Locals: int temp;
//         int periods;
//         int ip;
// Returned: no variables returned
// Globals: char ip_address[ARRAY_SIZE_16];
//          char ip_first_line[ARRAY_SIZE_9];
//          char ip_second_line[ARRAY_SIZE_8];
//          volatile unsigned char current_iot_character;
//          extern volatile char update_display;
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  int temp = RESET;
  int periods = RESET;
  int ip = RESET;
  lcd_4line();
   
  for(temp = RESET; periods < SPLIT_IP; temp++){
    ip_first_line[temp] = ip_address[ip];
    if(ip_address[ip++] == PERIOD) periods++;
  }
  ip_first_line[++temp] = NULL;
  for(temp = RESET; ip_address[ip] != NULL; temp++){
    ip_second_line[temp] = ip_address[ip++];
  }
  ip_second_line[++temp] = NULL;
  display_3 = ip_first_line;
  posL3 = INDENT_0;
  display_4 = ip_second_line;
  posL4 = INDENT_0;
  update_display = TRUE;
//------------------------------------------------------------------------------  
}

void grab_iot_character(void){
//------------------------------------------------------------------------------
// Function Name: grab_iot_character
// Description: This function checks to see if a new character has been written
//              to the receive buffer from the IOT device and, if so, copies it 
//              over to current_iot_character.
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no values returned
// Globals: volatile unsigned char current_iot_character;
//          extern volatile unsigned int iot_rx_ring_wr;
//          extern unsigned int iot_rx_ring_rd;
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  if (iot_rx_ring_rd == iot_rx_ring_wr){    // If no character is avalable Return 0x00
    current_iot_character = NO_CHARACTER;
  }else{
    UCA1IE &= ~UCRXIE;                      // Disable USART0 RX interrupt
    current_iot_character = IOT_Char_Rx[iot_rx_ring_rd];
    increment_iot_rx_ring();
    UCA1IE |= UCRXIE;                       // Enable USART0 RX interrupt
  }
//------------------------------------------------------------------------------
}

int wait_for_iot_character(void){
//------------------------------------------------------------------------------
// Function Name: wait_for_iot_character
// Description: This function waits for a character to be entered by the IOT module.
//              It will time out after 1 second.
//              This requires character_time_out be incremented in a timer.
//              In the timer associated, it is a 5msec timer and the code is:
//              if(iot_character_time_out <= 50){
//                iot_character_time_out++;
//              }
//
// Passed: no variables passed
// Locals: no local variables
// Returned: int defined as TRUE or FALSE
// Globals: volatile unsigned char iot_character_time_out;
//          extern volatile unsigned int iot_rx_ring_wr;
//          extern unsigned int iot_rx_ring_rd;
//          volatile unsigned char current_iot_character;
//          extern volatile unsigned char IOT_Char_Rx[SMALL_RING_SIZE];
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  iot_character_time_out = RESET;
  while(iot_character_time_out <= ONE_SECOND){
    if (iot_rx_ring_rd != iot_rx_ring_wr){  // Character has arrived
      UCA1IE &= ~UCRXIE;                    // Disable USART0 RX interrupt
      current_iot_character = IOT_Char_Rx[iot_rx_ring_rd];
      increment_iot_rx_ring();
      UCA1IE |= UCRXIE;                     // Enable USART0 RX interrupt
      return FALSE;
    }
    for (int i = TIME_FOR_FLAG; i > RESET; i--);         // Time for flag to set
  }
  return TRUE;
//------------------------------------------------------------------------------
}

void increment_iot_rx_ring(void){
//------------------------------------------------------------------------------
// Function Name: increment_iot_rx_ring
// Description: This function increments the variable iot_rx_ring_rd that is used
//              to read from the IOT receiving buffer.  It is a ring buffer so 
//              it loops back to the beginning once it reaches the end.
//
// Passed: no variables passed
// Locals: no local variables
// Returned: no variables returned
// Globals: extern unsigned int iot_rx_ring_rd;
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  iot_rx_ring_rd++;
  if (iot_rx_ring_rd >= (SMALL_RING_SIZE)){
    iot_rx_ring_rd = BEGINNING;
  }
//------------------------------------------------------------------------------
}

void out_wifi(char character){
//------------------------------------------------------------------------------
// Function Name: out_wifi
// Description: Transmits a character to the IOT module
//
// Passed: char character
// Locals: no local variables
// Returned: no variables returned
// Globals: no global variables
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  while (!(UCA1IFG & UCTXIFG));             // USCI_A1 TX buffer ready?
  UCA1TXBUF = character;
//------------------------------------------------------------------------------
}

void check_WIND_cmd(void){
//------------------------------------------------------------------------------
// Function Name: check_WIND_cmd
// Description: This function checks to see if the IOT sent back an important
//              indication that the program needs to take action on.
//              21- scanning for wifi, display "Scanning" on line 1 of the LCD
//              24- IOT transmitted the IP address upon startup, now capture and display it
//              25- connected to ncsu wifi, display "ncsu" on line 1 of the LCD
//              30,33,41- wifi lost, reset module
//
// Passed: char character
// Locals: no local variables
// Returned: no variables returned
// Globals: extern char wind_num[ARRAY_SIZE_3];
//          extern char *display_1;
//          extern char posL1;
//          char reset_cmd[ARRAY_SIZE_10] = {"AT+CFUN=1"};
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  if(wind_num[FIRST_VALUE] == ASCII_2){
    switch(wind_num[SECOND_VALUE]){
    case ASCII_1:
      display_1 = "Scanning";
      posL1 = INDENT_1;
      break;
    case ASCII_5:
      display_1 = "ncsu";
      posL1 = INDENT_3;
      break;
    case ASCII_4:
      get_ip();
      display_ip();
      break;
    default: break;
    }
    update_display = TRUE;
  }
  if(((wind_num[FIRST_VALUE] == ASCII_3) && (wind_num[SECOND_VALUE] == ASCII_0)) || ((wind_num[FIRST_VALUE] == ASCII_4) && (wind_num[SECOND_VALUE] == ASCII_1)) || ((wind_num[FIRST_VALUE] == ASCII_3) && (wind_num[SECOND_VALUE] == ASCII_3))){
    send_command(reset_cmd,ARRAY_SIZE_10);
    PJOUT &= ~IOT_RESET;          // Hardware reset for the module
    Five_msec_Delay(HUNDRED_MSEC); // Delay 100 msec
    PJOUT |= IOT_RESET;           // Bring module back online
  }
  wind_num[FIRST_VALUE] = NULL;
  wind_num[SECOND_VALUE] = NULL;
//------------------------------------------------------------------------------
}

void movement_command(void){
//------------------------------------------------------------------------------
// Function Name: movement_command
// Description: This function stores the command from the IOT module in the
//              command[] array and then evaluates the values in the array to 
//              determine what movements to make.  Each command is in the format
//              5711YXXX where 5711 is the secure pin number for security, Y is 
//              either F, B, R, or L, and XXX is a series of three numbers used
//              to specify how long to go in that direction.
//
// Passed: int temp;
// Locals: no local variables
// Returned: int defined as TRUE or FALSE
// Globals: extern char *display_2;
//          extern char posL2;
//          extern char *display_3;
//          volatile unsigned char current_iot_character;
//          extern volatile char update_display;
//          command[ARRAY_SIZE_9];
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  for(int temp = RESET; temp < NINTH_VALUE; temp++){
    if(!(wait_for_iot_character())){
      command[temp] = current_iot_character;
    }
  }
  update_display = TRUE;
  if((command[FIRST_VALUE] == ASCII_5) && (command[SECOND_VALUE] == ASCII_7) && (command[THIRD_VALUE] == ASCII_1) && (command[FOURTH_VALUE] == ASCII_1)){
    switch(command[FIFTH_VALUE]){
    case ASCII_F:
      if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_1) && (command[EIGHTH_VALUE] == ASCII_0)){
        forward(FIFTY_PERCENT);
        Five_msec_Delay(ONE_SECOND);
        stop();
      }
      else if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_2) && (command[EIGHTH_VALUE] == ASCII_0)){
       forward(FIFTY_PERCENT);
        Five_msec_Delay(TWO_SECONDS);
        stop();
      }
      else if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_3) && (command[EIGHTH_VALUE] == ASCII_0)){
        forward(FIFTY_PERCENT);
        Five_msec_Delay(THREE_SECONDS);
        stop();
      }
      else if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_5) && (command[EIGHTH_VALUE] == ASCII_0)){
       forward(FIFTY_PERCENT);
        Five_msec_Delay(FIVE_SECONDS);
        stop();
      }
      lcd_BIG_mid();
      display_2 = "FORWARD";
      posL2 = INDENT_1;
      break;
    case ASCII_B:
      if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_2) && (command[EIGHTH_VALUE] == ASCII_0)){
        reverse(FIFTY_PERCENT);
        Five_msec_Delay(TWO_SECONDS);
        stop();
      }
      else if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_1) && (command[EIGHTH_VALUE] == ASCII_0)){
        reverse(FIFTY_PERCENT);
        Five_msec_Delay(ONE_SECOND);
        stop();
      }
      else if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_3) && (command[EIGHTH_VALUE] == ASCII_0)){
        reverse(FIFTY_PERCENT);
        Five_msec_Delay(THREE_SECONDS);
        stop();
      }
      else if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_5) && (command[EIGHTH_VALUE] == ASCII_0)){
        reverse(FIFTY_PERCENT);
        Five_msec_Delay(FIVE_SECONDS);
        stop();
      }
      lcd_BIG_mid();
      display_2 = "REVERSE";
      posL2 = INDENT_1;
      break;
    case ASCII_L:
      if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_4) && (command[EIGHTH_VALUE] == ASCII_5)){
        spin_ccwise(FORTY_FIVE_DEGREES);
      }
      else if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_9) && (command[EIGHTH_VALUE] == ASCII_0)){
        spin_ccwise(NINETY_DEGREES);
      }
      else if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_3) && (command[EIGHTH_VALUE] == ASCII_0)){
        spin_ccwise(THIRTY_DEGREES);
      }
      lcd_BIG_mid();
      display_2 = "LEFT";
      posL2 = INDENT_3;
      break;
    case ASCII_R:
      if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_9) && (command[EIGHTH_VALUE] == ASCII_0)){
        spin_cwise(NINETY_DEGREES);
      }
      else if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_4) && (command[EIGHTH_VALUE] == ASCII_5)){
        spin_cwise(FORTY_FIVE_DEGREES);
      }
      else if((command[SIXTH_VALUE] == ASCII_0) && (command[SEVENTH_VALUE] == ASCII_3) && (command[EIGHTH_VALUE] == ASCII_0)){
        spin_cwise(THIRTY_DEGREES);
      }
      lcd_BIG_mid();
      display_2 = "RIGHT";
      posL2 = INDENT_2;
      break;
    case ASCII_I:
      state = FINDING_LINE;
      break;
    case ASCII_E:
      state = EXITING;
      break;
    default: break;
    }
    display_3 = "          ";
    update_display = TRUE;
  }
//------------------------------------------------------------------------------
}

void extract_wind_indication(void){
//------------------------------------------------------------------------------
// Function Name: extract_wind_indication
// Description: This function extracts which WIND number indication was sent from
//              the IOT device, which will later be evaluated and acted upon in
//              check_WIND_cmd.  The wind number is stored in the wind_num[] array.
//
// Passed: no variables passed
// Locals: int temp;
// Returned: no variables returned
// Globals: volatile unsigned char current_iot_character;
//          extern char asynch_cmd_received;
//          extern char wind_num[ARRAY_SIZE_3];
//
// Author: Graham Flinchum
// Date: November 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------
  int temp = RESET;
  while(asynch_cmd_received){
      if(!(wait_for_iot_character())){
        if((current_iot_character >= ASCII_0) && (current_iot_character <= ASCII_9)){
          wind_num[temp] = current_iot_character;
          temp++;
          if(!(wait_for_iot_character())){
            if((current_iot_character >= ASCII_0) && (current_iot_character <= ASCII_9)){
            wind_num[temp++] = current_iot_character;
            }
          }
          asynch_cmd_received = FALSE;
        }
      }
    }
    wind_num[temp] = NULL;      // terminate array
//------------------------------------------------------------------------------
}