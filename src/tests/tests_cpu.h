#include <stdint.h>
#include "../cpu/cpu.h"
#include "../bus.h"


#ifndef TESTS_CPU_H
#define TESTS_CPU_H
void test_lda_ops(void);
void test_ldx_ops(void);
void test_ldy_ops(void);
void test_sta_ops(void);
void test_stx_ops(void);
void test_sty_ops(void);
void test_flags_ops(void);
void test_jmp_indirect_wraparound(void);
#endif // TESTS_CPU_H
