=================================================================================
PicoMite V5.07.05b13
Adds support for the 480x320 IPS ILI9341 display (use code ILI9341N)
Modifies the port function to read all pins simultaneously
Updates SDK to V1.4
Updates Compiler to version 11.2.1

PicoMite V5.07.05b11
Fixes bug when using "ON KEY int" command
Enables operation up to 378MHz

PicoMite V5.07.05b9
Fixes bug in SDK that caused SPI clock to set to wrong level before first transaction
Improved error checking of SETPIN command to avoid crashes
SETPIN for PWM no longer needs to specify the channel and port e.g. "SETPIN 1,PWM" is now accepted

PicoMite V5.07.05b8
Fixes crashing bug if edit is used after running a program with IR input
Removes support for the GDEH029A1 display as this controller is now obsolete
Reduces available RAM for VGA version from 108Kb to 104Kb to solve memory corruption issue

PicoMite V5.07.05b7
Fixed bug in PWM n,OFF requiring spurious extra parameter
Fixed bug in sound command overdriving the PWM when > 2 channels are used and the volume isn't explicitly specified

PicoMite V5.07.05b6
Re-engineering of PORT command to allow simultaneous update

PicoMite V5.07.05b5
Implementation of sprite command and function
Use EDIT 1 to edit in mode 1, Use EDIT 2 to edit in mode 2, Use EDIT to edit in current mode
Change timing of PAGE COPY ,,B to avoid screen artefacts

PicoMite V5.07.05b3
Fixes bug in BITBANG LCD CMD and BITBANG LCD DATA

PicoMite V5.07.05b2
Fixes issue on some Pico in VGA mode 1 output at 126MHz

PicoMite V5.07.05b1
Fixes bug in PIO READ
Allows a single integer variable to be used when nbr=1 in PIO READ
Implements DRAW3D command and function (VGA version only)
Implements FRAMEBUFFER command (VGA version only)



