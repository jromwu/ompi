
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
