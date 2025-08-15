# /!\ Disclaimer

This code is *NOT* good, it is *NOT* meant to replace any other existing emulator! This was written by a solo inexperienced guy with the goal of learning C, assembler, and the inner workings of a CPU. Make of that what you will, that being said, please feel welcome to open pull requests or issues, I will be happy to discuss them with you!


# Yet another 6502 emulator

Yes, I know there are hundreds of 6502 emulators floating around, but this one is mine! To be honest I am not interested by retro gaming or console emulation,
i started this project after watching [Ben Eater's](https://www.youtube.com/@BenEater) series on his 6502 based breadboard computer and being completely fascinated by it.
What you see so far has been written in about 5 days, I have yet to finish implement the opcodes or even a correct mainloop but I feel like the base architecture is there!
(I am writing all of this only for the git history, I doubt anyone would actually read this now...)


# How it works

It doesn't, that's the beauty of it!
No, seriously, you can at the moment provide a bin file as an argument with `org. 0x0800`, it will be loaded in memeory and executed from the firt byte.


# Resources and Acknowledgements

Without those resources, this project would not have been possible. They are listed here for reference, to help anyone who would like to learn more about the 6502, and as an acknowledgement to the work of their authors.

- [opcodes reference (6502.org)](http://6502.org/users/obelisk/6502/reference.html)
- [automatic makefile dependencies generation for all files of the project](https://stackoverflow.com/a/52036564)
- [overflow flag maths explaination](https://www.righto.com/2012/12/the-6502-overflow-flag-explained.html)
- [assembly introduction and compiler setup](https://www.youtube.com/watch?v=oO8_2JJV0B4&t=172s)
- [vasm (Dr. Volker Barthelmann's Compiler)](http://www.compilers.de/vasm.html)

- [wozmon.s](https://gist.github.com/beneater/8136c8b7f2fd95ccdd4562a498758217)
