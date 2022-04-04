#include    "xc.h"
#include    "math.h"
/* 
const short c2 = -20;
const short cs2 = -19;
const short d2 = -18;
const short

//Starts at C2 and goes to C6 changing rows every octave (half steps included so total = 12 per row)
//this is in micro seconds
//Order is C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B
const unsigned short notes[][] = {
                                {15288, 14430, 13620, 12856, 12134, 11453, 10810, 10204, 9631, 9090, 8580, 8099},
                                {7644, 7214, 6810, 6427, 6067, 5726, 5405, 5101, 4815, 4545, 4290, 4049},
                                {-9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2},
                                {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
                                {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26}
                                };

unsigned int frequency(char note){
    unsigned int freq = 1000000 / ((440 * pow(2,  (note/12) * 10000) / 10000));
} */