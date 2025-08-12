   .org $0600
      LDX #0
   LOOP:
      LDA MESSAGE,X
      BEQ DONE
      STA $F001 ;should be captured by the emulator!
      INX
      JMP LOOP
   DONE:
      BRK ; not implemented, should crash
   MESSAGE:
      .text "Hello, World!"
      .byte 0
