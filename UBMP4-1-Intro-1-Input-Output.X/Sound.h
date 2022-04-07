#ifndef SOUND_H
#define SOUND_H

unsigned int pow(unsigned int base, char power);

const unsigned short notes[5][12];

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
const unsigned short C3 = 7644;
const unsigned short Cs3 = 7214;
const unsigned short D3 = 6810;
const unsigned short Ds3 = 6427;
const unsigned short E3 = 6067;
const unsigned short F3 = 5726;
const unsigned short Fs3 = 5405;
const unsigned short G3 = 5101;
const unsigned short Gs3 = 4815;
const unsigned short A3 = 4545;
const unsigned short As3 = 4290;
const unsigned short B3 = 4049;
const unsigned short C4 = 3822;
const unsigned short Cs4 = 3607;
const unsigned short D4 = 3405;
const unsigned short Ds4 = 3214;
const unsigned short E4 = 3034;
const unsigned short F4 = 2864;
const unsigned short Fs4 = 2703;
const unsigned short G4 = 2551;
const unsigned short Gs4 = 2408;
const unsigned short A4 = 2273;
const unsigned short As4 = 2146;
const unsigned short B4 = 2025;
const unsigned short C5 = 1912;
const unsigned short Cs5 = 1805;
const unsigned short D5 = 1703;
const unsigned short Ds5 = 1608;
const unsigned short E5 = 1518;
const unsigned short F5 = 1433;
const unsigned short Fs5 = 1353;
const unsigned short G5 = 1277;
const unsigned short Gs5 = 1205;
const unsigned short A5 = 1137;
const unsigned short As5 = 1074;
const unsigned short B5 = 1014;
const unsigned short C6 = 957;
const unsigned short Cs6 = 904;
const unsigned short D6 = 853;
const unsigned short Ds6 = 805;
const unsigned short E6 = 760;
const unsigned short F6 = 718;
const unsigned short Fs6 = 678;
const unsigned short G6 = 636;
const unsigned short Gs6 = 604;
const unsigned short A6 = 570;
const unsigned short As6 = 539;
const unsigned short B6 = 509;

void var_delay_us(unsigned long microseconds);

void play_note(unsigned long note, unsigned long duration);

#endif