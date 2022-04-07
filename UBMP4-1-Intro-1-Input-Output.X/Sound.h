#ifndef SOUND_H
#define SOUND_H

const unsigned short notes[5][12];

const unsigned short C2;
const unsigned short Cs2;
const unsigned short D2;
const unsigned short Ds2;
const unsigned short E2;
const unsigned short F2;
const unsigned short Fs2;
const unsigned short G2;
const unsigned short Gs2;
const unsigned short A2;
const unsigned short As2;
const unsigned short B2;
const unsigned short C3;
const unsigned short Cs3;
const unsigned short D3;
const unsigned short Ds3;
const unsigned short E3;
const unsigned short F3;
const unsigned short Fs3;
const unsigned short G3;
const unsigned short Gs3;
const unsigned short A3;
const unsigned short As3;
const unsigned short B3;
const unsigned short C4;
const unsigned short Cs4;
const unsigned short D4;
const unsigned short Ds4;
const unsigned short E4;
const unsigned short F4;
const unsigned short Fs4;
const unsigned short G4;
const unsigned short Gs4;
const unsigned short A4;
const unsigned short As4;
const unsigned short B4;
const unsigned short C5;
const unsigned short Cs5;
const unsigned short D5;
const unsigned short Ds5;
const unsigned short E5;
const unsigned short F5;
const unsigned short Fs5;
const unsigned short G5;
const unsigned short Gs5;
const unsigned short A5;
const unsigned short As5;
const unsigned short B5;
const unsigned short C6;
const unsigned short Cs6;
const unsigned short D6;
const unsigned short Ds6;
const unsigned short E6;
const unsigned short F6;
const unsigned short Fs6;
const unsigned short G6;
const unsigned short Gs6;
const unsigned short A6;
const unsigned short As6;
const unsigned short B6;

void var_delay_us(unsigned short microseconds);

void play_note(unsigned short note, unsigned short duration);

#endif