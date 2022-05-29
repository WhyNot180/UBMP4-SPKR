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

unsigned int undertale[6][3] = {{Fs | FullNote | O5, F | FullNote | O5, Ds | FullNote | O5, Cs | FullNote | O5, Ds | FullNote | O5, As | FullNote | O4},
                                  {Rest | FullNote, Ds | FullNote | O3, Fs | FullNote | O3, Gs | FullNote | O3, As | FullNote | O3, Fs | FullNote | O3},
                                  {Ds | FullNote | O2, Rest | FullNote, Rest | FullNote, Rest | FullNote, Fs | FullNote | O2, Rest | FullNote}};
 
int main(void)
{
    // Configure oscillator and I/O ports. These functions run once at start-up.
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
   
    // Code in this while loop runs repeatedly.
    while(1)
    {
        if (SW2 == 0) {
            playNote(C | QuarterNote | O8, E | QuarterNote | O8, G | QuarterNote | O8, C | QuarterNote | O8, E | QuarterNote | O8, G | QuarterNote | O8);
        }

        if (SW4 == 0) {
            playNote(C | QuarterNote | O3, E | QuarterNote | O3, G | QuarterNote | O3, Rest | QuarterNote | O3, Rest | QuarterNote | O3, Rest | QuarterNote | O3);
        }

        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }
    }
}