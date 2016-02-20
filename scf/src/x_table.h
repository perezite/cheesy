
#ifndef __X_TABLE_H__
#define __X_TABLE_H__

//***********************************************************
// Kernel launcher for _compute_tables() kernel
// Compute the complete reduction of the list
// Input: *particles - the particles on host side
// Output:*table - the fully reduced table
//***********************************************************
void compute_x_tables_GPU(float *particles, int nparticles,
                          float x1_table[MAX_N+1][MAX_L+1][MAX_L+1], float x2_table[MAX_N+1][MAX_L+1][MAX_L+1]);

//***********************************************************
// Kernel launcher for _acceleration() kernel
// Compute a list of accelerations for each particle
// Input:   *particles - the particles on host side
//          *x_table - the (already generated) x-table
//           on host side
//          *gamma_table - the (already generated) gamma
//           on host side
// Output:  *accels - a list of length 3*nparticles
//           containing the cartesian acceleration of
//           each particles acceleration
//***********************************************************
void accel_gravity_scf_gpu(float *particles, int nparticles,
                          float x1_table[MAX_N+1][MAX_L+1][MAX_L+1], float x2_table[MAX_N+1][MAX_L+1][MAX_L+1],
                          float gamma1_table[MAX_N+1][MAX_L+1], float gamma2_table[MAX_L+1],
                          float *accels);

//***********************************************************
// cleanup the device arrays
//***********************************************************
void gpumem_cleanup();

//***********************************************************
// test kernel for bug tracking in device mode
//***********************************************************
void gpu_test_kernel(float *particles, int nparticles); 


#endif
