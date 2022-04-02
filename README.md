# UBMP4-Music-Player

## What is this project about?

---

This is a project for the UBMP4, designed to play relatively complex music, created via Microchip's MPLAB Xpress IDE.

## Who is this project for?

---

This is a personal project, however, anyone who would like to use this code, or modify a copy, is welcome to do so.

## Project limitations

---

The UBMP4 uses a PIC16F1459 microcontroller, notable features include: 

- 8k words of program FLASH (6k when using USB bootloader)
- 128B of user FLASH
- 1kb of RAM

The UBMP4 has one built-in piezo buzzer, more can potentially be added, but in this project we will only be using the one.
More information can be found at the [UBMP4 website](https://mirobo.tech/ubmp4) and [Microchip's site for the microcontroller](https://www.microchip.com/en-us/product/PIC16F1459)

## Features

---

### Sound creation

The sound is created by toggling the buzzer on and of at a specified frequency with a delay between each note in order to accentuate it. The length of these notes is determined by a 'for' statement.[^1]

To play multiple notes at once you must put the notes in the same statement.[^2]

Each frequency is recorded in a table.[^3]

[^1]: It is important to note that each delay will add up to the duration of the note and can change the integer required for the statement. e.g. C4 has a delay of 3822uS while D4 has a delay of 3405, therefore D4 will need to be played slightly more than C4 to last the same duration.

[^2]: This will add up to be significantly longer than a single note.

[^3]: | Note | Array Pos | Frequency (Hz) | Frequency (uS) | 
|---|---|---|---|
| C2 | 0 | 65.41 | 15288 |
