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
    bluetooth_config();         // Configure EUSART for communicating with HC-08 bluetooth module
    struct Chord chord;
    // Code in this while loop runs repeatedly.
    while(1)
    {
        if (SW2 == 0) {
            chord.chordNotes[0] = C | QuarterNote | O8;
            chord.chordNotes[1] = E | QuarterNote | O8;
            chord.chordNotes[2] = G | QuarterNote | O8;
            playNote(chord);
        }

        if (SW4 == 0) {
            chord.silentRhythm = bluetooth_getChar;
            chord.chordNotes[0] = C | QuarterNote | O4;
            chord.chordNotes[1] = Rest | QuarterNote | O4;
            chord.chordNotes[2] = Rest | QuarterNote | O4;
            playNote(chord);
        }

        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }
    }
}