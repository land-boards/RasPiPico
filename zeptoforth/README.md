# zeptoforth

zeptoforth is a Cortex-M Forth, currently targeted at the Raspberry Pi Pico (but it should also work with other RP2040 boards using compatible Winbond Quad SPI flash) and the STM32L476, STM32F407, and STM32F746 DISCOVERY boards. Ports to more platforms are on hold due to the current chip shortage (aside from a possible port to an STM32H7 board I have lying around somewhere).

API documentation is available [on GitHub](https://github.com/tabemann/zeptoforth/tree/master/docs/words) and in the docs directory on the build tarballs (which is the same as the html directory in git and in the source tarballs available on GitHub generated from that, which is built from the API documentation in Markdown format in the docs directory in git). There is also a [wiki on GitHub](https://github.com/tabemann/zeptoforth/wiki) outlining zeptoforth and providing useful guides to it.

Its kernel has versions written in Thumb-1 assembly, for the Raspberry Pi Pico, and Thumb-2 assembly, for the STM32L476, STM32F407, and STM32F746 DISCOVERY boards. and there is a body of other core code that is loaded after it is loaded which is written in Forth.

The library of code included along with the zeptoforth kernel, which is present in its full form in `full` and `full_swdcom` builds, includes the following:

* A priority-scheduled preemptive multitasker
* Semaphores
* Locks, with priority inversion handling
* Message-oriented queue channels
* Message-oriented rendezvous channels, aka "fchannels"
* Message-oriented bidirectional synchronous reply channels, aka "rchannels"
* Byte-oriented streams
* Interrupt service handler-safe channels, aka "schannels"
* Task notifications
* Action scheduler support
* Multicore support (on the Raspberry Pi Pico)
* Double cell and fixed point numeric support
* Lambda expressions
* A disassembler
* SysTick support
* User-reconfigurable processor exception vector support
* Interrupt-driven serial IO
* GPIO support (including EXTI support on STM32 microcontrollers)
* Maps, including counted string and integer-keyed maps
* Heap allocators
* Memory pool allocators
* Task pool allocators
* Action pool allocators
* Temporary storage allocators (used to provide immediate string constants)
* A line editor
* LED drivers
* Quad SPI flash storage support (on the STM32F746 DISCOVERY board and the Raspberry Pi Pico)
* A block editor (on the STM32F746 DISCOVERY board and the Raspberry Pi Pico)
* Random number generator support
* Pseudorandom number generator support (using the TinyMT32 PRNG)
* Programmable input/output support (on the Raspberry Pi Pico)
* Optional swdcom support

To load the zeptoforth image (whether just the kernel or an image including precompiled Forth code) onto an STM32L476, STM32F407, or STM32F746 DISCOVERY board, first install st-flash, then attach the DISCOVERY board to one's PC via USB and execute:

    $ st-flash erase
    $ st-flash write <location of the zeptoforth image> 0x08000000
    $ st-flash reset

Note the address referred to above. This will also reboot the board.

To load the zeptoforth image (whether just the kernel or an image including precompiled Forth code) onto a Raspberry Pi Pico board, hold down the BOOTSEL button while connecting the Raspberry Pi Pico to one's computer via USB. This will result in a USB Mass Storage device appearing in one's `/dev` directory, and if supported by one's system, automatically mounted. Then one can copy the appropriate UF2 file to the USB Mass Storage device, which will automatically cause it to be loaded into flash and then executed.

Prebuilt binaries are in `bin/<version>/<platform>/` in release tarballs. They are not included in source code-only zips or tarballs, or in the git repository.

\<Location of the zeptoforth image> is either:

* a freshly built `zeptoforth.<platform>.{bin, uf2}` file in the root directory of zeptoforth
* `zeptoforth_kernel-<version>.{bin, uf2}` (without precompiled Forth code)
* `zeptoforth_<type>-<version>.{bin, uf2}` (with full precompiled Forth code)

where \<type> is one of:

* `full` (full functionality compiled in except for swdcom support with a cornerstone to enable resetting functionality back to "factory" settings)
* `full_swdcom` (full functionality compiled in including swdcom support with a cornerstone to enable resetting functionality back to "factory" settings)
* `mini` (i.e. without fixed number, allocator, scheduler, or disassembler support, without swdcom support)
* `mini_swdcom` (i.e. without fixed number, allocator, scheduler, or disassembler support, including swdcom support)

and where \<platform> is one of

* `stm32f407`
* `stm32f746`
* `stm32l476`
* `rp2040`

Note that for the `rp2040` platform, to load code with the bootloader onto the Raspberry Pi Pico one needs a `.uf2` file rather than a `.bin` file, unlike the other platforms, which will be located in the same location. Note that these files contain a boot block with a CRC32 checksum. Also note that from release 0.35.0 on the `rp2040` platform does not have the `full_swdcom` or `mini_swdcom` builds as swdcom does not properly function on this platform.

To build the kernel for each of the supported platforms, one first needs to install the gas and binutils arm-none-eabi toolchain along with Python 3.9 or later, and then execute:

    $ make

to use the default version or:

    $ make VERSION=<version>

This build a `zeptoforth.<platform>.bin`, a `zeptoforth.<platform>.ihex`, and a `zeptoforth.<platform>.elf` file for each supported platform. Additionally a `zeptoforth.rp2040.uf2` file will be built for the rp2040 platform. The `zeptoforth.<platform>.elf` file is of use if one wishes to do source debugging with gdb of the zeptoforth kernel, otherwise disregard it.

To use the board on Linux, download and install [e4thcom](https://wiki.forth-ev.de/doku.php/en:projects:e4thcom), [swdcom](https://github.com/crest/swdcom), [GNU Screen](https://www.gnu.org/software/screen/), or [picocom](https://github.com/npat-efault/picocom). To use the board on xBSD, the same applies, except that e4thcom does not work under xBSD. To use the board on Windows, it is highly recommend one uses [escom](https://github.com/Edzelf/escom), which provides e4thcom-like functionality for Windows.

The following applies if one is using e4thcom: If one is using an STM32F407 DISCOVERY or Raspberry Pi Pico board, attach a USB-to-serial converter to your machine (make sure you have the proper permissions to access its device file) and, for the STM32F407 DISCOVERY board, attach the RXD pin on the converter to PA2 on the board and the TXD pin on the converter to PA3 on the board or, for the Raspberry Pico, attach the RXD pin on the converter to GPIO0 on the board and the TXD pin on the converter to GPIO1 on the board with jumper cables. Then, from the zeptoforth base directory execute:

    $ e4thcom -t noforth -b B115200 -d <device, typically one of ttyACM0 or ttyUSB0>

Once e4thcom comes up, execute (including the leading '#'), for the STM32L476 DISCOVERY:

    #include src/stm32l476/forth/setup_<type>.fs

or, for the STM32F407 DISCOVERY:

    #include src/stm32f407/forth/setup_<type>.fs

or, for the STM32F746 DISCOVERY:

    #include src/stm32f746/forth/setup_<type>.fs

or, for the Raspberry Pi Pico:

    #include src/rp2040/forth/setup_<type>.fs

where \<type> is one of the types given above, with the meanings given above.

This will load the auxiliary Forth routines that would be useful to have onto the MCU. This code is that is included in the `zeptoforth_<type>-<version>.bin` images along with the kernel itself. The last thing that is included for full builds is a "cornerstone" named `restore-state` which, when executed, as follows:

    restore-state

erases everything compiled to Flash afterwards and then does a restart.

To do a restart by itself (which now does a full reset of the hardware), execute the following:

    reboot

Note that e4thcom is Linux-specific. Another terminal emulator to use with zeptoforth is GNU Screen. One must configure it to use 115200 baud, 8 data bits, 1 stop bit, and currently there is no support for flow control with GNU Screen. Note that zeptoforth uses ACK and NAK for flow control, with ACK indicating readiness to accept a new line of input, and NAK indicating an error; these are not supported by GNU Screen. As a result, one would  have to use `slowpaste 5` with screen to set a proper paste speed. (This is far slower than the ACK/NAK method used with e4thcom.) Additionally, as screen does not honor directives to load files automatically, one will need to use `readbuf <path>` and `paste <path>` to paste files into the terminal manually.

A better approach than using `slowpaste`, `readbuf`, and `paste` with screen is to use `codeload3.py`, which is in the `utils` directory and which honors the e4thcom directives, so it can be used with the included `setup.fs` files without modification. It is invoked as follows:

    $ ./utils/codeload3.py [-p <device>] -B 115200 serial <Forth source file>

It has significantly better performance and functionality than screen with `slowpaste` and is the recommended method of code uploading if e4thcom is not available. Note that it requires Python 3 and pySerial, and it must be given executable permissions before it may be executed.

Another terminal emulator one may use is picocom, which has many of the same considerations here as GNU Screen. For this reason it is not recommended for mass code uploads, for which `codeload3.py` is a better choice, and rather is limited in practice to interactive usage.

If one is using swdcom (assuming one has already built it and installed `swd2` in some suitable location such as `/usr/local/bin` and that one has already written the `zeptoforth_swdcom-<version>.bin` binary to the board), simply execute `swd2`. This will provide a terminal session with zeptoforth. To upload Forth code to execute to the board, execute in the directory from which `swd2` was executed:

    cat <path> > upload.fs && pkill -QUIT swd2

This will simply upload the given file to the board as-is without any support for `#include` or `#require`, unlike e4thcom.

Note that screen and e4thcom are not suitable for using the block editor on the STM32F746 DISCOVERY board or the Raspberry Pi Pico???attempting to use the block editor on them will lock up zeptoforth because it will wait forever for a response when querying the terminal for a cursor position???whereas picocom and swdcom enable it to be used. Also, to use the block editor one must have the backspace key set to $7F (DEL), as it is by default; remapping backspace to backspace will break deleting characters in the block editor

