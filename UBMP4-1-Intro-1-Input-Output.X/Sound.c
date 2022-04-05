#include    "xc.h"
#include    "UBMP410.h"
#include    "Sound.h"

//Starts at C2 and goes to B6 changing rows every octave (half steps included so total = 12 per row)
//this is in micro seconds
//Order is C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B
const unsigned short notes[5][12] = {
                                {15288, 14430, 13620, 12856, 12134, 11453, 10810, 10204, 9631, 9090, 8580, 8099},
                                {7644, 7214, 6810, 6427, 6067, 5726, 5405, 5101, 4815, 4545, 4290, 4049},
                                {3822, 3607, 3405, 3214, 3034, 2864, 2703, 2551, 2408, 2273, 2146, 2025},
                                {1912, 1805, 1703, 1608, 1518, 1433, 1353, 1277, 1205, 1137, 1074, 1014},
                                {957, 904, 853, 805, 760, 718, 678, 636, 604, 570, 539, 509}
                                };

void var_delay_us(unsigned short microseconds) {
    unsigned short delay = microseconds;
    while (delay > 0) {
        __delay_us(1);
        delay--;
    }
}

void play_note(unsigned short note, unsigned short duration) {
    BEEPER = 1;
    var_delay_us(note);
    BEEPER = 0;
}