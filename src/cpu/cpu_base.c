#include "../bus.h"
#include "cpu.h"
#include <stdint.h>

// SLEEP
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleep_ms(int milliseconds){ // cross-platform sleep function
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    if (milliseconds >= 1000)
      sleep(milliseconds / 1000);
    usleep((milliseconds % 1000) * 1000); //NOLINT
#endif
}



//-----------------------------
// HELPERS
//-----------------------------
void setZN(Cpu *cpu, Byte val) {
   cpu->Z = (val == 0);
   cpu->N = ( (val & 0x80) != 0 );
}

void setV(Cpu *cpu, Byte a, Byte b, uint16_t result) {
   // Set Overflow flag
   // Overflow occurs if the sign of A and B are the same, but the sign of result differs

   //       NOT XOR      AND   XOR  AND
   //        |   |         |    |    |
   cpu->V = (~(a ^ b) & (a ^ result) & 0x80) != 0;
   // Effectively, this checks if the sign bits of A and B are the same, but differ from the sign bit of the result.
   // example: A = +128, B = +200, result = -56
   // yes, dont ask me why this is so complicated, it just is.
}

Addr readAddr(Cpu *cpu, Bus *bus) {
   return (Addr)busRead(bus, cpu->PC + 1) | ((Byte)busRead(bus, cpu->PC + 2) << 8);
}

void pushStack(Cpu *cpu, Bus *bus, Byte value) {
   busWrite(bus, STACK_START + cpu->SP, value);
   cpu->SP = (cpu->SP - 1) & 0xFF; // wrap around
}

Byte pullStack(Cpu *cpu, Bus *bus) {
   cpu->SP = (cpu->SP + 1) & 0xFF;
   return busRead(bus, STACK_START + cpu->SP);
}
