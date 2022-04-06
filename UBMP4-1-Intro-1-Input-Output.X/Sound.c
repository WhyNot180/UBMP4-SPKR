#include    "xc.h"
#include    "UBMP410.h"
#include    "Sound.h"

//Starts at C2 and goes to B6 changing rows every octave (half steps included so total = 12 per row)
//this is the period (1/frequency) in micro seconds
//Order is C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B
const unsigned short notes[5][12] = {
                                {15288, 14430, 13620, 12856, 12134, 11453, 10810, 10204, 9631, 9090, 8580, 8099},
                                {7644, 7214, 6810, 6427, 6067, 5726, 5405, 5101, 4815, 4545, 4290, 4049},
                                {3822, 3607, 3405, 3214, 3034, 2864, 2703, 2551, 2408, 2273, 2146, 2025},
                                {1912, 1805, 1703, 1608, 1518, 1433, 1353, 1277, 1205, 1137, 1074, 1014},
                                {957, 904, 853, 805, 760, 718, 678, 636, 604, 570, 539, 509}
                                };

const unsigned short C2 = 15288;
const unsigned short Cs2 = 14430;
const unsigned short D2 = 13620;
const unsigned short Ds2 = 12856;
const unsigned short E2 = 12134;
const unsigned short F2 = 11453;
const unsigned short Fs2 = 10810;
const unsigned short G2 = 10204;
const unsigned short Gs2 = 9631;
const unsigned short A2 = 9090;
const unsigned short As2 = 8580;
const unsigned short B2 = 8099;
const unsigned short C3
const unsigned short Cs3
const unsigned short D3
const unsigned short Ds3
const unsigned short E3
const unsigned short F3
const unsigned short Fs3
const unsigned short G3
const unsigned short Gs3
const unsigned short A3
const unsigned short As3
const unsigned short B3
const unsigned short C4
const unsigned short Cs4
const unsigned short D4
const unsigned short Ds4
const unsigned short E4
const unsigned short F4
const unsigned short Fs4
const unsigned short G4
const unsigned short Gs4
const unsigned short A4
const unsigned short As4
const unsigned short B4
const unsigned short C5
const unsigned short Cs5
const unsigned short D5
const unsigned short Ds5
const unsigned short E5
const unsigned short Fs5
const unsigned short G5
const unsigned short Gs5
const unsigned short A5
const unsigned short As5
const unsigned short B5
const unsigned short C6
const unsigned short Cs6
const unsigned short D6
const unsigned short Ds6
const unsigned short E6
const unsigned short F6
const unsigned short Fs6
const unsigned short G6
const unsigned short Gs6
const unsigned short A6
const unsigned short As6
const unsigned short B6

void var_delay_us(unsigned short microseconds) {
    unsigned short delay = microseconds;
    while (delay > 0) {
        __delay_us(1);
        delay--;
    }
}

void play_note(unsigned short note, unsigned short duration) {
    for (int i = 0; i < duration; i++) {
        BEEPER = 1;
        var_delay_us(note);
        BEEPER = 0;
    }
}