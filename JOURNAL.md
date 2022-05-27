# Journal

## Programming The Beeper

---

### May 25

This is the start of my journal of my journey into 1-bit music. I already have some knowledge prior to the beginning
of this journal. I am currently attempting to increase the quality of my sound and gain more control over the length
and timbre of the notes. To achieve this I am trying out a different approach from before. It is in a kind of fractal
where the various frequency domains can be individually controlled.

This includes:

- The Rhythmic domain that controls the main rhythm of the note
- The Effect domain, which allows control over the timbre and thereby *effect* of the sound
- The Pitch domain that controls the *perceived* pitch of a note (psycho-acoustics is weird)
- The Sample domain, which is the most fundamental of the four, this is where the music is updated

I have combined them in a structure similar to this one:

	if (pitchCounter-- == 0) {
		pitchCounter = frequency;
		output1 ^= 1;

		if (effectCounter-- == 0) {
			effectCounter = effect;
			output2 ^= 1;

			if (rhythmCounter-- == 0) {
				rhythmCounter = rhythm;
				output3 ^= 1;
			}
		}
	}
	BEEPER = output1 & output2 & output3;

This makes sound only play once all outputs are *on* thereby making it have it's distinct pitch, timbre, and rhythm. I can thank Dr. Blake Troise
(a.k.a ProtoDome) and his website and paper on this subject can be found [here](https://phd.protodome.com/).

Unfortunately, this alone does not allow for effective pulse interleaving, as each note is out of sync, but it shall provide a solid basis.
I suspect I might be able to have a master counter of sorts to process the effect and rhythm domains seperately of the pitches.

### May 26