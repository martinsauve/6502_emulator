#include "../bus.h"
#include "cpu.h"

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

Addr readAddr(Cpu *cpu, Bus *bus) {
   return (Addr)bus->memory[cpu->PC + 1] | ((Byte)bus->memory[cpu->PC + 2] << 8);
}

void pushStack(Cpu *cpu, Bus *bus, Byte value) {
   bus->map.STACK[cpu->SP] = value;
   cpu->SP = (cpu->SP - 1) & 0xFF; // wrap around
}

Byte pullStack(Cpu *cpu, Bus *bus) {
   cpu->SP = (cpu->SP + 1) & 0xFF;
   return bus->map.STACK[cpu->SP];
}
