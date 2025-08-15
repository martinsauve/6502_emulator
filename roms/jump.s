      .org $0600
      LDX #0
   LOOP:
      LDA MESSAGE,X
      BEQ DONE
      JSR PUTCHAR
      INX
      JMP LOOP
   DONE:
      NOP
      BRK
   PUTCHAR:
      STA $5000 ;should be captured by the emulator!
      RTS

   MESSAGE:
      .text "Hello, World!\n"
