#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../cpu/cpu.h"
#include "../bus.h"


#ifndef TESTS_CPU_H
#define TESTS_CPU_H
void test_lda_ops();
void test_ldx_ops();
void test_ldy_ops();
void test_sta_ops();
void test_stx_ops();
void test_sty_ops();
void test_flags_ops();
void test_jmp_indirect_wraparound();
#endif // TESTS_CPU_H
