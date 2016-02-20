#ifndef __CONFIG_H__
#define __CONFIG_H__

//***********************************************************
// data configuration
//*********************************************************
#define N_DOFS 7													// number of d.o.f.s. for one particle

#define N_PARTICLES 2048             							// total number of particles
																	// TODO: does this have to be a power of two ?
// 8192
// 4194304

#define MAX_N 4
#define MAX_L 3

// launch config for X-table kernel
#define GRID_DIM_X 8    									        // number of blocks per grid (x)
                                                                // TODO: choose a smart value for that!
#define BLOCK_DIM_X 32											// Number of threads per block (x)
                                                                // Rem: THIS HAS TO BE A POWER OF TWO
                                                                // TO WORK PROPERLY
                                                                // AND N_PARTICLES/BLOCK_DIM_X MUST BE A POWER OF TWO
                                                                // AS WELL(otherwise, the full reduction kernel
                                                                // will produce crap)
                                                                // Rem: this value has to be determined by the
                                                                // shared memory available and MAX_N, MAX_L
                                                                // (i.e.: is part of the launch configuration)
#define BLOCK_DIM_Y (MAX_N+1)									// number of threads per block (y)

#define NUM_GRIDS (N_PARTICLES/(GRID_DIM_X*BLOCK_DIM_X))			// total number of grids/chunks
                                                                // rem: does not handle odd divisions yet
                                                                // (should perform ceiling)
//#define N_PARTICLES_GRID (GRID_DIM_X*BLOCK_DIM_X)					// number of particles computed within one
                                                                // grid

// launch config for accel kernel
#define ACCEL_GRID_DIM_X 8
#define ACCEL_BLOCK_DIM_X 512                                    // Rem: THIS HAS TO BE A POWER OF TWO
                                                                // TO WORK PROPERLY
#define ACCEL_NUM_GRIDS (N_PARTICLES/GRID_DIM_X)

// more defines..
#define N_N (MAX_N+1)											// number of n indices
#define N_L (MAX_L+1)											// number of l indices
#define PARTICLES_ENTRIES (N_DOFS*N_PARTICLES)
#define TRIANGLE_ENTRIES (N_L*(N_L+1))							// for both sin and cos (i.e. a float
                                                                // triangle)
#define COEFF_TRIANGLE_ENTRIES N_N*N_L                           // the coefficients C_lm, D_lm, E_lm, F_lm
#define ACCEL_CACHE_ENTRIES (4*COEFF_TRIANGLE_ENTRIES)           // cache for the _accel kernel
#define CACHE_ENTRIES (BLOCK_DIM_X*BLOCK_DIM_Y*TRIANGLE_ENTRIES)	// number of entries for one cache
#define X_TABLE_ENTRIES N_N*N_L*(N_L + 1)					    // for both sin and cos tables
#define LIST_GRID_ENTRIES (X_TABLE_ENTRIES*GRID_DIM_X) 			// the results written by one grid
#define LIST_ALL_ENTRIES (LIST_GRID_ENTRIES*NUM_GRIDS)			// the results written by all blocks of all grids
#define GAMMA1_TABLE_ENTRIES (N_N*N_L)
#define GAMMA2_TABLE_ENTRIES N_L
#define ACCELS_ENTRIES (3*N_PARTICLES)

#define SZ_FLOAT sizeof(float)
#define PARTICLES_SZ (PARTICLES_ENTRIES*SZ_FLOAT)
#define RANDOMVALS_SZ (N_PARTICLES*SZ_FLOAT)
#define CACHE_SIZE (CACHE_ENTRIES*SZ_FLOAT)
#define X_TABLE_SZ (X_TABLE_ENTRIES*SZ_FLOAT)
#define LIST_GRID_SZ (LIST_GRID_ENTRIES*SZ_FLOAT)
#define LIST_ALL_SZ (LIST_ALL_ENTRIES*SZ_FLOAT)
#define LIST_ALL_SZ_X
#define LIST_ALL_SZ_Y   N_lm
#define GAMMA1_TABLE_SZ (GAMMA1_TABLE_ENTRIES*SZ_FLOAT)
#define GAMMA2_TABLE_SZ (GAMMA2_TABLE_ENTRIES*SZ_FLOAT)
#define ACCELS_SZ (ACCELS_ENTRIES*SZ_FLOAT)

#define R_MIN 0.0					                        	// sampling range for particle distribution
#define R_MAX 2.0

// timing defines
#define GPU_TIMING
#define CPU_TIMING

// debug defines
//#define GPU_PRINT
#define HERNSPHERE_PRINT

#endif

