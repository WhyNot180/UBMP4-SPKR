#include    "xc.h"
#include    "stdint.h"
#include    "stdbool.h"
#include    "UBMP410.h"
#include    "Sound.h"

// x is whether it is silent or not, y is the period, z is the amount to bitshift
#define PERIODGATE(y, z) y << z
#define WAVEFORMGATE(y, z) y >> z

// The full length periods of each note
static unsigned long truePeriods[3] = { 0, 0, 0 };

// The duty cycle of each note (e.g. if truePeriod[n] = 100 and firstEffects[n] = 4 then waveForms[n] = 6)
static unsigned long waveForms[3] = { 0, 0, 0 };

// Counters for the pitch of each note
static unsigned long pitch[3] = { 0, 0, 0 };

void receivePitchData(unsigned long* truePeriod, unsigned long* pitch, unsigned long* waveForm) {
    unsigned long pitchData[2] = {0, 0};
    // asks for rhythm, silent, and effect info
    while (!RCIF) {
        TXREG = 0;
        while (!TRMT);
    }
    char effect = bluetooth_getChar();
    // asks for pitch info
    while (!RCIF) {
        TXREG = 1;
        while (!TRMT);
    }
    pitchData[0] = bluetooth_getChar();
    while (!RCIF) {
        TXREG = 2;
        while (!TRMT);
    }
    pitchData[1] = bluetooth_getChar();
    *truePeriod = PERIODGATE(((pitchData[0] << 8) | pitchData[1]), 1); // Stitches together the 8-bit data into a 16-bit int
    *pitch = *truePeriod;
    *waveForm = WAVEFORMGATE(*truePeriod, effect);
}

void __interrupt() test(void) {
    if (RCIE && RCIF) {
        
        RCIE = 0;
        RCIF = 0;
        char channel = bluetooth_getChar();
        if (channel == 0) {
            receivePitchData(&truePeriods[0], &pitch[0], &waveForms[0]);
        }

        if (channel == 1) {
            receivePitchData(&truePeriods[1], &pitch[1], &waveForms[1]);
        }

        if (channel == 2) {
            receivePitchData(&truePeriods[2], &pitch[2], &waveForms[2]);
        }

        RCIE = 1;
    }
}

void _makeSound(void)
{

    // Output [n][0] is for pitch and effect domain and output [n][1] is for rhythm domain
    char outputs[3] = { 0, 0, 0 };

    do {
        BEEPER = outputs[0];
        // Decrements pitch counter every cycle of the while loop and if it is less than the duty cycle (i.e 6.25%) then the output is one else it is 0
        if (pitch[0]-- == 0) {
            pitch[0] = truePeriods[0];
        }
        if (pitch[0] <= waveForms[0]) {
            outputs[0] = 1;
        }
        if (pitch[0] >= waveForms[0]) {
            outputs[0] = 0;
        }


        BEEPER = outputs[1];
        if (pitch[1]-- == 0) {
            pitch[1] = truePeriods[1];
        }
        if (pitch[1] <= waveForms[1]) {
            outputs[1] = 1;
        }
        if (pitch[1] >= waveForms[1]) {
            outputs[1] = 0;
        }

        BEEPER = outputs[2];
        if (pitch[2]-- == 0) {
            pitch[2] = truePeriods[2];
        }
        if (pitch[2] <= waveForms[2]) {
            outputs[2] = 1;
        }
        if (pitch[2] >= waveForms[2]) {
            outputs[2] = 0;
        }

    } while (1);
    BEEPER = 0;
}