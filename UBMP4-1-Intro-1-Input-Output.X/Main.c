/*==============================================================================
 Project: Intro-1-Input-Output
 Date:    March 1, 2022
 
 This example UBMP4.1 input and output program demonstrates pushbutton input,
 LED (bit) output, port latch (byte) output, time delay functions, and simple
 'if' condition structures.
 
 Additional program analysis and programming activities demonstrate byte output,
 logical condition operators AND and OR, using delay functions to create sound,
 and simulated start-stop button functionality.
==============================================================================*/
 
#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions
 
#include    "UBMP410.h"         // Include UBMP4.1 constant and function definitions
 
#include    "Sound.h"
 
// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.
 
// The main function is required, and the program begins executing from here.
 
int main(void)
{
    // Configure oscillator and I/O ports. These functions run once at start-up.
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
   
    // Code in this while loop runs repeatedly.
    while(1)
    {
        if (SW2 == 0) {
            playNote(Or, Or, Or);
            playNote(Or, Or, Or);
            playNote(C | QuarterNote, E | QuarterNote, G | QuarterNote);
        }
 
        if (SW3 == 0) {
            playNote(Or, Or, Or);
            playNote(Or, Or, Ou);
            playNote(E | QuarterNote, G | QuarterNote, C | QuarterNote);
        }

        if (SW4 == 0) {
            playNote(Or, Or, Or);
            playNote(Or, Or, Od);
            playNote(E | QuarterNote, G | QuarterNote, C | QuarterNote);
        }

        if (SW5 == 0) {
            playNote(Or, Or, Or);
            playNote(C | QuarterNote, Rest | QuarterNote, Rest | QuarterNote);
            playNote(Ou, Or, Or);
            playNote(C | QuarterNote, Rest | QuarterNote, Rest | QuarterNote);
        }

        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }
    }
}