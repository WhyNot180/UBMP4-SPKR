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

Today I received a surprise present from Mr. Lam! He gave me the bluetooth module I was looking for much earlier than expected *and* a bigger piezo beeper. Perhaps I could
integrate it into my project if I have some extra time. In other news, I have made a great discovery regarding domains! The master counter works! I implemented this using
the greatest common divisor of the periods of all the notes, however, this also results in the note not having any rhythm to it, which I need to fix. There are more exciting
things I have discovered though! I realized that the effect counter effectively acts as a divisor of the waveform meaning that if I set it to "1" then I have a 50% duty cycle,
and if I set it to 2 then it is a 25% duty cycle (and so on). This means I can effectively change the timbre of the note! Additionally, if I were to decrease the duty cycle
enough then it may be used to change through volume as well! Now that I think about it, it feels kind of obvious, but I find that discovering these things on my own is
still fun :p

### May 27

Today was a mostly uneventful day. The most I did was *attempt* to de-solder a header from the bluetooth module. The result was a module was without any header and holes
plugged with solder. Before I finished, I had to leave for my next class and Mr. Lam said that he would take care of it for me (thanks). Other than that it was a pretty uneventful
day.

### May 28

Today's work was primarily focused on figuring out how to implement note durations and how to get the master counter to work properly. Before, the master counter caused the note to
output a continuous sound regardless of the other domains. Initially, I thought that I should use the lowest common multiple as the counter, however, this made it take far too long to
play any note, instead, I decided to make it 220kHz (clock frequency / 220000), as this happens to the sampling speed ProtoDome uses in his 1-bit generator for PIM. This worked marvelously!
After I did that, I discovered that instead of affecting the duration of the note, as I had initially expected, it instead made a sort of ringing sound (similarly to a phone's ring-tone)
and changed the amount of "rings" heard. After doing some more experimentation, I learned that this was actually caused by the effect domain! This means that, while it can help give a more pleasant
tone, the way it is currently implemented creates that ringing tone. After doing some digging I saw that this was actually mentioned in Dr. Troise's paper. It seems I shall have to come up
with a different way of changing the timbre or the duration, perhaps I could combine the pitch and effect domain bits? That would require more processing power though. Hmm, decisions, decisions.

### May 29

This time my primary focus was on getting the effect and rhythm domains to comply with each other. I managed to get the effect domain to work without messing up the rhythm domain and I also discovered
some bugs related to rests being played when they were not suppsosed to, which were fixed. The main issue I now face is getting rhythm to function *well*. It uses the master counter to 
determine when to switch the rhythm bit on and off, but it is very inconvenient to try and figure out the specific values for the right length of note. This is what I must work on the next time
I am working on programming. Most likely tomorrow I will be working on soldering and unsoldering the bluetooth module and the voltage divider, it's a bit ahead of schedule, but the sooner the better.
An additional note on implementing the rhythm domain, while it *does* work currently, there does seem to be a very subtle beat frequency which will require more investigating to eliminate.

### May 30

Another day focused on in-school work. It was soldering once more, and I managed to solder on an expansion header and Mr. Lam helped me unsolder the resistors connected to the Tx and Rx pins.
I initially wasn't too sure how to get the solder off without damaging anything, but Mr. Lam came to the rescue again and showed me that heating up the solder and then smacking the board will do the job
just fine. Other than that there wasn't anything else done, so this was mostly a 'break' day for helping me organize my thoughts. Tomorrow shall be focused on getting the solder out of the holes
and getting a bread board ready for testing if I have the available time. After school it shall mostly be focused on cleaning up my code-base.

### May 31

I forgot there were 31 days in May. So, the cleanup is actually going to be tomorrow, which is somewhat convenient considering that I stayed to help with a surprise party for Mr. Moore. 
The only thing I did was complete a prototype of the complete circuit on a bread board. In other words, I just need to program it to get it ready for testing. I didn't do much else today. Tomorrow
in school time will be focused on testing the bluetooth module and, afterwards, a giant cleanup of my code base since it is quite ugly at the moment. Oh, I almost forgot, but I also realized that the total
length of the Rhythm domain is equal to: master counter * rhythm counter. 