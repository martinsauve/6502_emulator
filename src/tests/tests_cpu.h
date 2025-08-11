#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../cpu.h"
#include "../memory.h"


#ifndef TESTS_CPU_H
#define TESTS_CPU_H
void print_lda_result(const char* label, Cpu *cpu, uint8_t expectedA);
void test_lda_ops();
#endif // TESTS_CPU_H
