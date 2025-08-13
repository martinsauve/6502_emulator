      .org $0600
      LDX #0
   LOOP:
      LDA MESSAGE,X
      BEQ DONE
      STA $F001 ;should be captured by the emulator!
      INX
      JMP LOOP
   DONE:
      NOP
      BRK
   MESSAGE:
      .text "Hello, World!\n"
