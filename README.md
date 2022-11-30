# Description

I 3D printed portal radio and people might want recreate it so I'm making code public for everyone else.

# Required parts

- esp32 - I used esp32 lite model.
- 3.3V stepdown - for esp and amp. I used some MP1484EN based circuit but it doesn't really matter, make it take 4-6V input and spit out 1A
- Some resistors, I think I used 220Ω for blue and something bigger for red but i can't remember
- 20 blue leds - I think I used less.
- 1 red led - It would be better to use RGB for green/red switching but I didn't have one
- 1 speaker - I used some random 0.5W 8Ω speaker, most of them should work as our amp is pretty capable
- 2 switches
- 4x AA batteries case
- MAX98357 DAC AMP combo
- microSD to SD adapter - at first I used some pcb with microsd reader but it was 5V so i wired directly to sd adapter (you can still pull microsd) because I already have 3.3V regulated

# Wiring 

I will maybe possibly do some schematic later but I don't have any software on hand so not yet

But basically all leds are parallel, resistor then led, all powered directly from battery (4-6V). 

Then from battery is also powered 3.3V step-down. it turns off at 4.5V so it will turn off when batteries get discharged.

From 3.3V step-down, powered are esp32 and amp.

Amp is connected via i2s, gain and SD are left unconnected (i had some issues, you can connect gain and tie SD HIGH for left channel as my MAX pcb only had left channel (and I used only left speaker).

SDcard is connected to esp32 + power from step-down.

There are two switches, one cuts off power from battery (for turning it on/off) and other one is INPUT_PULLUP so connected from pin to ground that controls mode in which radio is in.

You can wire potentiometer for volume control but it caused me some issues so I removed it.

# Usage

There are two modes switched by one switch (restart required after switching because I didn't bother).

In normal, loop mode, it loops file "looping_radio_mix_1min.wav" which is normal looping_radio_mix.wav pulled from game files just repeated 2 times because even though music file is looped there is tiny pause between loops and that makes the pause super hard to notice. Also in this mode if BT device is detected nearby it will switch to looping "dinosaur_01.wav" which is pulled from portal 2 game files and decoded produces image of companion cube on the moon. I don't include those files as they're proprietary but looping mix is easy to find on internet and both can be easily extracted from portal 1 and 2.

If you switch modes it goes through all tracks on sdcard, skipping the system tracks (loop and dinosaur). There's no next/prev/play/pause/volume it just goes. Also if you put anything make sure its mono (doesn't necessarily have to be but avoid just to issue of playing only left channel) and make them wav files as they're already decoded and you don't bother poor esp with decoding (although the library can handle mp3/flac it seems i wouldn't do it). In this mode hidden transmission won't kick in.
