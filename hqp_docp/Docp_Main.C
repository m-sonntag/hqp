/* 
 * Docp_Main.C --
 *
 *    Main function for the Omuses demo collection
 *
 *  rf, 2/10/98
 *
 */

#include <stdio.h>
#include <assert.h>

#include <Hqp.h>
#include "Prg_DID.h"

/**
 * Simple main function.
 * First Hqp is initialized using the Docp interface.
 * Afterwards the solver is executed via the If interface.
 */
int main(int argc, char *argv[])
{
  // create interpreter for interface library
  if (If_CreateInterp(argc, argv) != IF_OK)
    printf("Can't create Tcl interpreter: %s\n", If_ResultString());

  // Assert that Hqp uses same data representation as this application
  // (might be different if different C++ compilers are used)
  assert(If_SizeOfInt() == sizeof(int));
  assert(If_SizeOfReal() == sizeof(Real));

  // Initialize Hqp
  Hqp_Init(If_Interp());

  // Create optimization program
  Prg_DID *prg = new Prg_DID();

  // Configure solver
  printf("Configure solver\n");
  double eps = 1e-5;
  const char *mat_solver = "LQDOCP";

  // write sqp_eps
  if (If_SetReal("sqp_eps", eps) != IF_OK)
    printf("Can't write sqp_eps: %s\n", If_ResultString());

  // select matrix solver
  if (If_SetString("qp_mat_solver", mat_solver) != IF_OK)
    printf("Can't select qp_mat_solver: %s\n", If_ResultString());

  // test reading back modified options
  // (from now on error tests are omitted for If-calls
  //  in order to improve readability)
  eps = 0.0;
  If_GetReal("sqp_eps", eps);
  printf(" set sqp_eps to: %g\n", eps);

  mat_solver = NULL;
  If_GetString("qp_mat_solver", mat_solver);
  printf(" set qp_mat_solver to: %s\n", mat_solver);

  printf("\nSolve problem\n");

  // Setup and initialize problem
  If_Eval("prg_setup");		// setup problem
  If_Eval("prg_simulate");	// perform initial value simulation
  If_Eval("sqp_init");		// initialize SQP solver

  // Solve problem
  if (If_Eval("hqp_solve") != IF_OK) {
    printf("Failed    : %s\n", If_ResultString());
  }
  else {
    printf("Optimal solution found\n");
  }

  // Write solution statistics
  double objective;
  int iters, steps;
  If_GetReal("prg_f", objective);
  If_GetInt("sqp_iter", iters);
  If_GetInt("prg_fbd_evals", steps);
  printf("Objective : %g\n", objective);
  printf("Iters     : %d\n", iters);
  printf("Line steps: %d\n", steps);

  // Free program
  delete prg;

  return 0;
}
