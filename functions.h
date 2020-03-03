//------------------------------------------------------------------------------
// File Name: functions.h
// Description: This file contains the function declarations for all other files
//
// Author: Graham Flinchum
// Date: September 2016
// Compiler: Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------

// Function prototypes main
void main(void);

// Function prototypes init
void Init_Conditions(void);
void Init_PWM(void);

// Function prototypes timers
void Init_Timers(void);
void Timer_code(void);
void Five_msec_Delay(unsigned int fivemsec);
void Init_Timer_A0(void);
void Init_Timer_A1(void);
void Init_Timer_B1(void);
void Init_Timer_B2(void);

// Function prototypes clocks
void Init_Clocks(void);

// Function prototypes systems
void enable_interrupts(void);

// Function prototypes
__interrupt void Timer2_B0_ISR(void);
__interrupt void TIMER2_B1_ISR(void);
void measure_delay(void);
void usleep10(unsigned int usec);

// Function prototypes ports
void Init_Ports(void);
void Init_Port1(void);
void Init_Port2(void);
void Init_Port3(void);
void Init_Port4(void);
void Init_PortJ(void);

// LCD
void Init_LCD(void);
void WriteIns(char instruction);
void WriteData(char data);
void ClrDisplay(void);
void SetPostion(char pos);
void DisplayOnOff(char data);
void lcd_BIG_mid(void);
void lcd_4line(void);
void lcd_out(char *s, char line, char position);

// SPI
void Init_SPI_B0(void);
void SPI_B0_write(char byte);

// Switches
void Init_Switches(void);
void switch_control(void);
void enable_switch_SW1(void);
void enable_switch_SW2(void);
void disable_switch_SW1(void);
void disable_switch_SW2(void);
void Switches_Process(void);
void Display_Process(void);

// Moving
void left_reverse(int rate);
void right_reverse(int rate);
void reverse(int rate);
void left_forward(int rate);
void right_forward(int rate);
void forward(int rate);
void left_stop(void);
void right_stop(void);
void stop(void);
void circle_left(int count);
void circle_right(int count);
void spin_cwise(int time);
void spin_ccwise(int time);

// ADC
void HEXtoBCD(int hex_value);
void ADC_Process(void);
void Init_ADC(void);
void Display_Detectors(void);
void detect_movement(void);
void White_Configuration(void);
void Black_Configuration(void);
void Configure(void);
void Find_Line(void);

// Serial
void Init_Serial_UCA0(int baud_rate);
void Init_Serial_UCA1(int baud_rate);

// IOT
void menu(void);
void iot_menu(void);
void grab_usb_character(void);
void grab_iot_character(void);
int wait_for_usb_character(void);
int wait_for_iot_character(void);
void menu_program(void);
void print_CR(void);
void outchar(char character);
void increment_usb_rx_ring(void);
void increment_iot_rx_ring(void);
void out_wifi(char character);
void send_command(char array[], int size);
void get_command(void);
void set_command(void);
void get_ip(void);
void display_ip(void);
void check_WIND_cmd(void);
void movement_command(void);
void extract_wind_indication(void);