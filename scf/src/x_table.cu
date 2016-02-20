#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <cutil_inline.h>
#include <iostream>

#include "config.h"
//#include "cuPrintf.cuh"
#include "reduction.h"

#ifndef MIN
#define MIN(x,y) ((x < y) ? x : y)
#endif

using namespace std;

typedef float Doub;
typedef float Float;
typedef int Int;

__device__ float PI=3.141592653589793f;

__device__ bool _debug0 = false;
bool debug0_dev = false;

// global device variables for x_table kernel
bool gmem_x_table_is_initialised = false;                   // has global dev memory already been allocated
float *d_list_all;
float *d_particles;

// global device variables for accel kernel
bool gmem_accel_is_initialised = false;                     // has global dev memory already been allocated
float *d_accels;

// tables with constant values for accel kernel
__constant__ float x_constants[X_TABLE_ENTRIES];
__constant__ float gamma1_constants[GAMMA1_TABLE_ENTRIES];
__constant__ float gamma2_constants[GAMMA2_TABLE_ENTRIES];
//__device__ float buggyval[50];

// Function declarations
unsigned int nextPow2( unsigned int x );
void getNumBlocksAndThreads(int whichKernel, int n, int maxBlocks, int maxThreads, int &blocks, int &threads);
__device__ float _ultraspher(float xi, int n, float alpha);
__device__ void _print_cache(float *cache);
__device__ void _print_list_all(float *list_all);
__global__ void print_cuda_array(float *arr, int len, int whichrow);
void x_table_cleanup();
void accel_cleanup();

//***********************************************************
// index mapper functions
//***********************************************************
// this map groups the entries with the same (n,x,l,m) indices
// but different particle (threadidx) indices
__device__ inline int _map_cache(int n, int threadidx, int x, int l, int m)
{
    const int N_lm = TRIANGLE_ENTRIES;
    int idx = n*N_lm + x*(N_lm/2) + (l*(l+1))/2 + m;
    idx *= BLOCK_DIM_X;
    idx += threadidx;
    return idx;
}
// some other possible layout for the cache
__device__ inline int _map_cache2(int n, int threadidxx, int x, int l, int m)
{
    const int N_lm = TRIANGLE_ENTRIES;
    int idx = N_lm*(threadidxx + n*BLOCK_DIM_X);
    idx += x*(N_lm/2) + l*(l+1)/2 + m;
    return idx;
}
// map to list-all
// optimized for coalesced access: entries with the same indices
// (n,x,l,m) come in a row (i.e. one after each other)
__device__ inline int _map_list_all(int grid, int block, int n, int x, int l, int m)
{
    const int N_lm = TRIANGLE_ENTRIES;
    int idx = n*N_lm + x*(N_lm/2) + (l*(l+1))/2 + m;
    idx *= GRID_DIM_X*NUM_GRIDS;
    idx += GRID_DIM_X*grid;
    idx += block;
    return idx;
}
// map to X-table
__device__ int _map_x_table(int n, int x, int l, int m)
{
    const int N_lm = TRIANGLE_ENTRIES;
    int idx = n*N_lm + x*(N_lm/2) + l*(l+1)/2 + m;
    return idx;
}
int map_x_table(int n, int x, int l, int m)
{
    const int N_lm = TRIANGLE_ENTRIES;
    int idx = n*N_lm + x*(N_lm/2) + l*(l+1)/2 + m;
    return idx;
}
// map to gamma1-table
__device__ int _map_gamma1_table(int n, int l)
{
    int idx = n*N_L + l;
    return idx;
}
int map_gamma1_table(int n, int l)
{
    int idx = n*N_L + l;
    return idx;
}

//***********************************************************
// This kernel evaluates the values of the individual terms
// and stores the resulting (reduced) table of each block
// to d_list_all
// Args:
// d_list_all: a 2D-array to store the list of tables
// Rem: do not use for timing
//***********************************************************
__global__ void _compute_all_list(float *particles, int nparticles, float *d_list_all)
{
    const int n = threadIdx.y;								// this threads n-index
                                                            // within the block
    const int block = blockIdx.x;							// this threads block-index
    const int tid = threadIdx.x;
    __shared__ float cache[CACHE_ENTRIES];                  // size = number of threads * triangle size

    int p = block*BLOCK_DIM_X + threadIdx.x;                // particle which is processed by this thread
    int grid = 0;

    // forward declarations
    float fact, fact2, oldfact,pll,pmm, _pmm, pmmp1,omx2;
    float sign;
    float P_l_m;
    float inv_renorm;                                       // inverse renormalization factor in (6.7.8)
    // internal tables
    float cmphi[MAX_L+1];                                   // cos(m*phi_k)
    float smphi[MAX_L+1];                                   // sin(m*phi_k)
    float chi[MAX_L+1];                                     // prefactor of (Hernquist 2.25)
    float pi_4 = 4.0f*PI;
    // registers
    float rk, thk, phik, mk, xi_k, x;
    float C_n_k, phi_nl_t_k, term_0, term_1, new_m;
    float ny_0_0, ny_m_m, ny_l_m;
    float chi_0, chi_l, fac_chi;
    int ll;

    while(p < nparticles)
    {
        // step 1: load the data from global memory,
        // compute from this data the terms to sum up
        // and store the values in the cache

        // load the data from global memory
        rk = particles[N_DOFS*p];
        thk = particles[N_DOFS*p + 1];	// theta_k
        phik = particles[N_DOFS*p + 2];	// phi_k
        mk = particles[N_DOFS*p + 6];		// mass of particle k
        //mk = 1.0;
        xi_k = (rk - 1.0f) / (rk + 1.0f);
        x = cos(thk);
        omx2=(1.0f-x)*(1.0f+x);
        // build the sincos table
        for(int m = 0; m <= MAX_L; m++)
        {
            __sincosf(m*phik, &smphi[m], &cmphi[m]);           // accurate?
            //cmphi[m] = cos(m*phik);
            //smphi[m] = sin(m*phik);
        }
        // build the chi table
        chi_0 = 1.0f/(rk + 1.0f);
        chi_l = chi_0;
        fac_chi = rk*chi_0*chi_0;
        for(int l = 0; l <= MAX_L; l++)
        {
            chi[l] = chi_l;
            chi_l *= fac_chi;
        }

        // init recursions
        sign = 1.0;             // sign in (NR3 6.7.10)
        ny_0_0 = 1.0f;           // ny init (inverse renormalization for
        ny_m_m = ny_0_0;        // legendre polynomials, see NR3)
        _pmm=1.0f;               // legendre init (underscore means no prefactor)
        fact2=1.0f;
        for(int m = 0; m <= MAX_L; m++)
        {
            // compute pmm (with prefactor, see (6.7.10) )
            pmm=sqrt((2.0f*m+1.0f)*_pmm/(pi_4));
            pmm *= sign;
            sign *= -1.0f;
            // compute P_m+1_m
            pmmp1=x*sqrt(2.0f*m+3.0f)*pmm;
            oldfact = sqrt(2.0f*m+3.0f);
            // compute inverse renormalization
            // factor (diagonal entry of lm-triangle)
            ny_l_m = ny_m_m;

            for(int l = m; l <= MAX_L; l++)
            {
                // compute P_l_m
                inv_renorm = (pi_4)/(2.0f*l+1.0f);                          // accurate ?
                inv_renorm *= ny_l_m;
                inv_renorm = sqrt(inv_renorm);
                P_l_m = pmm * inv_renorm;

                // compute other quantities
                C_n_k = _ultraspher(xi_k, n, 2.0f*l + 1.5f);                // accurate ?
                phi_nl_t_k = - chi[l] * C_n_k * sqrt(pi_4);

                // now, compute the final terms and
                // store them in cache
                term_0 = mk * phi_nl_t_k * P_l_m * cmphi[m];
                term_1 = mk * phi_nl_t_k * P_l_m * smphi[m];
                int idx_0 = _map_cache(n, tid, 0, l, m);
                int idx_1 = _map_cache(n, tid, 1, l, m);
                cache[idx_0] = term_0;
                cache[idx_1] = term_1;
                //if(n==1 && l == 0 && m == 0)
                //{
                 //   cuPrintf("GPU (k=%d n=%d x=%d l=%d m=%d): %f\n", p, n, 0, l, m, term_0);
                //}

                // update inverse renormalization factor
                ny_l_m *= ((l+m+1.0f)/(l-m+1.0f) );

                // update legendre polynomial
                ll = l + 2;
                fact=sqrt((4.0f*ll*ll-1.0f)/(ll*ll-m*m));                // accurate?
                pll=(x*pmmp1-pmm/oldfact)*fact;
                oldfact=fact;
                pmm=pmmp1;
                pmmp1=pll;
            }

            // update inverse renormalization factor
            new_m = m+1;
            ny_m_m *= (2*new_m-1)*(2*new_m);
            // update legendre polynomial
            _pmm *= omx2*fact2/(fact2+1.0f);
            fact2 += 2.0f;
        }

        __syncthreads();

        // print the cache
        #ifdef GPU_PRINT
        /*cuPrintfRestrict(0,CUPRINTF_UNRESTRICTED);
        cuPrintf("original cache, block %d: \n", block);
        _print_cache(cache);
        cuPrintfRestrict(CUPRINTF_UNRESTRICTED, CUPRINTF_UNRESTRICTED);*/
        #endif

        // step 2: reduction of current block with non-divergent warps
        // (sequential adressing)
        // and swapped loop order
        for(int x = 0; x < 2; x++)
        {
            for(int l = 0; l <= MAX_L; l++)
            {
                for (int m = 0; m <= MAX_L; m++)
                {
                    //cuPrintf("reducing at block %d: (%d %d %d)\n", blockIdx.x, x, l, m);
                    if(m <= l)
                    {
                        // reduce all the tid's of the current
                        // index n-tuple (n,x,l,m)
                        for(unsigned int s=blockDim.x/2; s > 0; s>>=1)
                        {
                            if(tid < s)
                            {
                                int idx1 = _map_cache(n,tid,x,l,m);
                                int idx2 = _map_cache(n, tid+s, x, l, m);
                                cache[idx1] += cache[idx2];
                            }
                            // __syncthreads();                      // TODO: can probably be omitted, since each
                                                                    // row consists in exactly one warp
                        }
                    }
                }
            }
        }

        __syncthreads();

        // print the cache
        #ifdef GPU_PRINT
        /*cuPrintfRestrict(0,CUPRINTF_UNRESTRICTED);
        cuPrintf("reduced cache, block %d: \n", block);
        _print_cache(cache);
        cuPrintfRestrict(CUPRINTF_UNRESTRICTED, CUPRINTF_UNRESTRICTED);*/
        #endif

        // step 3: write the reduction of the current block to the
        // all list, which resides in global memory
        if(tid == 0)                            // reduced table is in first thread of each block
        {
            for(int x = 0; x < 2; x++)
            {
                for(int l = 0; l <= MAX_L; l++)
                {
                    for (int m = 0; m <= MAX_L; m++)
                    {
                        int list_all_idx = _map_list_all(grid, block, n, x, l, m);
						int cache_idx = _map_cache(n, tid, x, l, m);
                        if(m <= l)
                        {
                            d_list_all[list_all_idx] = cache[cache_idx];
                        }
                    }
                }
            }
        }

        // step 4: move to the next grid
        grid = grid + 1;
        p += GRID_DIM_X*BLOCK_DIM_X;

        __syncthreads();
    } // while (p)
}

//***********************************************************
// This kernel computes a list of the accelerations acting
// on each particles
// to d_list_all
// Args:
// particles, nparticles: the particle data
// x_table: table containing the x1- and x2-table from
//          the host code
// gamma_table: the gamma table
// Result:
// compute the accelerations and store them in accels
//***********************************************************
__global__ void _accel(float *particles, int nparticles, float *accels)
{
    const int block = blockIdx.x;							// this threads block-index
    const int tid = threadIdx.x;
    int p = block*ACCEL_BLOCK_DIM_X + threadIdx.x;                // particle which is processed by this thread
    int grid = 0;

    while(p < nparticles)
    {
        // step 1: load the data from global memory,
        // and then compute and sum up the terms

        // load the data from global memory
        float r = particles[p*N_DOFS];
        float th = particles[p*N_DOFS + 1];
        float phi = particles[p*N_DOFS + 2];
        float x = cos(th);
        float a_x, a_y, a_z;
        int maxl = MAX_L;
        int maxn = MAX_N;
        float a_r = 0, a_th = 0, a_phi = 0;

        // forward declarations for recurrence relation of legendre polynomials
        float fact, fact2, oldfact,pll,pmm, _pmm, pmmp1,omx2;
        float P_l_m, P_lm1_m, P_m_m;
        float DP_m_m, DP_l_m;
        float inv_renorm;          // inverse renormalization factor in (6.7.8)
        float sign;
        float new_m;
        float ny_0_0, ny_m_m, ny_l_m;
        float theta_m_m, theta_l_m; // recursion for (l-m)!/(l+m)!
        theta_m_m = 1.0f;
        int ll;
        omx2=(1.0f-x)*(1.0f+x);
        float pi_4 = 4.0f*PI;
        // inverse renormalization for legendre
        sign = -1.0f;                 // sign from (NR3 6.7.10)
        ny_0_0 = 1.0f;
        ny_m_m = ny_0_0;
        fact2=1.0f;
        // init _pmm, pmm and pmmp1
        _pmm=1.0f;
        pmm = sqrt(1.0f/(4.0f*PI));
        pmmp1 = x*sqrt(3.0f)*pmm;    // l=l+1 (strange notation in NR3..)
        // various forward declarations
        float fac0,fac1,fac2,fac3,fac5,fac6,fac7,fac8, xi, accum_fac;
        float N_lm,K_nl,A_nl_t,C_n,C_n_prime,phi_nl_t,d_phi_nl_t_dr;
        float term1,term2,fac9,term;
        float C_lm,D_lm,E_lm,F_lm;

        int counter = 0;
        // sum up the coefficients
        for(int m = 0; m <= maxl; m++)
        {
            // init inverse renormalization (l)
            ny_l_m = ny_m_m;
            // init full legendre (NR3 6.7.8, untilded) (l)
            inv_renorm = (pi_4)/(2*m+1.0f);
            inv_renorm *= ny_l_m;
            inv_renorm = sqrt(inv_renorm);
            P_m_m = pmm * inv_renorm;
            P_l_m = P_m_m;
            // init deriv of legendre (l)
            DP_m_m = (x*m*P_m_m)/(x*x-1.0f);
            DP_l_m = DP_m_m;

            /*if(p == 323451 && counter == 0)
            {
                buggyval[0] = (x*x-1.0f);
                buggyval[1] = cos(th);
                buggyval[2] = th;
                counter++;
            }*/

            // init theta (l)
            theta_l_m = theta_m_m;
            // other stuff for the inner loop (l-loop)
            oldfact = sqrt(2.0f*m+3.0f);

            for (int l = m; l <= maxl; l++)
            {
                // compute the coefficients C_lm, D_lm, E_lm, F_lm
                C_lm=0.0f, D_lm=0.0f, E_lm=0.0f, F_lm=0.0f;

                // constant before float-sum
                N_lm = ( (2.0f*l + 1.0f)/(4.0f*PI) ) * theta_l_m;
                if(m != 0)				// TODO: faster check?
                {                       // TODO: omit this on the GPU
                    N_lm *= 2.0f;
                }
                // first part of float-sum (3.17) (sum over n)
                // precomputed constants for n-loop
                fac0 = (l + 1.0f)*(2.0f*l + 1.0f);
                fac1 = (4.0f*PI)/powf(2.0f,8.0f*l + 6.0f);
                //float fac2 = gamma2_cache[l]*gamma2_cache[l];
                fac2 = gamma2_constants[l]*gamma2_constants[l];
                fac3 = sqrt(4.0f*PI);
                fac5 = - ( powf(r, l)/powf(1 + r,2.0f*l+1.0f) ) * fac3;
                fac6 = (l/r) - ( (2.0f*l+1.0f)/(1.0f+r) );
                fac7 = ( (4.0f*(2.0f*l + 1.5f))/((1.0f+r)*(1.0f+r)));
                fac8 = fac3 * (-1.0f) * powf(r,l)/(powf(1+r,2.0f*l+1.0f));
                xi = (r - 1.0f) / (r + 1.0f);
                accum_fac = 1.0f;                             // accumulator for factorial

                for(int n = 0; n <= maxn; n++)
                {
                    K_nl = 0.5f*n*(n + 4.0f*l + 3.0f) + fac0;    // (2.23)
                    A_nl_t = accum_fac*(n + 2.0f*l + 1.5f ) * fac2;    // (2.31)
                    //A_nl_t /= -K_nl * fac1 * gamma1_cache[_map_gamma1_table(n,l)];
                    A_nl_t /= -K_nl * fac1 * gamma1_constants[_map_gamma1_table(n,l)];
                    C_n = _ultraspher(xi, n, 2.0f*l + 1.5f);
                    C_n_prime = _ultraspher(xi, n-1, 2.0f*l + 2.5f); // derivative of Gegenbauer polynomial
                    if(n == 0)
                    {
                        C_n_prime = 0;
                    }
                    phi_nl_t = fac5 * C_n;
                    d_phi_nl_t_dr = 0.0f;
                    term1 = fac6;                                     // (3.26)
                    term1 *= phi_nl_t;
                    term2 = fac7 * C_n_prime * fac8;                  // (3.26)
                    d_phi_nl_t_dr = term1 + term2;

                    fac9 = N_lm*A_nl_t;
                    C_lm += fac9 * phi_nl_t * x_constants[_map_x_table(n,0,l,m)];
                    D_lm += fac9 * phi_nl_t * x_constants[_map_x_table(n,1,l,m)];
                    E_lm += fac9 * d_phi_nl_t_dr * x_constants[_map_x_table(n,0,l,m)];
                    F_lm += fac9 * d_phi_nl_t_dr * x_constants[_map_x_table(n,1,l,m)];

                    // update the recurrence relations
                    accum_fac *= (n+1);                                     // update factorial accumulator
                }

                // now that we have the coefficients, use them to compute
                // another summand of the accelerations and sum up
                term = P_l_m * ( E_lm*cos(m*phi) + F_lm*sin(m*phi) );
                a_r += term;
                a_th += DP_l_m * (-sin(th)) * ( C_lm*cos(m*phi) + D_lm*sin(m*phi) );
                                                                            // the second factor here is the inner
                                                                            // derivative of the legendre poly
                a_phi += m*P_l_m * (D_lm*cos(m*phi) - C_lm*sin(m*phi));

                // update inverse renormalization factor
                ny_l_m *= ((l+m+1.0f)/(l-m+1.0f) );
                // update legendre
                ll = l + 2;
                fact=sqrt((4.0f*ll*ll-1.0f)/(ll*ll-m*m));
                pll=(x*pmmp1-pmm/oldfact)*fact;
                oldfact=fact;
                pmm=pmmp1;
                pmmp1=pll;
                // update P_l_m
                inv_renorm = (pi_4)/(2*(l+1)+1.0);
                inv_renorm *= ny_l_m;
                inv_renorm = sqrt(inv_renorm);
                P_lm1_m = P_l_m;                    // previous value needed for
                                                    // recursion of the derivative
                P_l_m = pmm * inv_renorm;
                // update derivative of legendre
                DP_l_m = 1.0/(x*x-1.0) * ( (l+1)*x*P_l_m - (l+1+m)*P_lm1_m );
                // update theta
                theta_l_m *= (l-m+1.0f)/(l+m+1.0f);
            }

            // update _pmm (without prefactor, see (6.7.10))
            _pmm *= omx2*fact2/(fact2+1.0f);
            fact2 += 2.0f;
            // update pmm (with prefactor, see (6.7.10))
            new_m = m+1;
            pmm=sqrt((2.0f*new_m+1.0f)*_pmm/(pi_4));
            pmm *= sign;
            sign *= -1.0f;
            // update pmmp1
            pmmp1=x*sqrt(2.0f*new_m+3.0f)*pmm;
            oldfact = sqrt(2.0f*new_m+3.0f);
            // update inverse renormalization factor
            ny_m_m *= (2*new_m-1)*(2*new_m);
            // update theta
            theta_m_m *= 1.0f/((2.0f*m+1.0f)*(2.0f*m+2.0f));
        }

        // multiply acceleration components with spherical gradient perfactors and negative sign
        // (see Hernquist 3.21 - 3.23)
        a_r *= -(1.0f);
        a_th *= -1.0f/r;
        a_phi *= -1.0f/(r*(sin(th)));

        // transform acceleration components to cartesian coords
        a_x = sin(th)*cos(phi)*a_r + cos(th)*cos(phi)*a_th - sin(phi)*a_phi;
        a_y = sin(th)*sin(phi)*a_r + cos(th)*sin(phi)*a_th + cos(phi)*a_phi;
        a_z = cos(th)*a_r - sin(th)*a_th;

        /*if(p == 323451)
        {
            buggyval[0] = a_r;
            buggyval[1] = a_th;
            buggyval[2] = a_phi;
            buggyval[3] = -1.0f/r;
            //buggyval[4] = a_r;
        }*/

        // store the accelerations
        accels[p*3] = a_x;
        accels[p*3+1] = a_y;
        accels[p*3+2] = a_z;

        // step 2: move to the next grid
        grid = grid + 1;
        p += ACCEL_GRID_DIM_X*ACCEL_BLOCK_DIM_X;

        __syncthreads();
    }


  /*  __syncthreads();
    if(tid == 0 && block == 0)
    {
        cuPrintf("buggy: %f\n", buggyval);
    }*/

}

__global__ void debug_kernel()
{
    for(int i = 0 ; i < 10; i++)
    {
//        cuPrintf("debug %d: %f\n", i, buggyval[i]);
    }
}

__device__ float _ultraspher(float xi, int n, float alpha)
{
    float C_nm1 = 1.0;
    if(n == 0)
        return C_nm1;
    float C_n = 2.0*alpha*xi;
    if(n == 1)
        return C_n;

    float C_m;
    for(int m = 2; m <= n; m++)
    {
        C_m = (1.0/m) * (2.0*((m-1.0)+alpha)*xi*C_n - ((m-1.0)+2.0*alpha-1.0)*C_nm1 );
        C_nm1 = C_n;
        C_n = C_m;
    }

    return C_m;
}

void reduce_list(float *data, int nrows, int size, float* gpu_result)
{
    // configure the kernel launch
    int maxThreads = 256;  // number of threads per block
    int whichKernel = 6;
    int maxBlocks = 64;
    int numBlocks = 0;
    int numThreads = 0;
    getNumBlocksAndThreads(whichKernel, size, maxBlocks, maxThreads, numBlocks, numThreads);

    //printf("num blocks = %d\n", numBlocks);

    // allocate memory for the results
    float* d_odata = NULL;
    float* h_odata = (float*) malloc(numBlocks*sizeof(float));
    cutilSafeCallNoSync( cudaMalloc((void**) &d_odata, numBlocks*sizeof(float)) );

    // reduce each of the rows seperately
    for (int r = 0; r < nrows; ++r)
    {
        int row_sz = sizeof(float)*size;
        float* row = (float*)((char*)data + r * row_sz);
        cudaThreadSynchronize();
        reduce<float>(size, numThreads, numBlocks, whichKernel, row, d_odata);
        cudaThreadSynchronize();
        cutilSafeCallNoSync( cudaMemcpy( h_odata, d_odata, numBlocks*sizeof(float), cudaMemcpyDeviceToHost) );
        cudaThreadSynchronize();
        // reduce residual terms of the individual blocks on the CPU
        gpu_result[r] = 0.0;
        for(int i=0; i<numBlocks; i++)
        {
            //if(_debug0) printf("row %d, term %d: %f\n", r, i, h_odata[i]);
            gpu_result[r] += h_odata[i];
        }
    }

    // cleanup
    cudaFree(d_odata);
}

extern "C"
bool isPow2(unsigned int x)
{
    return ((x&(x-1))==0);
}

unsigned int nextPow2( unsigned int x )
{
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return ++x;
}

void getNumBlocksAndThreads(int whichKernel, int n, int maxBlocks, int maxThreads, int &blocks, int &threads)
{
    if (whichKernel < 3)
    {
        threads = (n < maxThreads) ? nextPow2(n) : maxThreads;
        blocks = (n + threads - 1) / threads;
    }
    else
    {
        threads = (n < maxThreads*2) ? nextPow2((n + 1)/ 2) : maxThreads;
        blocks = (n + (threads * 2 - 1)) / (threads * 2);
    }


    if (whichKernel == 6)
        blocks = MIN(maxBlocks, blocks);
}

__global__ void _print_randomvals(float *rvals, int nparticles)
{
    //cuPrintf("randomvals:\n");

    for(int i = 0; i < nparticles; i++)
    {
        //cuPrintf("%f ", rvals[i]) ;
    }
    //cuPrintf("\n");
}

__global__ void _print_particles(float *particles, int nparticles, int dofs)
{
    //cuPrintf("particles:\n");

    for(int i = 0; i < nparticles; i++)
    {
        //cuPrintf("%d: %f %f %f %f\n", i, particles[i*dofs], particles[i*dofs+1], particles[i*dofs+2], particles[i*dofs+3]) ;
    }
    //cuPrintf("\n");
}

__device__ void _print_cache(float *cache)
{
    for(int n = 0; n < BLOCK_DIM_Y; n++)
    {
        //cuPrintf("n=%d: ", n);
        for(int p = 0; p < BLOCK_DIM_X; p++)
        {
            for(int x = 0; x < 2; x++)
            {
                for(int l = 0; l <= MAX_L; l++)
                {
                    for (int m = 0; m <= l; m++)
                    {
                        int cache_idx = _map_cache(n,p,x,l,m);
                        float val = cache[cache_idx];
                        ////cuPrintf("%.10f ", val);
                        //cuPrintf("%.4f ", val);
                    }
                }
                //cuPrintf(",");
            }
            //cuPrintf("\t;\t");
        }
        //cuPrintf("\n");
    }
}

__global__ void _print_list_all(float *list_all)
{
    //cuPrintf("all-list:\n");
    for(int grid = 0; grid < NUM_GRIDS; grid++)
    {
        //cuPrintf("grid %d:\n", grid);

        for(int n = 0; n < BLOCK_DIM_Y; n++)
        {
            //cuPrintf("n=%d: ", n);
            for(int block = 0; block < GRID_DIM_X; block++)
            {
                for(int x = 0; x < 2; x++)
                {
                    for(int l = 0; l <= MAX_L; l++)
                    {
                        for (int m = 0; m <= l; m++)
                        {
                            int list_all_idx = _map_list_all(grid, block, n, x, l, m);
                            float val = list_all[list_all_idx];
                            //cuPrintf("%.10f ", val);
                        }
                    }
                    ////cuPrintf(",");
                }
                //cuPrintf("\t;\t");
            }
            //cuPrintf("\n");
        }
        //cuPrintf("\n");
    }
}

__global__ void print_cuda_array(float *arr, int len, int whichrow)
{
    //cuPrintf("row %d: ", whichrow);
    for(int i = 0; i < len; i++)
    {
        //cuPrintf("%f ", arr[i]);
    }
    //cuPrintf("\n");
}

void shuffle_tables(float *table, float x1_table[MAX_N+1][MAX_L+1][MAX_L+1], float x2_table[MAX_N+1][MAX_L+1][MAX_L+1])
{
    for(int n = 0; n < BLOCK_DIM_Y; n++)
    {
        for(int l = 0; l <= MAX_L; l++)
        {
            for (int m = 0; m <= l; m++)
            {
                int x1_idx = map_x_table(n, 0, l, m);
                int x2_idx = map_x_table(n, 1, l, m);
                x1_table[n][l][m] = table[x1_idx];
                x2_table[n][l][m] = table[x2_idx];
            }
        }
    }
}

void compute_x_tables_GPU(float *particles, int nparticles,
                          float x1_table[MAX_N+1][MAX_L+1][MAX_L+1], float x2_table[MAX_N+1][MAX_L+1][MAX_L+1])
{
    // launch configuration
    int blocksPerGrid = GRID_DIM_X;						// for maximal performance
    dim3 threadsPerBlock(BLOCK_DIM_X, BLOCK_DIM_Y);		// (particlesPerThread, N_n)

    // allocate memory
    if(gmem_x_table_is_initialised == false)
    {
		cutilSafeCall( cudaMalloc((void **)&d_list_all, LIST_ALL_SZ) );
		cutilSafeCall( cudaMalloc((void **)&d_particles, PARTICLES_SZ) );
        gmem_x_table_is_initialised = true;
    }

    // init the timer
#ifdef GPU_TIMING
    cudaEvent_t start, stop;
    float time_all_list;
    float time_reduce_list;
    float time_memcpy;
    float time_shuffle;
    float time_all;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
#endif

	// init the printer
#ifdef GPU_PRINT
	cudaPrintfInit();
#endif

    // copy data to device memory
#ifdef GPU_TIMING
    cudaEventRecord(start, 0);
#endif
    cutilSafeCall( cudaMemcpy(d_particles, particles, PARTICLES_SZ, cudaMemcpyHostToDevice) );
    cutilSafeCall( cudaThreadSynchronize() );
#ifdef GPU_TIMING
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time_memcpy, start, stop);
#endif

    // perform the all-list kernel
#ifdef GPU_TIMING
    cudaEventRecord(start, 0);
#endif
    _compute_all_list<<< blocksPerGrid, threadsPerBlock>>>(d_particles, nparticles, d_list_all);
    cutilSafeCall( cudaThreadSynchronize() );
#ifdef GPU_TIMING
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time_all_list, start, stop);
#endif

    // perform the reduce_list kernel
#ifdef GPU_TIMING
    cudaEventRecord(start, 0);
#endif
    float table[X_TABLE_ENTRIES];
    reduce_list(d_list_all, X_TABLE_ENTRIES, GRID_DIM_X*NUM_GRIDS, table);
    cutilSafeCall( cudaThreadSynchronize() );
#ifdef GPU_TIMING
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time_reduce_list, start, stop);
#endif

    // shuffle the table array to the x1_table and x2_table parameters
#ifdef GPU_TIMING
    cudaEventRecord(start, 0);
#endif
    shuffle_tables(table, x1_table, x2_table);
#ifdef GPU_TIMING
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time_shuffle, start, stop);
#endif

#ifdef GPU_TIMING
    time_all = time_all_list + time_reduce_list + time_memcpy + time_shuffle;
    printf("X-tables GPU timing results : ");
    printf("all-list: %f ms, reduce: %f ms, mem-copy: %f ms, shuffle: %f ms, all: %f ms\n", time_all_list, time_reduce_list, time_memcpy, time_shuffle, time_all);
#endif

    // cleanup the timer
#ifdef GPU_TIMING
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
#endif

    // cleanup the printer
#ifdef GPU_PRINT
    cudaPrintfDisplay(stdout, true);
    cudaPrintfEnd();
#endif

}

void accel_gravity_scf_gpu(float *particles, int nparticles,
                          float x1_table[MAX_N+1][MAX_L+1][MAX_L+1], float x2_table[MAX_N+1][MAX_L+1][MAX_L+1],
                          float gamma1_table[MAX_N+1][MAX_L+1], float gamma2_table[MAX_L+1],
                          float *accels)
{
    // launch configuration
    int blocksPerGrid = ACCEL_GRID_DIM_X;				// for maximal performance
    int threadsPerBlock = ACCEL_BLOCK_DIM_X;              		// (particlesPerThread, N_n)

    // allocate memory
    if(gmem_accel_is_initialised == false)
    {
        cutilSafeCall( cudaMalloc((void **)&d_particles, PARTICLES_SZ) );
        cutilSafeCall( cudaMalloc((void **)&d_accels, ACCELS_SZ) );
        gmem_accel_is_initialised = true;
    }

    // init the timer
#ifdef GPU_TIMING
    cudaEvent_t start, stop;
    float time_accel;
    float time_memcpy;
    float time_all;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
#endif

	// init the printer
#ifdef GPU_PRINT
	cudaPrintfInit();
#endif

#ifdef GPU_TIMING
    cudaEventRecord(start, 0);
#endif
    // shuffle the non-linear host variables to linear temporary version which
    // can be directly copied with cudaMemcpy
    float x_table_temp[X_TABLE_ENTRIES];
    float gamma1_table_temp[GAMMA1_TABLE_ENTRIES];
    for(int n = 0 ; n <= MAX_N; n++)    //X-tables
    {
        for(int l = 0; l <= MAX_L; l++)
        {
            for(int m=0; m <= l; m++)
            {
                int idx1 = map_x_table(n, 0, l, m);
                int idx2 = map_x_table(n, 1, l, m);
                x_table_temp[idx1] = x1_table[n][l][m];
                x_table_temp[idx2] = x2_table[n][l][m];
            }
        }
    }
    for(int n = 0; n <= MAX_N; n++)
    {
        for(int l = 0 ; l <= MAX_L; l++)
        {
            int idx = map_gamma1_table(n, l);
            gamma1_table_temp[idx] = gamma1_table[n][l];
        }
    }

    // copy data to device memory
    cutilSafeCall( cudaMemcpy(d_particles, particles, PARTICLES_SZ, cudaMemcpyHostToDevice) );
    cutilSafeCall( cudaThreadSynchronize() );

    // copy data to constant memory
    cudaMemcpyToSymbol(x_constants, x_table_temp, X_TABLE_SZ);
    cudaMemcpyToSymbol(gamma1_constants, gamma1_table_temp, GAMMA1_TABLE_SZ);
    cudaMemcpyToSymbol(gamma2_constants, gamma2_table, GAMMA2_TABLE_SZ);
#ifdef GPU_TIMING
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time_memcpy, start, stop);
#endif

#ifdef GPU_TIMING
    cudaEventRecord(start, 0);
#endif
    // perform the accel kernel
    printf("_accel() launch with blocksPerGrid=%d, threadsPerBlock=%d\n", blocksPerGrid,
            threadsPerBlock);
    _accel<<< blocksPerGrid, threadsPerBlock>>>(d_particles, nparticles, d_accels);
    cutilSafeCall( cudaThreadSynchronize() );
#ifdef GPU_TIMING
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time_accel, start, stop);
#endif

    //debug_kernel<<<1,1>>>();

#ifdef GPU_TIMING
    time_all = time_accel + time_memcpy;
    printf("Acceleration GPU timing results : ");
    printf("accel: %f ms, mem-copy: %f ms, all: %f ms\n", time_accel, time_memcpy, time_all);
#endif

    // cleanup the timer
#ifdef GPU_TIMING
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
#endif

    // cleanup the printer
#ifdef GPU_PRINT
    cudaPrintfDisplay(stdout, true);
    cudaPrintfEnd();
#endif

    // copy data to host
    cutilSafeCall( cudaMemcpy(accels, d_accels, ACCELS_SZ, cudaMemcpyDeviceToHost) );

}

void gpumem_cleanup()
{
    if(gmem_accel_is_initialised || gmem_x_table_is_initialised)
    {
        cudaFree(d_particles);
    }
    if(gmem_accel_is_initialised)
    {
        cudaFree(d_accels);
    }
    if(gmem_x_table_is_initialised)
    {
        cudaFree(d_list_all);
    }
}

void gpu_test_kernel(float *particles, int nparticles)
{
    // launch configuration
    int blocksPerGrid = GRID_DIM_X;						// for maximal performance
    dim3 threadsPerBlock(BLOCK_DIM_X, BLOCK_DIM_Y);		// (particlesPerThread, N_n)

    // allocate memory
    if(gmem_x_table_is_initialised == false)
    {
		cutilSafeCall( cudaMalloc((void **)&d_list_all, LIST_ALL_SZ) );
		cutilSafeCall( cudaMalloc((void **)&d_particles, PARTICLES_SZ) );
        gmem_x_table_is_initialised = true;
    }

    // copy data to device memory
    cutilSafeCall( cudaMemcpy(d_particles, particles, PARTICLES_SZ, cudaMemcpyHostToDevice) );
    cutilSafeCall( cudaThreadSynchronize() );

}
