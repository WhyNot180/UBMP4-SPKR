#include    "xc.h"
#include    "stdbool.h"
#include    "UBMP410.h"
#include    "Sound.h"
 
#define MIN(x, y) x < y ? x : y
#define MAX(x, y) x < y ? y : x
#define TOTALNOTES (period == 0 ? 0 : 1) + (period2 == 0 ? 0 : 1) + (period3 == 0 ? 0 : 1) + (period4 == 0 ? 0 : 1) + (period5 == 0 ? 0 : 1) + (period6 == 0 ? 0 : 1)
 
 
unsigned long ipow(unsigned long base, unsigned char exp) {
    unsigned long result = base;
    for (unsigned char i = 1; i < exp; i++) {
        result *= base;
    }
    return result;
}

int findGCD(unsigned long a, unsigned long b) {
    unsigned long t = 0;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long findLCM(unsigned long a, unsigned long b) {
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
 
void _makeSound(unsigned long cycles, unsigned long period, bool silent,
    unsigned long period2, bool silent2,
    unsigned long period3, bool silent3,
    unsigned long period4, bool silent4,
    unsigned long period5, bool silent5,
    unsigned long period6, bool silent6)
{
    unsigned long truePeriods[3] = { !silent ? period << 1 : 1, !silent2 ? period2 << 1 : 1, !silent3 ? period3 << 1 : 1 };
    unsigned long waveForms[3] = { !silent ? period >> 3 : 0, !silent2 ? period2 >> 3 : 0, !silent3 ? period3 >> 3 : 0 };
    unsigned long pitch[3] = { !silent ? period << 1 : 1, !silent2 ? period2 << 1 : 1,!silent3 ? period3 << 1 : 1 };
    unsigned char coreRhythm = 1;
    unsigned char rhythms[3] = { coreRhythm, coreRhythm, coreRhythm };
    unsigned char coreEffect = 8;
    unsigned char effects[3] = { coreEffect, coreEffect, coreEffect };
    unsigned char coreStructure = 1;
    unsigned char structures[3] = { coreStructure, coreStructure, coreStructure };
    unsigned long trueMasterCount = findLCM(pitch[0], findLCM(pitch[1], pitch[2]));
    unsigned long masterCount = 218;
    bool end[3] = { 0, 0, 0 };
    char outputs[3][4] = { {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0} };
    bool trueSilents[3] = { silent, silent2, silent3 };
    do {
        //note: The Rhythm domain appears to add to the notes played at once, howvever, this is the work of the *evil* effect domain
        // It essentially provides a clearer tone, but it is what causes a sort of ringing, similar to a phone
        BEEPER = outputs[0][0]; //& outputs[0][1] & outputs[0][2];
        LED3 = outputs[0][0]; //& outputs[0][1] & outputs[0][2];
        if (pitch[0]-- == 0) {
            pitch[0] = truePeriods[0];
        }
        if (pitch[0] <= waveForms[0]) {
            outputs[0][0] = 1;
        }
        if (pitch[0] >= waveForms[0]) {
            outputs[0][0] = 0;
        }


        BEEPER = outputs[1][0]; //& outputs[1][1] & outputs[1][2];
        LED3 = outputs[1][0]; //& outputs[1][1] & outputs[1][2];
        if (pitch[1]-- == 0) {
            pitch[1] = truePeriods[1];
        }
        if (pitch[1] <= waveForms[1]) {
            outputs[1][0] = 1;
        }
        if (pitch[1] >= waveForms[1]) {
            outputs[1][0] = 0;
        }

        BEEPER = outputs[2][0]; //& outputs[2][1] & outputs[2][2];
        LED3 = outputs[2][0]; //& outputs[2][1] & outputs[2][2];
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
                rhythms[0] = coreRhythm;
                outputs[0][2] ^= 1;
            }
            if (rhythms[1]-- == 0) {
                rhythms[1] = coreRhythm;
                outputs[1][2] ^= 1;
            }
            if (rhythms[2]-- == 0) {
                rhythms[2] = coreRhythm;
                outputs[2][2] ^= 1;
            }
        }
    } while (1);//!(end[0] & end[1] & end[2]));
    BEEPER = 0;
}
 
void playNote(unsigned int notePlus, unsigned int note2Plus, unsigned int note3Plus, unsigned int note4Plus, unsigned int note5Plus, unsigned int note6Plus)
{
    enum MusicalNote note = notePlus & MUSICAL_NOTE_MASK;
    unsigned long period = 0;

    enum MusicalNote note2 = note2Plus & MUSICAL_NOTE_MASK;
    unsigned long period2 = 0;

    enum MusicalNote note3 = note3Plus & MUSICAL_NOTE_MASK;
    unsigned long period3 = 0;

    enum MusicalNote note4 = note4Plus & MUSICAL_NOTE_MASK;
    unsigned long period4 = 0;

    enum MusicalNote note5 = note5Plus & MUSICAL_NOTE_MASK;
    unsigned long period5 = 0;

    enum MusicalNote note6 = note6Plus & MUSICAL_NOTE_MASK;
    unsigned long period6 = 0;

    switch (note)
    {
    case Rest:
        // It shouldn't matter what the period is, as long as the total cycles normalizes to the proper
        // note length.  We just want the Rest to be silent for the correct length of time.
        period = CLOCK_FREQ / 62;
        break;
    default:
        if (note <= B)
            period = lowerNotePeriods[note];
        break;
    }

    // For secondary note
    switch (note2)
    {
    case Rest:
        // It shouldn't matter what the period is, as long as the total cycles normalizes to the proper
        // note length.  We just want the Rest to be silent for the correct length of time.
        period2 = CLOCK_FREQ / 62;
        break;
    default:
        if (note2 <= B)
            period2 = lowerNotePeriods[note2];
        break;
    }
    
    // For tertiary note
    switch (note3)
    {
    case Rest:
        // It shouldn't matter what the period is, as long as the total cycles normalizes to the proper
        // note length.  We just want the Rest to be silent for the correct length of time.
        period3 = CLOCK_FREQ / 62;
        break;
    default:
        if (note3 <= B)
            period3 = lowerNotePeriods[note3];
        break;
    }

    switch (note4)
    {
    case Rest:
        // It shouldn't matter what the period is, as long as the total cycles normalizes to the proper
        // note length.  We just want the Rest to be silent for the correct length of time.
        period4 = CLOCK_FREQ / 62;
        break;
    default:
        if (note4 <= B)
            period4 = lowerNotePeriods[note4];
        break;
    }

    switch (note5)
    {
    case Rest:
        // It shouldn't matter what the period is, as long as the total cycles normalizes to the proper
        // note length.  We just want the Rest to be silent for the correct length of time.
        period5 = CLOCK_FREQ / 62;
        break;
    default:
        if (note5 <= B)
            period5 = lowerNotePeriods[note5];
        break;
    }

    switch (note6)
    {
    case Rest:
        // It shouldn't matter what the period is, as long as the total cycles normalizes to the proper
        // note length.  We just want the Rest to be silent for the correct length of time.
        period6 = CLOCK_FREQ / 62;
        break;
    default:
        if (note6 <= B)
            period6 = lowerNotePeriods[note6];
        break;
    }
 
    enum MusicalNoteLength noteLength = notePlus & MUSICAL_LENGTH_MASK;
    unsigned long length = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(noteLength);

    enum MusicalNoteLength note2Length = note2Plus & MUSICAL_LENGTH_MASK;
    unsigned long length2 = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(note2Length);

    enum MusicalNoteLength note3Length = note3Plus & MUSICAL_LENGTH_MASK;
    unsigned long length3 = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(note3Length);

    enum MusicalNoteLength note4Length = note4Plus & MUSICAL_LENGTH_MASK;
    unsigned long length4 = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(note4Length);

    enum MusicalNoteLength note5Length = note5Plus & MUSICAL_LENGTH_MASK;
    unsigned long length5 = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(note5Length);

    enum MusicalNoteLength note6Length = note6Plus & MUSICAL_LENGTH_MASK;
    unsigned long length6 = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(note6Length);
 
    // We need to adjust the period by the octave (and a preferred scaling value)
    // Also, we want the note to play for the precise length of time regardless of the period
    // so we have to adjust the number of cycles by the period
    unsigned long adjustedPeriod = period / ipow(2, findOctave(notePlus & OCTAVE_NOTE_MASK)) / PERIOD_SCALE;
    unsigned long adjustedPeriod2 = period2 / ipow(2, findOctave(note2Plus & OCTAVE_NOTE_MASK)) / PERIOD_SCALE;
    unsigned long adjustedPeriod3 = period3 / ipow(2, findOctave(note3Plus & OCTAVE_NOTE_MASK)) / PERIOD_SCALE;
    unsigned long adjustedPeriod4 = period4 / ipow(2, findOctave(note4Plus & OCTAVE_NOTE_MASK)) / PERIOD_SCALE;
    unsigned long adjustedPeriod5 = period5 / ipow(2, findOctave(note5Plus & OCTAVE_NOTE_MASK)) / PERIOD_SCALE;
    unsigned long adjustedPeriod6 = period6 / ipow(2, findOctave(note6Plus & OCTAVE_NOTE_MASK)) / PERIOD_SCALE;

    _makeSound(length / (MAX((adjustedPeriod + adjustedPeriod2 + adjustedPeriod3 + adjustedPeriod4 + adjustedPeriod5 + adjustedPeriod6), 1)),
            adjustedPeriod, note == Rest ? true : false,
            adjustedPeriod2, note2 == Rest ? true : false,
            adjustedPeriod3, note3 == Rest ? true : false,
            adjustedPeriod4, note4 == Rest ? true : false,
            adjustedPeriod5, note5 == Rest ? true : false,
            adjustedPeriod6, note6 == Rest ? true : false);
 
    __delay_ms(16);
}