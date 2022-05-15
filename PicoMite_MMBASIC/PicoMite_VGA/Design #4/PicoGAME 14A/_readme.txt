The dsn file is for TinyCAD and is the schematic diagram. TinyCAD is free.


The lay6 file is the pcb layout drawing in Sprint Layout 6.0 file.
A free viewer is available here:
https://www.electronic-software-shop.com/lng/en/support/free-viewer-software/?language=en

Many thanks to Geoff Graham for his excellent MMBasic and to Peter Mather for his hard work in porting it (and widely extending it!) for the Raspberry Pi Pico to produce the PicoMite and then the PicoMite VGA Edition. Thanks also to Raspberry Pi, who I think have produced more than originally expected in the Pico. Thanks also to the folks on The Back Shed Forum, which is the spiritual home of everything to do with MMBasic.


Mick - 26/04/22



CHANGES FOR THE 14A VERSION

These are NOT noted in the Construction Notes or in the Bill of Materials.

A Two position link has been added to allow the incoming supply to go to either side of D1. This is to allow marginal 5V supplies, where the voltage drop of D1 may not be acceptable. It is also a useful place to take supply current readings.

Positions for output capacitors and bleed resistors have been added for the audio. Nominal values of 100uF and 2K2 have been shown, but these may vary with the load. They should be ok for 32R headphones. The bleed resistors ensure that the capacitor is charged at switch-on so that if headphones are plugged in the pop sound is much reduced. If only line output level is used the capacitors could be reduced to about 2.2uF and the resistors increased to 47k or more.This has not been tested.

