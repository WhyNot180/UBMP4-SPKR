/*==============================================================================
 File: UBMP410.c
 Date: March 1, 2022
 
 UBMP4.1 (PIC16F1459) hardware initialization functions
 
 Initialization functions to configure the PIC16F1459 oscillator and on-board
 UBMP4 devices. Include UMBP410.h in your main program to call these functions.
==============================================================================*/

#include    "xc.h"              // XC compiler general include file

#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP410.h"         // Include UBMP4.1 constant and function definitions

// TODO Initialize oscillator, ports and other PIC/UBMP hardware features here:

// Configure oscillator for 48 MHz operation (required for USB bootloader).
void OSC_config(void)
{
    OSCCON = 0xFC;              // Set 16MHz HFINTOSC with 3x PLL enabled
    ACTCON = 0x90;              // Enable active clock tuning from USB clock
    while(!PLLRDY);             // Wait for PLL lock (disable for simulation)
}

// Configure hardware ports and peripherals for on-board UBMP4 I/O devices.
void UBMP4_config(void)
{
    OPTION_REG = 0b01010111;    // Enable port pull-ups, TMR0 internal, div-256

    LATA = 0b00000000;          // Clear Port A latches before configuring PORTA
    TRISA = 0b00001111;         // Set RUNLED and Beeper pins as outputs
    ANSELA = 0b00000000;        // Make all Port A pins digital
    WPUA = 0b00001000;          // Enable weak pull-up on SW1 input only

    LATB = 0b00000000;          // Clear Port B latches before configuring PORTB
    TRISB = 0b01110000;         // Enable pushbutton pins as inputs (SW2-SW5)
    ANSELB = 0b00000000;        // Make all Port B pins digital
    WPUB = 0b01010000;          // Enable weak pull-ups on pushbutton inputs (SW2 and SW4)

    LATC = 0b00000000;          // Clear Port C latches before configuring PORTC
    TRISC = 0b00001111;         // Set LED pins as outputs, H1-H4 pins as inputs
    ANSELC = 0b00000000;        // Make all Port C pins digital

    // Enable interrupts here, if required.
}

// Configure ADC for 8-bit conversion from on-board phototransistor Q1 (AN7).
void ADC_config(void)
{
    LATC = 0b00000000;          // Clear Port C latches before configuring PORTC
    // Configure RC3 for analog input. Enable additional or other inputs below:
    TRISCbits.TRISC3 = 1;       // Disable individual output drivers (TRISx.bit = 1)
    ANSELC = 0b00001000;        // Enable individual analog inputs (ANSELx.bit = 1)
    // General ADC setup and configuration
    ADCON0 = 0b00011100;        // Set channel to AN7, leave A/D converter off
    ADCON1 = 0b01100000;        // Left justified result, FOSC/64 clock, +VDD ref
    ADCON2 = 0b00000000;        // Auto-conversion trigger disabled
}

// Enable ADC and switch the input mux to the specified channel (use channel
// constants defined in UBMP410.h header file - eg. ANQ1).
void ADC_select_channel(unsigned char channel)
{
    ADON = 1;                   // Turn the A-D converter on
    ADCON0 = (ADCON0 & 0b10000011); // Clear channel select (CHS) bits by ANDing
    ADCON0 = (ADCON0 | channel);	// Set channel by ORing with channel const.
}

// Convert currently selected channel and return 8-bit conversion result.
unsigned char ADC_read(void)
{
    GO = 1;                     // Start the conversion by setting Go/~Done bit
	while(GO)                   // Wait for the conversion to finish (GO==0)
        ;                       // Terminate the empty while loop
    return (ADRESH);            // Return the MSB (upper 8-bits) of the result
}

// Enable ADC, switch to specified channel, and return 8-bit conversion result.
// Use channel constants defined in UBMP410.h header file (eg. ANQ1).
unsigned char ADC_read_channel(unsigned char channel)
{
    ADON = 1;                   // Turn the A-D converter on
    ADCON0 = (ADCON0 & 0b10000011); // Clear channel select (CHS) bits by ANDing
    ADCON0 = (ADCON0 | channel);	// Set channel by ORing with chan. constant
    __delay_us(5);              // Allow input to settle (charges internal cap.)
    GO = 1;                     // Start the conversion by setting Go/~Done bit
	while(GO)                   // Wait for the conversion to finish (GO==0)
        ;                       // Terminate the empty while loop
    ADON = 0;                   // Turn the A-D converter off
    return (ADRESH);            // Return the MSB (upper 8-bits) of the result
}

// Configure EUSART for HC-08 bluetooth module
void bluetooth_config(void)
{
    SPBRGH = 4;                 // Sets baudrate to 9600
    SPBRGL = 225;               // Sets baudrate to 9600
    BRGH = 1;                   // Sets high speed baudrate
    BRG16 = 1;                  // Enables 16 bit baudrate 
    SYNC = 0;                   // Sets to Asyncronous transmission
    SPEN = 1;                   // Enables EUSART
    TX9 = 0;                    // Enables 8-bit transmission
    RX9 = 0;                    // Enables 8-bit recieving
    GIE = 1;                    // Disables global interrupts
    PEIE = 1;                   // Disables peripheral interrupts
    TXIE = 0;                   // Disables interrupts on TX pin
    RCIE = 1;                   // Disables interrupts on RX pin
    TXEN = 1;                   // Enables transmission pin
    CREN = 1;                   // Enables receiver pin
}

char bluetooth_getChar(void)
{
    // Checks for over-run error
    if (OERR) {
        CREN = 0;
        CREN = 1; // Resets receiver pin
    }

    // If a char has been sent, return the char, otherwise return 0
    if (RCIF == 1) {
        while (!RCIF);
        return RCREG;
    }
    else {
        return -1;
    }
}