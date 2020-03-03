//------------------------------------------------------------------------------
// File Name: macros.h
// Description: This file contains required defines
//
// Graham Flinchum
// September 2016
// Built with IAR Embedded Workbench Version: 6.50.1
//------------------------------------------------------------------------------

// Main
#define FIVE_COUNT              (5)
#define TRUE                    (1)
#define FALSE                   (0)
#define ALWAYS                  (1)
#define RESET                   (0)
#define ONE                    (50)
#define TWO                   (100)
#define THREE                 (150)
#define FOUR                  (200)
#define CNTL_STATE_INDEX        (3) // Control States
#define SW1                  (0x01) // Switch 1
#define SW2                  (0x02) // Switch 2
#define CNTL_STATE_INDEX        (3)
#define QUARTER_SECOND         (50) // for using Five_msec_Delay for 0.25 sec
#define FIVE_MSEC               (1) // for using Five_msec_Delay for 5 msec
#define HUNDRED_MSEC           (20)
#define WIFI                    (1)
#define FINDING_LINE            (2)
#define PAUSING                 (3)
#define FOLLOWING               (4)
#define EXITING                 (5)
#define BOTH                    (3)

// Arrays
#define ARRAY_SIZE_2            (2)
#define ARRAY_SIZE_3            (3)
#define ARRAY_SIZE_4            (4)
#define ARRAY_SIZE_5            (5)
#define ARRAY_SIZE_6            (6)
#define ARRAY_SIZE_7            (7)
#define ARRAY_SIZE_8            (8)
#define ARRAY_SIZE_9            (9)
#define ARRAY_SIZE_10          (10)
#define ARRAY_SIZE_11          (11)
#define ARRAY_SIZE_12          (12)
#define ARRAY_SIZE_13          (13)
#define ARRAY_SIZE_14          (14)
#define ARRAY_SIZE_15          (15)
#define ARRAY_SIZE_16          (16)
#define ARRAY_SIZE_17          (17)
#define ARRAY_SIZE_18          (18)
#define ARRAY_SIZE_19          (19)
#define ARRAY_SIZE_20          (20)
#define ARRAY_SIZE_25          (25)
#define ARRAY_SIZE_28          (28)
#define FIRST_VALUE             (0)
#define SECOND_VALUE            (1)
#define THIRD_VALUE             (2)
#define FOURTH_VALUE            (3)
#define FIFTH_VALUE             (4)
#define SIXTH_VALUE             (5)
#define SEVENTH_VALUE           (6)
#define EIGHTH_VALUE            (7)
#define NINTH_VALUE             (8)
#define TENTH_VALUE             (9)

// LCD
#define LCD_HOME_L1	       0x80
#define LCD_HOME_L2            0xA0
#define LCD_HOME_L3            0xC0
#define LCD_HOME_L4            0xE0
#define FIRST_CHAR            (0x00)
#define CHAR_PER_LINE         (0x0B) // number of chars per display line
#define INDENT_4                 (4) // number of spaces to indent from left
#define INDENT_3                 (3)
#define INDENT_2                 (2)
#define INDENT_1                 (1)
#define INDENT_0                 (0)

// Port 1 Pins
#define V_DETECT_R           (0x01)  // Voltage from Right Detector
#define V_DETECT_L           (0x02)  // Voltage from Left Detector
#define LCD_BACKLITE         (0x04)  // Control Signal for LCD_BACKLIGHT
#define V_THUMB              (0x08)  // Voltage from Thumb Wheel
#define SPI_CS_LCD           (0x10)  // LCD Chip Select
#define RESET_LCD            (0x20)  // LCD Reset
#define SIMO_B               (0x40)  // SPI mode - slave in/master out of USCI_B0
#define SOMI_B               (0x80)  // SPI mode - slave out/master in of USCI_B0

// Port 2 Pins
#define USB_TXD              (0x01) //
#define USB_RXD              (0x02) //
#define SPI_SCK              (0x04) //
#define P2_3                 (0x08) //
#define P2_4                 (0x10) //
#define CPU_TXD              (0x20) //
#define CPU_RXD              (0x40) //
#define P2_7                 (0x80) //

// Port 3 Pins
#define X                    (0x01) // 
#define Y                    (0x02) // 
#define P3Z                  (0x04) // 
#define IR_LED               (0x08) // 
#define R_FORWARD            (0x10) // 
#define L_FORWARD            (0x20) // 
#define R_REVERSE            (0x40) // 
#define L_REVERSE            (0x80) // 

// Port 4 Pins
#define SW1                  (0x01)  // Switch 1
#define SW2                  (0x02)  // Switch 2
// Port J Pins

#define IOT_WAKEUP           (0x01) // 
#define IOT_FACTORY          (0x02) // 
#define IOT_STA_MINIAP       (0x04) // 
#define IOT_RESET            (0x08) // 
#define XINR                 (0x10) // XINR
#define XOUTR                (0x20) // XOUTR

// Other Port Defines
#define INPUT                (0x00)
#define LOW                  (0x00)

// Clocks
#define FLLN_BITS         (0x03ffu)
#define FLLN_255          (0x00ffu)
#define CSLOCK               (0x01) // Any incorrect password locks registers
#define CLEAR_REGISTER     (0X0000)

// Timers
#define ONE_THOUSAND         (1000)
#define INCREMENT               (1)
#define DECREMENT               (1)
#define TA0CCR0_INTERVAL     (2500) // 8,000,000 / 2 / 8 / (1 / 10msec)
#define TA0CCR1_INTERVAL      (500) // 8,000,000 / 2 / 8 / (1 / 1msec)
#define TA0CCR2_INTERVAL     (2500) // 8,000,000 / 2 / 8 / (1 / 5msec)
#define TA1CCR0_INTERVAL    (62500) // 8,000,000 / 4 / 8 / (1 / 250msec)
#define TA1CCR1_INTERVAL      (250) // 8,000,000 / 4 / 8 / (1 / 1msec)
#define TA1CCR2_INTERVAL     (1250) // 8,000,000 / 4 / 8 / (1 / 5msec)
#define TB0CCR0_INTERVAL    (25000) // 8,000,000 / 2 / 8 / (1 / 50msec)
#define TB0CCR1_INTERVAL      (500) // 8,000,000 / 2 / 8 / (1 / 1msec)
#define TB0CCR2_INTERVAL     (2500) // 8,000,000 / 2 / 8 / (1 / 5msec)
#define TOGGLE_LCD             (10)
#define DEBOUNCED             (200)

// Moving
#define START                   (1)
#define GO                      (1)
#define STOP                   (91)
#define OFF                     (0)
#define HALF_SECOND           (100)
#define ONE_SECOND            (200)
#define TWO_SECONDS           (400)
#define THREE_SECONDS         (600)
#define FOUR_SECONDS          (800)
#define FIVE_SECONDS         (1000)
#define FORWARD_ONE_SECOND     (20)
#define REVERSE_ONE_SECOND     (20)

// PWM
#define WHEEL_PERIOD         (8000)
#define TEN_PERCENT           (800)
#define FIFTEEN_PERCENT      (1200)
#define TWENTY_PERCENT       (1600)
#define TWENTY_FIVE_PERCENT  (2000)
#define THIRTY_PERCENT       (2400)
#define FORTY_PERCENT        (3200)
#define FIFTY_PERCENT        (4000)
#define SIXTY_PERCENT        (4800)
#define SEVENTY_PERCENT      (5600)
#define EIGHTY_PERCENT       (6400)
#define NINETY_PERCENT       (7200)
#define ONE_HUNDRED_PERCENT  (8000)

// ADC
#define RESET_STATE        (0x0000)
#define TEN_THOUSANDS           (0)
#define THOUSANDS               (1)
#define HUNDREDS                (2)
#define TENS                    (3)
#define ONES                    (4)
#define NULL_CHAR               (5)
#define TEN_THOUSAND        (10000)
#define ONE_THOUSAND         (1000)
#define ONE_HUNDRED           (100)
#define NINETY_NINE            (99)
#define BLACK_THRESHOLD       (300)
#define WHITE_THRESHOLD       (100)
#define FIVE_PERCENT_ADC       (50)
#define TEN_DECIMAL            (10)
#define NINE_DECIMAL            (9)
#define MENU_DIVIDE           (500)
#define AVG_COUNT              (16)
#define DIVIDE_BY_SIXTEEN       (4)
#define DIVIDE_BY_TWO           (1)
#define NINETY_DEGREES        (100)
#define FORTY_FIVE_DEGREES     (50)
#define THIRTY_DEGREES         (33)
#define LEFT                    (1)
#define RIGHT                   (2)
#define LEEWAY                 (50)
#define STUCK                  (10)

// Serial
#define BEGINNING               (0) // Start of ring buffer
#define CLEAR                (0x00)
#define BRW_9600               (52)
#define MCTLW_9600         (0x4911)
#define BRW_115200              (4)
#define MCTLW_115200       (0x5551)
#define BAUD_9600               (1) // 9600 baud selected
#define BAUD_115200             (2) // 115,200 baud selected
#define NONE                    (0)
#define CASE_8               (0x08) // case 8 for serial ISR
#define SMALL_RING_SIZE        (32) // Ring Buffer Size
#define LARGE_RING_SIZE        (32)
#define NO_CHARACTER         (0x00) // No character available

// ASCII Table
#define NULL                 (0x00) // ASCII value for a '\0'
#define LINE_FEED            (0x0A) // ASCII value for a line feed
#define CARRIAGE_RETURN      (0x0D) // ASCII value for a carriage return
#define SPACE                (0x20) // ASCII value for a ' '
#define EXCLAMATION          (0x21) // ASCII value for a '!'
#define QUOTES               (0x22) // ASCII value for a '"'
#define POUND                (0x23) // ASCII value for a '#'
#define DOLLAR               (0x24) // ASCII value for a '$'
#define PERCENTAGE           (0x25) // ASCII value for a '%'
#define AMPERSAND            (0x26) // ASCII value for a '&'
#define APOSTROPHE           (0x27) // ASCII value for a '''
#define OPEN_PARENTHESIS     (0x28) // ASCII value for a '('
#define CLOSE_PARENTHESIS    (0x29) // ASCII value for a ')'
#define ASTERISK             (0x2A) // ASCII value for a '*'
#define PLUS                 (0x2B) // ASCII value for a '+'
#define COMMA                (0x2C) // ASCII value for a ','
#define HYPHEN               (0x2D) // ASCII value for a '-'
#define PERIOD               (0x2E) // ASCII value for a '.'
#define ASCII_0              (0x30) // ASCII value for a '0'
#define ASCII_1              (0x31) // ASCII value for a '1'
#define ASCII_2              (0x32) // ASCII value for a '2'
#define ASCII_3              (0x33) // ASCII value for a '3'
#define ASCII_4              (0x34) // ASCII value for a '4'
#define ASCII_5              (0x35) // ASCII value for a '5'
#define ASCII_6              (0x36) // ASCII value for a '6'
#define ASCII_7              (0x37) // ASCII value for a '7'
#define ASCII_8              (0x38) // ASCII value for a '8'
#define ASCII_9              (0x39) // ASCII value for a '9'
#define COLON                (0x3A) // ASCII value for a ':'
#define SEMICOLON            (0x3B) // ASCII value for a ';'
#define LESS_THAN            (0x3C) // ASCII value for a '<'
#define EQUALS               (0x3D) // ASCII value for a '='
#define GREATER_THAN         (0x3E) // ASCII value for a '>'
#define QUESTION             (0x3F) // ASCII value for a '?'
#define AT_SYMBOL            (0x40) // ASCII value for a '@'
#define ASCII_A              (0x41) // ASCII value for a 'A'
#define ASCII_B              (0x42) // ASCII value for a 'B'
#define ASCII_C              (0x43) // ASCII value for a 'C'
#define ASCII_D              (0x44) // ASCII value for a 'D'
#define ASCII_E              (0x45) // ASCII value for a 'E'
#define ASCII_F              (0x46) // ASCII value for a 'F'
#define ASCII_G              (0x47) // ASCII value for a 'G'
#define ASCII_H              (0x48) // ASCII value for a 'H'
#define ASCII_I              (0x49) // ASCII value for a 'I'
#define ASCII_J              (0x4A) // ASCII value for a 'J'
#define ASCII_K              (0x4B) // ASCII value for a 'K'
#define ASCII_L              (0x4C) // ASCII value for a 'L'
#define ASCII_M              (0x4D) // ASCII value for a 'M'
#define ASCII_N              (0x4E) // ASCII value for a 'N'
#define ASCII_O              (0x4F) // ASCII value for a 'O'
#define ASCII_P              (0x50) // ASCII value for a 'P'
#define ASCII_Q              (0x51) // ASCII value for a 'Q'
#define ASCII_R              (0x52) // ASCII value for a 'R'
#define ASCII_S              (0x53) // ASCII value for a 'S'
#define ASCII_T              (0x54) // ASCII value for a 'T'
#define ASCII_U              (0x55) // ASCII value for a 'U'
#define ASCII_V              (0x56) // ASCII value for a 'V'
#define ASCII_W              (0x57) // ASCII value for a 'W'
#define ASCII_X              (0x58) // ASCII value for a 'X'
#define ASCII_Y              (0x59) // ASCII value for a 'Y'
#define ASCII_Z              (0x5A) // ASCII value for a 'Z'
#define CARET                (0x5E) // ASCII value for a '^'
#define ASCII_a              (0x61) // ASCII value for a 'a'
#define ASCII_b              (0x62) // ASCII value for a 'b'
#define ASCII_c              (0x63) // ASCII value for a 'c'
#define ASCII_d              (0x64) // ASCII value for a 'd'
#define ASCII_e              (0x65) // ASCII value for a 'e'
#define ASCII_f              (0x66) // ASCII value for a 'f'
#define ASCII_g              (0x67) // ASCII value for a 'g'
#define ASCII_h              (0x68) // ASCII value for a 'h'
#define ASCII_i              (0x69) // ASCII value for a 'i'
#define ASCII_j              (0x6A) // ASCII value for a 'j'
#define ASCII_k              (0x6B) // ASCII value for a 'k'
#define ASCII_l              (0x6C) // ASCII value for a 'l'
#define ASCII_m              (0x6D) // ASCII value for a 'm'
#define ASCII_n              (0x6E) // ASCII value for a 'n'
#define ASCII_o              (0x6F) // ASCII value for a 'o'
#define ASCII_p              (0x70) // ASCII value for a 'p'
#define ASCII_q              (0x71) // ASCII value for a 'q'
#define ASCII_r              (0x72) // ASCII value for a 'r'
#define ASCII_s              (0x73) // ASCII value for a 's'
#define ASCII_t              (0x74) // ASCII value for a 't'
#define ASCII_u              (0x75) // ASCII value for a 'u'
#define ASCII_v              (0x76) // ASCII value for a 'v'
#define ASCII_w              (0x77) // ASCII value for a 'w'
#define ASCII_x              (0x78) // ASCII value for a 'x'
#define ASCII_y              (0x79) // ASCII value for a 'y'
#define ASCII_z              (0x7A) // ASCII value for a 'z'
#define TILDE                (0x7E) // ASCII value for a '~'

// IOT
#define SPLIT_IP                (2)
#define TIME_FOR_FLAG        (0x10)