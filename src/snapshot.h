#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "cpu/cpu.h"
#include "bus.h"


#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#define SNAP_FILE_FORMAT_VERSION       1
#define SNAP_FILE_FORMAT_HEADER  "SNAP" /*must be 4 bytes*/

int loadSnapshot(const char *filename, Cpu *cpu, Bus *bus);
int saveSnapshot(const Cpu *cpu, const Bus *bus, const char *filename);

#endif // SNAPSHOT_H
