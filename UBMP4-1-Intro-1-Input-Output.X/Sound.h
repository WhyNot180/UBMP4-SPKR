#ifndef SOUND_H
#define SOUND_H

const unsigned short notes[5][12];

void var_delay_us(unsigned short microseconds);

void play_note(unsigned short note, unsigned short duration);

#endif