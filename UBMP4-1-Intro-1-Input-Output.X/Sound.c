#include    "xc.h"
#include    "stdbool.h"
#include    "UBMP410.h"
#include    "Sound.h"
 
#define MIN(x, y) x < y ? x : y
#define MAX(x, y) x < y ? y : x
 
 
unsigned long ipow(unsigned long base, unsigned char exp) {
    unsigned long result = base;
    for (unsigned char i = 1; i < exp; i++) {
        result *= base;
    }
    return result;
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
 
void _makeSound(unsigned long cycles, unsigned long period, bool silent, unsigned long period2, bool silent2, unsigned long period3, bool silent3)
{
    unsigned long truePeriod;
    unsigned char alternate;
    bool trueSilent;
    for (unsigned int c = 0; c < cycles; c++)
    {
        alternate = c % 6;
        switch (alternate){
            case (0):
                truePeriod = period3;
                trueSilent = silent3;
                break;
            case (1):
                truePeriod = 0;
                trueSilent = true;
                break;
            case (2):
                truePeriod = period;
                trueSilent = silent;
                break;
            case (3):
                truePeriod = 0;
                trueSilent = true;
            case (4):
                truePeriod = period2;
                trueSilent = silent2;
            case (5):
                truePeriod = 0;
                trueSilent = true;
            default:
                break;
        }
        if (!trueSilent) BEEPER = !BEEPER;
        for (unsigned long p = 0; p < truePeriod; p++);
    }
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
 
    enum MusicalNoteLength noteLength = notePlus & ~MUSICAL_LENGTH_MASK;
    unsigned long length = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(noteLength);

    enum MusicalNoteLength note2Length = note2Plus & ~MUSICAL_LENGTH_MASK;
    unsigned long length2 = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(note2Length);

    enum MusicalNoteLength note3Length = note3Plus & ~MUSICAL_LENGTH_MASK;
    unsigned long length3 = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(note3Length);

    enum MusicalNoteLength note4Length = note4Plus & ~MUSICAL_LENGTH_MASK;
    unsigned long length4 = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(note4Length);

    enum MusicalNoteLength note5Length = note5Plus & ~MUSICAL_LENGTH_MASK;
    unsigned long length5 = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(note5Length);

    enum MusicalNoteLength note6Length = note6Plus & ~MUSICAL_LENGTH_MASK;
    unsigned long length6 = SIXTEENTH_NOTE_DURATION_CYCLES * findLength(note6Length);
 
    // We need to adjust the period by the octave (and a preferred scaling value)
    // Also, we want the note to play for the precise length of time regardless of the period
    // so we have to adjust the number of cycles by the period
    unsigned long adjustedPeriod = period / ipow(2, currentOctave) / PERIOD_SCALE;
    unsigned long adjustedPeriod2 = period2 / ipow(2, secondaryOctave) / PERIOD_SCALE;
    unsigned long adjustedPeriod3 = period3 / ipow(2, tertiaryOctave) / PERIOD_SCALE;
    _makeSound(length / (adjustedPeriod + adjustedPeriod2 + adjustedPeriod3), adjustedPeriod, note == Rest ? true : false,
            adjustedPeriod2, note2 == Rest ? true : false,
            adjustedPeriod3, note3 == Rest ? true : false);
 
    __delay_ms(16);
}