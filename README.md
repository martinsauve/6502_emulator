# :warning: Disclaimer

**DO NOT USE YET, PROJECT NOT FINISHED! ALL OPCODES ARENT EVEN IMPLEMENTED...**

This code is *NOT* good, it is *NOT* meant to replace any other existing emulator! This was written by a solo inexperienced guy with the goal of learning C, assembler, and the inner workings of a CPU. Make of that what you will, that being said, please feel welcome to open pull requests or issues, I will be happy to discuss them with you!


# Yet another 6502 emulator

Yes, I know there are hundreds of 6502 emulators floating around, but this one is mine! To be honest I am not interested by retro gaming or console emulation,
i started this project after watching [Ben Eater's](https://www.youtube.com/@BenEater) series on his 6502 based breadboard computer and being completely fascinated by it.
What you see so far has been written in about 8 days, I have yet to finish implement the opcodes or even a correct mainloop but I feel like the base architecture is there!


# How it works

## Build

Compile the project with the included Makefile. The default target will build the executable, the included ROMS, and drop you straight into Wozmon.

## Usage

This emulator runs by default in a terminal, but can be run in a GUI window with the `--gui` flag. The GUI is very basic and displays a text terminal and the CPU state.

You can load a ROM starting at `0x8000` (32K of ROM), that should set the reset vector `0xFFFC` to the address of the code.
`./6502 --rom <romfile path>`

This emulator supports a very simple form of I/O, mapping terminal stdin/stdout to `0x5000`, with an input ready flag at `0x5001`. This is loosely based on Ben Eater's 6502 breadboard computer design. Writing a byte to `0x5000` will print it to stdout, reading from `0x5000` will read a byte from stdin if available, and reading from `0x5001` will return `0x08` (7th bit set) if input is availabe, `0x00` otherwise. This is my (very crude) implemantation of ACIA.

This emulator also supports snapshots, using a custom binary format loosely based on the RIFF format(without `LIST`). You at the moment can load a snapshot using the `--snap <snapshot file path>` flag on the command line. Taking snapshots is implemented but not yet in the interface. The format shoud be understandable by looking at `src/snapshot.c`.

If no ROM of snapshot is provided, the emulator will attempt to run WozMon if it can find it in `roms/wozmon.bin`.

# Resources and Acknowledgements

Without those resources, this project would not have been possible. They are listed here for reference, to help anyone who would like to learn more about the 6502, and as an acknowledgement to the work of their authors.

- [opcodes reference (6502.org)](http://6502.org/users/obelisk/6502/reference.html)
- [automatic makefile dependencies generation for all files of the project](https://stackoverflow.com/a/52036564)
- [overflow flag maths explaination](https://www.righto.com/2012/12/the-6502-overflow-flag-explained.html)
- [assembly introduction and compiler setup](https://www.youtube.com/watch?v=oO8_2JJV0B4&t=172s)
- [vasm (Dr. Volker Barthelmann's Compiler)](http://www.compilers.de/vasm.html)

- [wozmon.s](https://gist.github.com/beneater/8136c8b7f2fd95ccdd4562a498758217)

- [RIFF binary file format](https://fr.wikipedia.org/wiki/Resource_Interchange_File_Format)

Shoutout to Github Copilot as well, I used it to help me review some of the code and it was surprisingly useful! Rest assured that I did not generate any of the code with it... :)
