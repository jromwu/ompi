/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2020 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2008 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2010-2012 Oak Ridge National Labs.  All rights reserved.
 * Copyright (c) 2015      Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "ompi/mpi/c/bindings.h"
#include "ompi/runtime/params.h"
#include "ompi/communicator/communicator.h"
#include "ompi/errhandler/errhandler.h"
#include "ompi/memchecker.h"
#include "ompi/runtime/ompi_spc.h"

#include "ompi/mpi/c/mpi_trace.h"

#if OMPI_BUILD_MPI_PROFILING
#if OPAL_HAVE_WEAK_SYMBOLS
#pragma weak MPI_Barrier = PMPI_Barrier
#endif
#define MPI_Barrier PMPI_Barrier
#endif

static const char FUNC_NAME[] = "MPI_Barrier";


int MPI_Barrier(MPI_Comm comm)
{
  mark_c("barr");
  int err = MPI_SUCCESS;

  SPC_RECORD(OMPI_SPC_BARRIER, 1);

  MEMCHECKER(
    memchecker_comm(comm);
  );

  /* Error checking */

  if (MPI_PARAM_CHECK) {
    OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
    if (ompi_comm_invalid(comm)) {
      return OMPI_ERRHANDLER_NOHANDLE_INVOKE(MPI_ERR_COMM, FUNC_NAME);
    }
  }

#if OPAL_ENABLE_FT_MPI
  /*
   * An early check, so as to return early if we are using a broken
   * communicator. This is not absolutely necessary since we will
   * check for this, and other, error conditions during the operation.
   */
  if( OPAL_UNLIKELY(!ompi_comm_iface_coll_check(comm, &err)) ) {
      OMPI_ERRHANDLER_RETURN(err, comm, err, FUNC_NAME);
  }
#endif

  /* Intracommunicators: Only invoke the back-end coll module barrier
     function if there's more than one process in the communicator */

  if (OMPI_COMM_IS_INTRA(comm)) {
    if (ompi_comm_size(comm) > 1) {
      err = comm->c_coll->coll_barrier(comm, comm->c_coll->coll_barrier_module);
    }
  }

  /* Intercommunicators -- always invoke, because, by definition,
     there's always at least 2 processes in an intercommunicator. */

  else {
      err = comm->c_coll->coll_barrier(comm, comm->c_coll->coll_barrier_module);
  }

  /* All done */
  mark_c("/barr");

  OMPI_ERRHANDLER_RETURN(err, comm, err, FUNC_NAME);
}
