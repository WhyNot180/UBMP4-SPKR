#ifndef SOUND_H
#define SOUND_H

// bits 1-3 are for the rhythm index (as initialized at the start of the song)
// bit 4 is for indicating whether the note should be treated as silent or not
#define RHYTHM_MASK 0b00000111
// bits 5-8 are for the timbre
#define EFFECT_MASK 0b11110000

// Struct for initial values at the start of song
struct Song
{
    unsigned long periods[3];
    uint8_t silents[3];
    unsigned char rhythmLengths[8];
    unsigned char silentRhythm;
    unsigned char firstEffects[3];
    unsigned int firstRhythms[3];
};

/**
 * Play a musical note
 *
 * @param notePlus a combined value that represent the MusicalNote and MusicalNoteLength.  
 * For example, a half note G can be encoded as notePlus = G | HalfNote
 */
void playNote(void);
 
/**
 * Make a noise on the buzzer with the given params a number of times (nTimes)
 **/
void _makeSound(struct Song song);

#endif