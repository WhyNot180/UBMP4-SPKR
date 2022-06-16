# Journal

## Programming The Beeper

---

### **May 25**

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

### **May 26**

Today I received a surprise present from Mr. Lam! He gave me the bluetooth module I was looking for much earlier than expected *and* a bigger piezo beeper. Perhaps I could
integrate it into my project if I have some extra time. In other news, I have made a great discovery regarding domains! The master counter works! I implemented this using
the greatest common divisor of the periods of all the notes, however, this also results in the note not having any rhythm to it, which I need to fix. There are more exciting
things I have discovered though! I realized that the effect counter effectively acts as a divisor of the waveform meaning that if I set it to "1" then I have a 50% duty cycle,
and if I set it to 2 then it is a 25% duty cycle (and so on). This means I can effectively change the timbre of the note! Additionally, if I were to decrease the duty cycle
enough then it may be used to change through volume as well! Now that I think about it, it feels kind of obvious, but I find that discovering these things on my own is
still fun :p

### **May 27**

Today was a mostly uneventful day. The most I did was *attempt* to de-solder a header from the bluetooth module. The result was a module was without any header and holes
plugged with solder. Before I finished, I had to leave for my next class and Mr. Lam said that he would take care of it for me (thanks). Other than that it was a pretty uneventful
day.

### **May 28**

Today's work was primarily focused on figuring out how to implement note durations and how to get the master counter to work properly. Before, the master counter caused the note to
output a continuous sound regardless of the other domains. Initially, I thought that I should use the lowest common multiple as the counter, however, this made it take far too long to
play any note, instead, I decided to make it 220kHz (clock frequency / 220000), as this happens to the sampling speed ProtoDome uses in his 1-bit generator for PIM. This worked marvelously!
After I did that, I discovered that instead of affecting the duration of the note, as I had initially expected, it instead made a sort of ringing sound (similarly to a phone's ring-tone)
and changed the amount of "rings" heard. After doing some more experimentation, I learned that this was actually caused by the effect domain! This means that, while it can help give a more pleasant
tone, the way it is currently implemented creates that ringing tone. After doing some digging I saw that this was actually mentioned in Dr. Troise's paper. It seems I shall have to come up
with a different way of changing the timbre or the duration, perhaps I could combine the pitch and effect domain bits? That would require more processing power though. Hmm, decisions, decisions.

### **May 29**

This time my primary focus was on getting the effect and rhythm domains to comply with each other. I managed to get the effect domain to work without messing up the rhythm domain and I also discovered
some bugs related to rests being played when they were not suppsosed to, which were fixed. The main issue I now face is getting rhythm to function *well*. It uses the master counter to 
determine when to switch the rhythm bit on and off, but it is very inconvenient to try and figure out the specific values for the right length of note. This is what I must work on the next time
I am working on programming. Most likely tomorrow I will be working on soldering and unsoldering the bluetooth module and the voltage divider, it's a bit ahead of schedule, but the sooner the better.
An additional note on implementing the rhythm domain, while it *does* work currently, there does seem to be a very subtle beat frequency which will require more investigating to eliminate.

### **May 30**

Another day focused on in-school work. It was soldering once more, and I managed to solder on an expansion header and Mr. Lam helped me unsolder the resistors connected to the Tx and Rx pins.
I initially wasn't too sure how to get the solder off without damaging anything, but Mr. Lam came to the rescue again and showed me that heating up the solder and then smacking the board will do the job
just fine. Other than that there wasn't anything else done, so this was mostly a 'break' day for helping me organize my thoughts. Tomorrow shall be focused on getting the solder out of the holes
and getting a bread board ready for testing if I have the available time. After school it shall mostly be focused on cleaning up my code-base.

### **May 31**

I forgot there were 31 days in May. So, the cleanup is actually going to be tomorrow, which is somewhat convenient considering that I stayed to help with a surprise party for Mr. Moore. 
The only thing I did was complete a prototype of the complete circuit on a bread board. In other words, I just need to program it to get it ready for testing. I didn't do much else today. Tomorrow
in school time will be focused on testing the bluetooth module and, afterwards, a giant cleanup of my code base since it is quite ugly at the moment. Oh, I almost forgot, but I also realized that the total
length of the Rhythm domain is equal to: master counter * rhythm counter. 

### **June 1**

THE GREAT FLOOD TO WASH AWAY ALL SINS IS HERE!! In all seriousness, I managed to clean up my code and it is ***so*** much more *<span style="font-family:Brush Script MT;">beautiful</span>* now.
The best part? It still works! I was worried that I might not have enough time to clean up my code, but luckily I did, and man was it worth it. I literally went from using 48% program space and
49.6% data space to 28.7% program space and 37.7% data space being used! That's a difference of about 19 and 11!! Anyways, tomorrow will be focused on programming the bluetooth module to work and
testing it to see if it actually works. Also, the one I currently have is an HC-08 not an HC-06 or HC-05, meaning it works with apple products, which is a plus. Also, it's bluetooth low energy
and not standard bluetooth, so it should use less overall energy.

### **June 2**

This bluetooth stuff has succeeded in making me thoroughly confused. First off, to get the baud rate you need to use a formula to find the value you need to put into a register, but guess what?
THERE'S TWO REGISTERS. This means it has to be a ratio, but I have no clue what that would be, and which register corresponds to which. So, I just winged it by having one be 0 and the other the
correct value. However, whenever I write a value from my phone to the module, it is a completely different value! I don't even know what it's receiving! I tried an echo test and it worked perfectly
fine, but when I connect it to the UBMP4 it doesn't receive the correct value. I don't know if this is due to a faulty module (unlikely) or an incorrect baudrate. Also, whenever I try to get a value 
from the UBMP4 I only get 0xF8 and after changing the baud rate it changed to 0xFE. Tomorrow will be getting Mr. Lam to help me figure this out.

### **June 3**

I had a great amount of frustration today. I managed to narrow it down to a baud rate issue, so then I had to figure out a ratio. Plot twist, there is no ratio! It is literally just two hexadecimal
numbers stitched together so that SPBRGH is the most significant 2 digits and SPBRGL is the two least significant digits. I also happened to learn that the max value for SPBRGH is 111 (0x6F) and 232 (0xE8)
for SPBRGL. The value I needed (1249) is equal to 0x04E1, so SPBRGH is 4 and SPBRGL is 225. After I did this everything worked perfectly! Well, on the transmitting end that is. On the receiving end I was
still having a some issues. Luckily it turned out that I had interrupts on and it was messing with everything. In short, I learned a valuble lesson today: data sheets need to be more specific, and interrupts
are not worth it. Tomorrow shall consist of me trying to combine the bluetooth aspects and the music aspects.

### **June 4**

I managed to do it! There were some problems regarding syntax, in fact, today I learned that not only does the compiler not generate an error when you call a function without parentheses, but it makes the EUSART
spit out absolute gibberish! Even when you're only trying to access the input register! I also learned that the compiler is does not like using char in for loops especially when it's going into negatives ("comparison
with constant -1 is never resolved, causes an infinite loop"). Luckily I managed to fix all of that and managed to change rhythm and effect (manually of course) while playing music! The only issue I have currently is
that I get requests for multiple channels at the same time, which is very strange and worrying, as those are only asked for in seperate 'if' statements!! That's what I'll be working on tomorrow, after that
I just need to create an app to do all the calculations and also spit out pitch values. In other words, I am ahead of schedule (yay)!!

## **June 5**

Huzzah! I have fixed it! Turns out that it is very picky about byte arrays and sending more than 8 bits at a time. Unfortunately, this discovery *will* force me to make it slower, but it should still be (hopefully) fast enough.
The only way to know is to start working on that application to interact with the board. Other than that, everything is peachy. I also added more comments and removed some legacy code that was used to calculate the length of each note.
There is only one thing left on the bucket list is starting on creating the application I need (I'm going to pretend that usb doesn't exist until I finish bluetooth).

### **June 8**

I forgot to update the journal, oops! Oh well, not a lot of interesting things happened during that period. It was mostly just me messing around with the board in class and trying to figure out if I can get onto something other than a
breadboard because, quite honestly, a breadboard isn't a very permanent or visually appealing way to create my circuit. Other than that I've installed android studio and am watching a 4 hour tutorial on how to make an android application.
So far, I've learned what views and layouts are, about attributes, a little bit about XML, and some stuff about buttons. Tomorrow will be more of the same.

### **June 9**

Today I burned myself while trying to unsolder the buzzer on the board. Not much else happened today. I'll try soldering on the big buzzer with some wires tomorrow.

### **June 10**

I wasn't able to unsolder the buzzer on my own so Mr. Lam ended up helping me (again). After that I haphazardly soldered the wires onto the buzzer and then onto the board. Tomorrow will be me continuing to try and learn android development and
android studio.

### **June 13**

Not much happened during previous days, as I was working on projects in other classes. Today I discovered that the point at which notes start blending into each other because of the lack of an FPU is at G6. I also discovered that the 6th octave is
optimal for making out individual notes in the c major chord. However, this may not be the case, as the sound from the initial chord is different from that which is supplied through bluetooth. In other words, the chord I hard-coded in sounds better than
the values supplied through bluetooth, I am not sure as to why this is and will be working on a solution tomorrow. Other than that I was learning more about android development, in a few more days I may be able to begin making the android app :)

### **June 15**

Today was focused on android development. I think I have almost learned enough to start proper development on the application. I've looked into the android libraries for bluetooth low-energy and they look pretty straight forward to use. I'm just a little
unsure if I can get everything finished in time for the showcase on the 24th. Well, time to find out later, right now I've got work to do!