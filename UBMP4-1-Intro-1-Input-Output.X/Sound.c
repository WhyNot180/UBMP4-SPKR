#include    "xc.h"
#include    "stdint.h"
#include    "stdbool.h"
#include    "UBMP410.h"
#include    "Sound.h"
 
#define MIN(x, y) x < y ? x : y
#define MAX(x, y) x < y ? y : x
#define TOTALNOTES (period == 0 ? 0 : 1) + (period2 == 0 ? 0 : 1) + (period3 == 0 ? 0 : 1)
#define ISREST(x) x == Rest ? 1 : 0

// x is whether it is silent or not, y is the period, z is the amount to bitshift
#define PERIODGATE(x, y, z) !x ? y << z : 1
#define WAVEFORMGATE(x, y, z) !x ? y >> z : 0
 
 
unsigned long ipow(unsigned long base, unsigned char exp) {
    unsigned long result = base;
    for (unsigned char i = 1; i < exp; i++) {
        result *= base;
    }
    return result;
}

unsigned long findGCD(unsigned long a, unsigned long b) {
    unsigned long t = 0;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

unsigned long findLCM(unsigned long a, unsigned long b) {
    return a / findGCD(a, b) * b;
}

unsigned char findOctave(enum Octave note) {
    switch (note)
    {
    case O2:
        return 2;
    case O3:
        return 3;
    case O4:
        return 4;
    case O5:
        return 5;
    case O6:
        return 6;
    case O7:
        return 7;
    case O8:
        return 8;
    default:
        return 1;
    }
}

unsigned long findLength(enum MusicalNoteLength noteLength) {
    switch (noteLength)
    {
    case EighthNote:
        return 2;
    case QuarterNote:
        return 4;
    case ThreeEighthNote:
        return 6;
    case HalfNote:
        return 8;
    case SixEighthNote:
        return 12;
    case FullNote:
        return 16;
    default:
        return 1;
    }
}
 
#define CLOCK_FREQ 48000000
unsigned long lowerNotePeriods[] = {
    // These period values must align with the MusicalNote indexes
    // Note frequency values attained from https://pages.mtu.edu/~suits/notefreqs.html
    // The period is calculated by dividing the clock frequency the by note frequency
    CLOCK_FREQ / 1635 * 100, // C
    CLOCK_FREQ / 1732 * 100, // Cs
    CLOCK_FREQ / 1835 * 100, // D
    CLOCK_FREQ / 1945 * 100, // Ds
    CLOCK_FREQ / 2060 * 100, // E
    CLOCK_FREQ / 2183 * 100, // F
    CLOCK_FREQ / 2312 * 100, // Fs
    CLOCK_FREQ / 2450 * 100, // G
    CLOCK_FREQ / 2596 * 100, // Gs
    CLOCK_FREQ / 2750 * 100, // A
    CLOCK_FREQ / 2914 * 100, // As
    CLOCK_FREQ / 3087 * 100, // B
};
 
void _makeSound(struct Song song)
{
    unsigned long truePeriods[3] = { PERIODGATE(song.silent1, song.periods[0], 1),
        PERIODGATE(song.silent2, song.periods[1], 1),
        PERIODGATE(song.silent3, song.periods[2], 1) };

    unsigned long waveForms[3] = { WAVEFORMGATE(song.silent1, song.periods[0], song.firstEffects[0]),
        WAVEFORMGATE(song.silent2, song.periods[1], song.firstEffects[1]),
        WAVEFORMGATE(song.silent3, song.periods[2], song.firstEffects[2])};

    unsigned long pitch[3] = { truePeriods[0], truePeriods[1], truePeriods[2]};

    unsigned char coreRhythms[8];

    for (int i = 7; i != -1; i--) {
        coreRhythms[i] = song.rhythmLengths[i];
    }

    unsigned int rhythms[3] = { song.firstRhythms[0], song.firstRhythms[1], song.firstRhythms[2] };

    unsigned int trueRhythms[3] = { song.firstRhythms[0], song.firstRhythms[1], song.firstRhythms[2] };

    uint8_t effects[3] = { song.firstEffects, song.firstEffects, song.firstEffects };

    unsigned char silents[3] = { song.silent1, song.silent2, song.silent3 };

    unsigned long masterCount = 218;

    uint16_t tempData = 0;

    char outputs[3][4] = { {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0} };

    do {
        //note: The Rhythm domain appears to add to the notes played at once, howvever, this is the work of the *evil* effect domain
        // It essentially provides a clearer tone, but it is what causes a sort of ringing, similar to a phone
        BEEPER = outputs[0][0] & outputs[0][2];
        LED3 = outputs[0][0] & outputs[0][2];
        if (pitch[0]-- == 0) {
            pitch[0] = truePeriods[0];
        }
        if (pitch[0] <= waveForms[0]) {
            outputs[0][0] = 1;
        }
        if (pitch[0] >= waveForms[0]) {
            outputs[0][0] = 0;
        }


        BEEPER = outputs[1][0] & outputs[1][2];
        LED3 = outputs[1][0] & outputs[1][2];
        if (pitch[1]-- == 0) {
            pitch[1] = truePeriods[1];
        }
        if (pitch[1] <= waveForms[1]) {
            outputs[1][0] = 1;
        }
        if (pitch[1] >= waveForms[1]) {
            outputs[1][0] = 0;
        }

        BEEPER = outputs[2][0] & outputs[2][2];
        LED3 = outputs[2][0] & outputs[2][2];
        if (pitch[2]-- == 0) {
            pitch[2] = truePeriods[2];
        }
        if (pitch[2] <= waveForms[2]) {
            outputs[2][0] = 1;
        }
        if (pitch[2] >= waveForms[2]) {
            outputs[2][0] = 0;
        }

        if (masterCount-- == 0) {
            masterCount = 218;
            if (rhythms[0]-- == 0) {
                rhythms[0] = outputs[0][2] ? song.silentRhythm : trueRhythms[0];
                outputs[0][2] ^= 1;
                if (!outputs[0][2]) {
                    TXREG = 88;
                    while (!TRMT);
                    while (!RCIF);
                    tempData = bluetooth_getChar();
                    trueRhythms[0] = coreRhythms[tempData & RHYTHM_ONE_MASK] + coreRhythms[(tempData & RHYTHM_TWO_MASK) >> 4];
                    effects[0] = (tempData & EFFECT_MASK) >> 8;
                    waveForms[0] = WAVEFORMGATE(silents[0], truePeriods[0], effects[0]);
                }
            }
            if (rhythms[1]-- == 0) {
                rhythms[1] = outputs[1][2] ? song.silentRhythm : trueRhythms[1];
                outputs[1][2] ^= 1;
                if (!outputs[1][2]) {
                    TXREG = 89;
                    while (!TRMT);
                    while (!RCIF);
                    tempData = bluetooth_getChar();
                    trueRhythms[1] = coreRhythms[tempData & RHYTHM_ONE_MASK] + coreRhythms[(tempData & RHYTHM_TWO_MASK) >> 4];
                    effects[1] = (tempData & EFFECT_MASK) >> 8;
                    waveForms[1] = WAVEFORMGATE(silents[1], truePeriods[1], effects[1]);
                }
            }
            if (rhythms[2]-- == 0) {
                rhythms[2] = outputs[2][2] ? song.silentRhythm : trueRhythms[2];
                outputs[2][2] ^= 1;
                if (!outputs[2][2]) {
                    TXREG = 90;
                    while (!TRMT);
                    while (!RCIF);
                    tempData = bluetooth_getChar();
                    trueRhythms[2] = coreRhythms[tempData & RHYTHM_ONE_MASK] + coreRhythms[(tempData & RHYTHM_TWO_MASK) >> 4];
                    effects[2] = (tempData & EFFECT_MASK) >> 8;
                    waveForms[2] = WAVEFORMGATE(silents[2], truePeriods[2], effects[2]);
                }
            }
        }
    } while (1);
    BEEPER = 0;
}
 
void playNote(struct Chord chord)
{

    // Initializes the notePeriods with their respective values based on their letter names
    enum MusicalNote notes[3];
    unsigned long notePeriods[3] = { 0, 0, 0 };
    for (int i = 2; i != -1; i--) {
        notes[i] = chord.chordNotes[i] & MUSICAL_NOTE_MASK;
        switch (notes[i])
        {
        case Rest:
            // It shouldn't matter what the period is, as long as the total cycles normalizes to the proper
            // note length.  We just want the Rest to be silent for the correct length of time.
            notePeriods[i] = CLOCK_FREQ / 62;
            break;
        default:
            if (notes[i] <= B)
                notePeriods[i] = lowerNotePeriods[notes[i]];
            break;
        }
    }

    struct Song song;
    
    // We need to adjust the period by the octave (and a preferred scaling value)
    // Also, we want the note to play for the precise length of time regardless of the period
    // so we have to adjust the number of cycles by the period
    for (int i = 2; i != -1; i--) {
        song.periods[i] = notePeriods[i] / ipow(2, findOctave(chord.chordNotes[i] & OCTAVE_NOTE_MASK)) / PERIOD_SCALE;
    }

    for (int i = 7; i != -1; i--) {
        TXREG = i + 48;
        while (!TRMT);
        while (!RCIF);
        song.rhythmLengths[i] = bluetooth_getChar();
    }

    for (int i = 2; i != -1; i--) {
        TXREG = i + 64;
        while (!TRMT)
        while (!RCIF);
        song.firstEffects[i] = bluetooth_getChar();

        TXREG = i + 67;
        while (!TRMT);
        while (!RCIF);
        song.firstRhythms[i] = bluetooth_getChar();
    }

    TXREG = 58;
    while (!RCIF);
    song.silentRhythm = bluetooth_getChar();

    // True (1) if note is a rest, otherwise false (0)
    song.silent1 = ISREST(notes[0]);
    song.silent2 = ISREST(notes[1]);
    song.silent3 = ISREST(notes[2]);

    _makeSound(song);
}