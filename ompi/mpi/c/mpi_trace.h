
#ifndef KUTRACE_H
#define KUTRACE_H

#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define __NR_kutrace_control 1023
#define KUTRACE_CMD_INSERT1 7
#define KUTRACE_MARKA           0x20A
#define KUTRACE_MARKB           0x20B
#define KUTRACE_MARKC           0x20C
#define KUTRACE_MARKD           0x20D
#define CLU(x) x##LLU

// Uppercase are mapped to lowercase
// All unexpected characters are mapped to '.'
//   - = 0x2D . = 0x2E / = 0x2F
// Base40 characters are _abcdefghijklmnopqrstuvwxyz0123456789-./
//                       0         1         2         3
//                       0123456789012345678901234567890123456789
// where the first is NUL.
static const char kToBase40[256] = {
   0,38,38,38, 38,38,38,38, 38,38,38,38, 38,38,38,38, 
  38,38,38,38, 38,38,38,38, 38,38,38,38, 38,38,38,38, 
  38,38,38,38, 38,38,38,38, 38,38,38,38, 38,37,38,39, 
  27,28,29,30, 31,32,33,34, 35,36,38,38, 38,38,38,38, 

  38, 1, 2, 3,  4, 5, 6, 7,  8, 9,10,11, 12,13,14,15,
  16,17,18,19, 20,21,22,23, 24,25,26,38, 38,38,38,38, 
  38, 1, 2, 3,  4, 5, 6, 7,  8, 9,10,11, 12,13,14,15,
  16,17,18,19, 20,21,22,23, 24,25,26,38, 38,38,38,38, 

  38,38,38,38, 38,38,38,38, 38,38,38,38, 38,38,38,38, 
  38,38,38,38, 38,38,38,38, 38,38,38,38, 38,38,38,38, 
  38,38,38,38, 38,38,38,38, 38,38,38,38, 38,38,38,38, 
  38,38,38,38, 38,38,38,38, 38,38,38,38, 38,38,38,38, 

  38,38,38,38, 38,38,38,38, 38,38,38,38, 38,38,38,38, 
  38,38,38,38, 38,38,38,38, 38,38,38,38, 38,38,38,38, 
  38,38,38,38, 38,38,38,38, 38,38,38,38, 38,38,38,38, 
  38,38,38,38, 38,38,38,38, 38,38,38,38, 38,38,38,38, 
};

static inline uint64_t DoControl(uint64_t command, uint64_t arg)
{
  return syscall(__NR_kutrace_control, command, arg);
}

// X86-64 inline version
//    uint64_t retval;
//    asm volatile
//    (
//        "syscall"
//        : "=a" (retval)
//        : "0"(__NR_dclab_control), "D"(command), "S"(arg)
//        : "cc", "rcx", "r11", "memory"
//    );
//    return retval;

static inline void mark(uint64_t n, const char* label) {
	int len = strlen(label);
	// If longer than 6 characters, take only the first 6
	if (len > 6) {len = 6;}
	uint64_t base40 = 0;
	// First character goes in last, comes out first
	for (int i = len - 1; i >= 0; -- i) {
		base40 = (base40 * 40) + kToBase40[label[i]];
	}
	  //         T             N                       ARG
	uint64_t temp = (CLU(0) << 44) | (n << 32) | (base40 &  CLU(0x00000000FFFFFFFF));
	DoControl(KUTRACE_CMD_INSERT1, temp);
}

static inline void mark_a(const char* label) {
	mark(KUTRACE_MARKA, label);
}
static inline void mark_b(const char* label) {
	mark(KUTRACE_MARKB, label);
}
static inline void mark_c(const char* label) {
	mark(KUTRACE_MARKC, label);
}

#endif


#undef LTTNG_UST_TRACEPOINT_PROVIDER
#define LTTNG_UST_TRACEPOINT_PROVIDER open_mpi

#undef LTTNG_UST_TRACEPOINT_INCLUDE
#define LTTNG_UST_TRACEPOINT_INCLUDE "mpi_trace.h"

#if !defined(__OPEN_MPI_TRACE_H__) || defined(LTTNG_UST_TRACEPOINT_HEADER_MULTI_READ)
#define __OPEN_MPI_TRACE_H__

#include <lttng/tracepoint.h>

LTTNG_UST_TRACEPOINT_EVENT(
	/* Tracepoint provider name */
	open_mpi,

	/* Tracepoint name */
	common,

	/* Input arguments */
	LTTNG_UST_TP_ARGS(
		char *, op,
		int, count,
		char *, data_type
	),

	/* Output event fields */
	LTTNG_UST_TP_FIELDS(
		lttng_ust_field_string(op, op)
		lttng_ust_field_integer(int, count, count)
		lttng_ust_field_string(data_type, data_type)
	)
)


#define mpi_tracepoint(arg...) lttng_ust_tracepoint(arg)

#endif /* __OPEN_MPI_TRACE_H__*/

#include <lttng/tracepoint-event.h>
