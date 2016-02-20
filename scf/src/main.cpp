#include "config.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include "output.h"
#include "nr3.h"
#include "gamma.h"
#include "plegendre.h"
#include "x_table.h"
using namespace std;

//#define DEBUG_SANDBOX
//#define DEBUG_NUMERICS

// custom floating point overflow checks
// for compiler portability
#define isnan(x) _isnan(x)
#define isinf(x) (!_finite(x))
#define fpu_error(x) (isinf(x) || isnan(x))

string outputParticleFile = "anim.dat";
string outputGifFile = "anim.gif";
bool plotAndShow = true;
bool computeOnGPU = false;

const Real Pi = M_PI;
const int nparticles = N_PARTICLES;
//const int nparticles = 256;
// const int nSteps = 200;
int nSteps = 50;
const Real stepWidth = 0.0001;
//const Real stepWidth = 10.0;
const Real epsilon = 0.0001;
const int dofs = 7;                             // 3 position coordinates (r, theta, phi), 3 velocity coordinates
                                                //(vx, vy, vz), 1 mass
Real particles[dofs*nparticles];		        // spherical coordinates and masses of density distribution
                                                // [r, theta, phi, m]
Real rmin = 0.0;					            // sampling range for generated particles
Real rmax = 2.0;

// look-up tables
Real x1_table[MAX_N+1][MAX_L+1][MAX_L+1];     // tables for the sum over k in (Hernquist 3.17)
Real x2_table[MAX_N+1][MAX_L+1][MAX_L+1];
Real gamma1_table[MAX_N+1][MAX_L+1];
Real gamma2_table[MAX_L+1];

//***********************************************************
// check if x,y,z are all finite
//***********************************************************
bool isfinite3D(Real x, Real y, Real z)
{
    //bool isFinite = isfinite(x) && isfinite(y) && isfinite(z);
	bool isFinite = !fpu_error(x) && !fpu_error(y) && !fpu_error(z);
    return isFinite;
}

//***********************************************************
// give uniform random number in [0,1)
//***********************************************************
Real unirand()
{
    return ( Real(rand())/Real(RAND_MAX + 1.0) );
}

//***********************************************************
// give gamma of a
//***********************************************************
Real gamm(Real a)
{
    return exp(gammln(a));
}

//***********************************************************
// give factorial of a
//***********************************************************
Real fac(int a)
{
    return factrl(a);
}

//***********************************************************
// compute assoc. legendre polynomial
// for indices l,m
//***********************************************************
Real P_lm(Real x, int l, int m)
{
    return plgndr(l, m, x);
}

//***********************************************************
// compute derivative of assoc. legendre polynomial:
// d/dtheta (P_lm(l,m, cos(theta))
// for indices l,m
// remark: beware, don't pass cos(theta) as argument
// TODO: What happens if cos(th) = 1 (see the recurrence
// formula)
//***********************************************************
Real DP_lm(Real th, int l, int m)
{
    // TODO: omit this
    //assert(th != 0);

    Real x = cos(th);
    Real res = 0.0;

    if(l==m)
    {
        res = ( 1.0/(x*x-1.0) ) * l*x*P_lm(x,l,m);
        res *= -sin(th);
    }
    else
    {
        res = ( 1.0/(x*x-1.0) ) * (l*x*P_lm(x,l,m)-(l+m)*P_lm(x,l-1,m));
        res *= -sin(th);
    }

    return res;
}

//***********************************************************
// compute derivative of assoc. legendre polynomial:
// d/dtheta (P_lm(l,m, cos(theta))
// for indices l,m
// remark: beware, don't pass cos(theta) as argument
//***********************************************************
Real DP_lm_slow(Real th, int l, int m)
{
    Real res = 0.0;
    res = (-1.0 - Real(l)) * cos(th) * P_lm(cos(th), l, m);
    res += (1+l-m) * P_lm(cos(th),1+l,m);
    res *= sin(th);
    res /= -1.0 + cos(th)*cos(th);
    res *= -1.0;

    return res;
}

//***********************************************************
// give ultraspherical polynomial of n with exponent alpha
// at normalized coordinate xi
//***********************************************************
Real ultraspher(Real xi, int n, Real alpha)
{
    Real C_nm1 = 1.0;
    if(n == 0)
        return C_nm1;
    Real C_n = 2.0*alpha*xi;
    if(n == 1)
        return C_n;

    Real C_m;
    for(int m = 2; m <= n; m++)
    {
        C_m = (1.0/(Real(m))) * (2.0*(Real(m-1)+alpha)*xi*C_n - (Real(m-1)+2.0*alpha-1.0)*C_nm1 );
        C_nm1 = C_n;
        C_n = C_m;
    }

    return C_m;
}

//***********************************************************
// compute tabulated values for the X expressions
// on the CPU
//***********************************************************
void compute_x_tables_CPU()
{
    // second part of Real sum (3.17) (sum over k)
    for(int n = 0; n <= MAX_N; n++)
    {
        for(int l = 0; l <= MAX_L; l++)
        {
            for(int m = 0; m <= l; m++)
            {
                x1_table[n][l][m] = 0.0;
                x2_table[n][l][m] = 0.0;
                for(int k = 0; k < nparticles; k++)
                {
                    Real rk = particles[dofs*k];
                    Real thk = particles[dofs*k + 1];	// theta_k
                    Real phik = particles[dofs*k + 2];	// phi_k
                    Real mk = particles[dofs*k + 6];		// mass of particle k
                    Real xi_k = (Real(rk) - 1.0) / (Real(rk) + 1.0);
                    Real C_n_k = ultraspher(xi_k, n, 2.0*Real(l) + 1.5);
                    Real phi_nl_t_k = - ( (pow(rk,Real(l))) / (pow(1.0+rk,2.0*Real(l)+1.0)) ) * C_n_k * sqrt(4.0*Pi);
                    x1_table[n][l][m] += mk * phi_nl_t_k * P_lm(cos(thk), l, m) * cos(Real(m)*phik);
                    x2_table[n][l][m] += mk * phi_nl_t_k * P_lm(cos(thk), l, m) * sin(Real(m)*phik);
                }
                //cout << "n = " << n << " l = " << l << " m = " << m << " table values: " << x1_table[n][l][m] << " " << x2_table[n][l][m] << endl;
            }
        }
    }
}

//***********************************************************
// compute tabulated values for the X expressions
//***********************************************************
void compute_x_tables() 
{
	if (computeOnGPU) {
		compute_x_tables_GPU(particles, nparticles, x1_table, x2_table);
	} else {
		compute_x_tables_CPU();
	}
}

//***********************************************************
// compute tabulated values for the gamma values
//***********************************************************
void compute_gamma_tables()
{
    // compute the first gamma table
    for(int n = 0; n <= MAX_N; n++)
    {
        for(int l = 0; l <= MAX_L; l++)
        {
            gamma1_table[n][l] = gamm(Real(n) + 4.0*Real(l) + 3.0);
        }
    }

    // compute the second gamma table
    for(int l = 0; l <= MAX_L; l++)
    {
        gamma2_table[l] = gamm(2.0*Real(l) + 1.5);
    }
}

//***********************************************************
// compute the coefficients A_lm, B_lm, C_lm, D_lm, E_lm, F_lm
// remark: don't forget to delete the return value
// after using
//***********************************************************
Real *coefficients(Real r, int l, int m)
{
    // compute the coefficients A_lm, B_lm, C_lm, D_lm, E_lm, F_lm
    Real *coeffs = new Real[6];
    coeffs[0] = 0;
    coeffs[1] = 0;
    coeffs[2] = 0;
    coeffs[3] = 0;
    coeffs[4] = 0;
    coeffs[5] = 0;

    // constant before Real-sum
    Real N_lm = ( (2.0*Real(l) + 1.0)/(4.0*Pi) ) * fac(l-m)/fac(l+m);	// TODO: simplify the factorial
    if(m != 0)				// TODO: faster check?
    {
        N_lm *= 2.0;
    }

    // first part of Real-sum (3.17) (sum over n)
    int maxn = MAX_N;
    //Real res = 0.0;
    for(int n = 0; n <= maxn; n++)
    {
        Real K_nl = 0.5*Real(n)*(Real(n) + 4.0*Real(l) + 3.0) + (Real(l) + 1.0)*(2.0*Real(l) + 1.0);    // (2.23)
        //Real I_nl = -K_nl * ( (4.0*Pi)/(pow(2.0,8.0*Real(l) + 6.0)) ) * gamm(Real(Real(n) + 4.0*Real(l) + 3.0));  // (2.31)
        Real I_nl = -K_nl * ( (4.0*Pi)/(pow(2.0,8.0*Real(l) + 6.0)) ) * gamma1_table[n][l];  // (2.31)
        //I_nl /= fac(n)*(Real(n) + 2.0*Real(l) + 1.5 ) * gamm(2.0*Real(l) + 1.5)*gamm(2.0*Real(l) + 1.5);
        I_nl /= fac(n)*(Real(n) + 2.0*Real(l) + 1.5 ) * gamma2_table[l]*gamma2_table[l];
        Real A_nl_t = 1.0/I_nl;			// t for tilde
        Real xi = (Real(r) - 1.0) / (Real(r) + 1.0);
        Real C_n = ultraspher(xi, n, 2.0*Real(l) + 1.5);
		Real C_n_prime = 0;
		if (n==0) {
			C_n_prime = 0;
		} else {
			C_n_prime = ultraspher(xi, n-1, 2.0*Real(l) + 2.5);                // derivative of Gegenbauer polynomial
		}
        Real roh_nl_t = K_nl/(2.0*Pi) * (pow(r,Real(l)))/(r*pow(1.0 + r,2.0*Real(l) + 3.0))*C_n*sqrt(4.0*Pi);
        Real phi_nl_t = - ( pow(r, Real(l))/pow(1.0 + r,2.0*Real(l)+1.0) ) * C_n * sqrt(4.0*Pi);
        Real d_phi_nl_t_dr = 0.0;
        Real term1 = (Real(l)/r ) - ( (2.0*Real(l)+1.0)/(1.0+r) );                                    // (3.26)
        term1 *= phi_nl_t;
        Real term2 = ( (4.0*(2.0*Real(l) + 1.5))/((1.0+r)*(1.0+r))) * C_n_prime;                        // (3.26)
        term2 *= sqrt(4.0*Pi) * (-1.0) * (pow(r,Real(l)))/(pow(1.0+r,2.0*Real(l)+1.0));
        d_phi_nl_t_dr = term1 + term2;

        coeffs[0] += N_lm * A_nl_t * roh_nl_t * x1_table[n][l][m];
        coeffs[1] += N_lm * A_nl_t * roh_nl_t * x2_table[n][l][m];
        coeffs[2] += N_lm * A_nl_t * phi_nl_t * x1_table[n][l][m];
        coeffs[3] += N_lm * A_nl_t * phi_nl_t * x2_table[n][l][m];
        coeffs[4] += N_lm * A_nl_t * d_phi_nl_t_dr * x1_table[n][l][m];
        coeffs[5] += N_lm * A_nl_t * d_phi_nl_t_dr * x2_table[n][l][m];
    }

    return coeffs;
}

//***********************************************************
// reconstruct the input mass density and potential and
// (both only radial component)
// compute the certesian acceleration at (r, theta, phi)
// remark: don't forget to delete the return value
// after using
//***********************************************************
Real *reconstruct(Real r, Real th, Real phi)
{
    int maxl = MAX_L;

    // compute density, potential and acceleration in spherical
    // coordinates
    Real a_r = 0, a_th = 0, a_phi = 0;
    Real roh = 0, pot = 0;         // density and potential

    for (int l = 0; l <= maxl; l++)
    {
        for(int m = 0; m <= l; m++)
        {
            //cout << l << " " << m << endl;
            Real *coeffs = coefficients(r, l, m);
            Real _A_lm = coeffs[0];
            Real _B_lm = coeffs[1];
            Real _C_lm = coeffs[2];
            Real _D_lm = coeffs[3];
            Real _E_lm = coeffs[4];
            Real _F_lm = coeffs[5];
            Real _P_lm = P_lm(cos(th), l, m);
            Real D_P_lm = DP_lm(th, l, m);
            roh += _P_lm * ( _A_lm*cos(Real(m)*phi) + _B_lm*sin(Real(m)*phi) );
            pot += _P_lm * ( _C_lm*cos(Real(m)*phi) + _D_lm*sin(Real(m)*phi) );
            Real term = _P_lm * ( _E_lm*cos(Real(m)*phi) + _F_lm*sin(Real(m)*phi) );
            a_r += term;
            a_th += D_P_lm * ( _C_lm*cos(Real(m)*phi) + _D_lm*sin(Real(m)*phi) );
            a_phi += Real(m)*_P_lm * (_D_lm*cos(Real(m)*phi) - _C_lm*sin(Real(m)*phi));
            delete [] coeffs;
        }
    }

    // multiply acceleration components with spherical gradient perfactors and negative sign
    a_r *= -(1.0);
    a_th *= -1.0/r;
    a_phi *= -1.0/(r*(sin(th)));

    // transform acceleration components to cartesian coords
    Real a_x = sin(th)*cos(phi)*a_r + cos(th)*cos(phi)*a_th - sin(phi)*a_phi;
    Real a_y = sin(th)*sin(phi)*a_r + cos(th)*sin(phi)*a_th + cos(phi)*a_phi;
    Real a_z = cos(th)*a_r - sin(th)*a_th;

    // store
    Real *res = new Real[5];
    res[0] = roh;
    res[1] = pot;
    res[2] = a_x;
    res[3] = a_y;
    res[4] = a_z;

    return res;
}

//***********************************************************
// the Hernquist distribution (2.9)
//***********************************************************
float massmodel_soft(Real r)
{
    Real eps = 0.1;
    Real r_soft = sqrt(r*r + eps*eps);
    Real roh = ( 1.0/(2.0*Pi) ) * (1.0/r_soft) * (1.0/pow(1+r_soft,3));
    return roh;
}

//***********************************************************
// go through all particles and displace the ones
// with th == 0 or th == Pi/2.0 a little bit from the pole
//***********************************************************
void displace_from_poles()
{
    for(int i = 0; i < nparticles; i++)
    {
        Real r = particles[i*dofs];
        Real th = particles[i*dofs + 1];

        if (r == 0) {
#ifdef DEBUG_NUMERICS
            warning("r == 0 value detected at index %d", i);
#endif
            particles[i*dofs] = epsilon;
        }
        if(th == 0.0){
#ifdef DEBUG_NUMERICS
            warning("theta == 0 value detected at index %d", i);
#endif
            particles[i*dofs + 1] = epsilon;
        }
        if(th == 2*Pi) {
#ifdef DEBUG_NUMERICS
            warning("theta == 2*Pi value detected at index %d", i);
#endif
            particles[i*dofs + 1] = th - epsilon;
        }

    }
}

//***********************************************************
// generate and store Hernquist sphere distribution
// with random velocities
//***********************************************************
void hernsphere()
{
    Real rohmax = massmodel_soft(0.0);		// maximal value of softened mass-model (2.9)

    int inserted = 0;

    while(inserted < nparticles)
    {
        Real r = rmax*pow(unirand(), Real(1.0/3.0));
        Real y = rohmax*unirand();

        if( massmodel_soft(r) < y)
        {
            // reject
            continue;
        }
        else
        {
            Real th = acos(2.0*unirand() - 1);
            Real phi = unirand()*2.0*Pi;
            //Real x = r*sin(th)*cos(phi);
            //Real y = r*sin(th)*sin(phi);
            //Real z = r*cos(th);
            //Real vx = unirand();
            //Real vy = unirand();
            //Real vz = unirand();
            Real vx = 0.0;
            Real vy = 0.0;
            Real vz = 0.0;
            Real m = 1.0;
            particles[inserted*dofs] = r;
            particles[inserted*dofs + 1] = th;
            particles[inserted*dofs + 2] = phi;
            particles[inserted*dofs + 3] = vx;
            particles[inserted*dofs + 4] = vy;
            particles[inserted*dofs + 5] = vz;
            particles[inserted*dofs + 6] = m;
            inserted++;
            cout << inserted << endl;
        }
    }
}

//***********************************************************
// compute acceleration at a point (r, th, phi)
// in cartesian coordinates
// remark: don't forget to free the return value
//***********************************************************
Real *accel_gravity_scf(Real r, Real th, Real phi)
{
    Real *res = reconstruct(r, th, phi);
    Real a_x = res[2];
    Real a_y = res[3];
    Real a_z = res[4];
    delete [] res;
    Real *ret = new Real[3];
    ret[0] = a_x;
    ret[1] = a_y;
    ret[2] = a_z;
    return ret;
}

//***********************************************************
// compute acceleration onto the particle with index i
// in the particles array in cartesian coordinates
// using self-consistent field
// remark: don't forget to free the return value
//***********************************************************
Real *accel_gravity_scf(int i)
{
    Real r = particles[i*dofs];
    Real th = particles[i*dofs + 1];
    Real phi = particles[i*dofs + 2];
    //cout << "accel at " << r << "," << th << "," << phi << endl;
    Real *res = reconstruct(r, th, phi);
    Real a_x = res[2];
    Real a_y = res[3];
    Real a_z = res[4];
    delete [] res;
    Real *ret = new Real[3];
    ret[0] = a_x;
    ret[1] = a_y;
    ret[2] = a_z;

#ifdef DEBUG_NUMERICS
    if (isfinite3D(ret[0], ret[1], ret[2]) == false) {
        error("bad particle after reconstruct at index %d\n", i);
    }
#endif
    return ret;
}

//***********************************************************
// compute self consistent field kick on the whole
// particle distribution in cartesian coordinates
// on the CPU
// @param nparticles Number of particles
// @param particles_old Particles with acceleration before kick
// @param particles_new Particles with acceleration after kick
// @param h Timestep for kick
//***********************************************************
void kick_scf_CPU(const int nparticles, Real* particles_old,
						   Real* particles_new, Real h) 
{
    for(int i = 0; i < nparticles; i++)
    {
        Real *res = accel_gravity_scf(i);
        Real ax = res[0];
        Real ay = res[1];
        Real az = res[2];
        particles_new[i*dofs + 3] = particles_old[i*dofs + 3] + h*ax;
        particles_new[i*dofs + 4] = particles_old[i*dofs + 4] + h*ay;
        particles_new[i*dofs + 5] = particles_old[i*dofs + 5] + h*az;
        delete [] res;
#ifdef DEBUG_NUMERICS
        if (isfinite3D(ax,ay,az) == false) {
            error("bad particle after kick at index %i\n", i);
        }
#endif
	}
}

//***********************************************************
// compute self consistent field kick on the whole
// particle distribution in cartesian coordinates
// on the GPU.
// @param nparticles Number of particles
// @param particles_old Particles with acceleration before kick
// @param particles_new Particles with acceleration after kick
// @param h Timestep for kick
//***********************************************************
void kick_scf_GPU(const int nparticles, Real* particles_old,
						   Real* particles_new, Real h) 
{
	float *accels = new float[3*nparticles];

    accel_gravity_scf_gpu(particles, nparticles, x1_table, x2_table,
                      gamma1_table, gamma2_table, accels);

    for(int i = 0; i < nparticles; i++)
    {
        float ax = accels[i*3+0];
        float ay = accels[i*3+1];
        float az = accels[i*3+2];
        particles_new[i*dofs + 3] = particles_old[i*dofs + 3] + h*ax;
        particles_new[i*dofs + 4] = particles_old[i*dofs + 4] + h*ay;
        particles_new[i*dofs + 5] = particles_old[i*dofs + 5] + h*az;
#ifdef DEBUG_NUMERICS
		if (isfinite3D(ax,ay,az) == false) {
			error("bad particle after kick at index %i\n", i);
		}
#endif
    }

    delete [] accels;
}

//***********************************************************
// compute self consistent field kick on the whole
// particle distribution in cartesian coordinates
// @param nparticles Number of particles
// @param particles_old Particles with acceleration before kick
// @param particles_new Particles with acceleration after kick
// @param h Timestep for kick
//***********************************************************
void kick_scf(const int nparticles, Real* particles_old,
							Real* particles_new, Real h)
{
	if (computeOnGPU) {
		kick_scf_GPU(nparticles, particles_old,
			particles_new, h);
	} else {
		kick_scf_CPU(nparticles, particles_old, 
			particles_new,h);
	}
}

//*************************************
// compute acceleration acting on particle at index i
// caused by all other particles in *particles
// in _cartesian_ coordinates
// with simple all pairs calculation
// remark: don't forget to delete the return value
// after using
// remark: particles must have _cartesian_ spatial coordinates
//*************************************
Real *accel_gravity_allpairs_cart(int i, Real *_particles)
{
    Real *a = new Real[3];
    //a[0] = 0.0; a[1] = 0.0; a[2] = 0.0;
    memset(a, 0, sizeof(Real)*3);

    float x_i[] = {_particles[i*dofs+0], _particles[i*dofs+1], _particles[i*dofs+2]};
    //cout << "accel at " << x_i[0] << "," << x_i[1] << "," << x_i[2] << endl;

    for(int j = 0; j < nparticles; j++)
    {
        if(i == j)
            continue;

        // compute acceleration of j onto i
        float m_j = _particles[j*dofs+6];
        float x_j[] = {_particles[j*dofs+0], _particles[j*dofs+1], _particles[j*dofs+2]};
        float r_ij[] = { (x_j[0] - x_i[0]) , (x_j[1] - x_i[1]), (x_j[2] - x_i[2])  };       // r_ij = x_j - x_i
        float dist = sqrt(r_ij[0]*r_ij[0] + r_ij[1]*r_ij[1] + r_ij[2]*r_ij[2]);
        float dist_3 = dist*dist*dist;
        float a_ij[] = { ((m_j/dist_3) * r_ij[0]), ((m_j/dist_3) * r_ij[1]), ((m_j/dist_3) * r_ij[2]) };   // accel of j ONTO i

        //cout << "accel of " << j << " onto " << i << ": (" << a_ij[0] << "," << a_ij[1] << "," << a_ij[2] << ")" << endl;

        // update force
        a[0] += a_ij[0];
        a[1] += a_ij[1];
        a[2] += a_ij[2];

        // print
        /*if(toggledebug4()) cout << "interaction: j=" << particles[j].index << " onto i=" << particles[i].index << endl;
        if(toggledebug4()) cout << "r_0=(" << particles[i].r[0] << " " << particles[i].r[1] << ")  ";
        if(toggledebug4()) cout << "v_0=(" << particles[i].v[0] << " " << particles[i].v[1] << ")  ";
        if(toggledebug4()) cout << "a=(" << particles[i].a[0] << " " << particles[i].a[1] << ")" << endl;*/
    }

    //cout << "accel onto " << i << ": (" << a[0] << "," << a[1] << "," << a[2] << ")" << endl;

    return a;
}

//*************************************
// compute acceleration acting on particle at index i
// caused by all other particles in *particles
// in _cartesian_ coordinates
// with simple all pairs calculation
// remark: don't forget to delete the return value
// after using
// remark: particles must have _spherical_ spatial coordinates
//*************************************
Real *accel_gravity_allpairs_spherical(int i, Real *_particles)
{
    Real *a = new Real[3];
    //a[0] = 0.0; a[1] = 0.0; a[2] = 0.0;
    memset(a, 0, sizeof(Real)*3);

//    cout << "accel at " << x_i[0] << "," << x_i[1] << "," << x_i[2] << endl;
    Real r = _particles[i*dofs+0];
    Real th = _particles[i*dofs+1];
    Real phi = _particles[i*dofs+2];
    Real x = r*sin(th)*cos(phi);
    Real y = r*sin(th)*sin(phi);
    Real z = r*cos(th);
    float x_i[] = {x, y, z};

    for(int j = 0; j < nparticles; j++)
    {
        if(i == j)
            continue;

        // transform coordinates to cartesian system
        Real r = _particles[j*dofs+0];
        Real th = _particles[j*dofs+1];
        Real phi = _particles[j*dofs+2];
        Real x = r*sin(th)*cos(phi);
        Real y = r*sin(th)*sin(phi);
        Real z = r*cos(th);
        float x_j[] = {x, y, z};

        // compute acceleration of j onto i
        float m_j = _particles[j*dofs+6];
        float r_ij[] = { (x_j[0] - x_i[0]) , (x_j[1] - x_i[1]), (x_j[2] - x_i[2])  };       // r_ij = x_j - x_i
        float dist = sqrt(r_ij[0]*r_ij[0] + r_ij[1]*r_ij[1] + r_ij[2]*r_ij[2]);
        float dist_3 = dist*dist*dist;
        float a_ij[] = { ((m_j/dist_3) * r_ij[0]), ((m_j/dist_3) * r_ij[1]), ((m_j/dist_3) * r_ij[2]) };   // accel of j ONTO i

        //cout << "accel of " << j << " onto " << i << ": (" << a_ij[0] << "," << a_ij[1] << "," << a_ij[2] << ")" << endl;

        // update force
        a[0] += a_ij[0];
        a[1] += a_ij[1];
        a[2] += a_ij[2];

        // print
        /*if(toggledebug4()) cout << "interaction: j=" << particles[j].index << " onto i=" << particles[i].index << endl;
        if(toggledebug4()) cout << "r_0=(" << particles[i].r[0] << " " << particles[i].r[1] << ")  ";
        if(toggledebug4()) cout << "v_0=(" << particles[i].v[0] << " " << particles[i].v[1] << ")  ";
        if(toggledebug4()) cout << "a=(" << particles[i].a[0] << " " << particles[i].a[1] << ")" << endl;*/
    }

    //cout << "accel onto " << i << ": (" << a[0] << "," << a[1] << "," << a[2] << ")" << endl;

    return a;
}

//*************************************
// integrate the system of *particles
// with leapfrog
// over a timestep of h
//*************************************
void integrate_leapfrog(Real h)
{
    Real *particles_new = new Real[dofs*nparticles];    // new config in cartesian coordinates
    Real *particles_old_cart = new Real[dofs*nparticles]; // old config in cartesian coordinates

    // copy the masses and velocities
    memcpy(particles_new, particles, sizeof(Real)*dofs*nparticles);
    memcpy(particles_old_cart, particles, sizeof(Real)*dofs*nparticles);

    // compute the cartesian coordinates of the old configuration
    for(int i = 0; i < nparticles; i++)
    {
        Real r = particles[i*dofs];
        Real th = particles[i*dofs + 1];
        Real phi = particles[i*dofs + 2];
        Real x = r*sin(th)*cos(phi);
        Real y = r*sin(th)*sin(phi);
        Real z = r*cos(th);
        particles_old_cart[i*dofs] = x;
        particles_old_cart[i*dofs + 1] = y;
        particles_old_cart[i*dofs + 2] = z;
    }

    // Drift 1/2
    for(int i = 0; i < nparticles; i++)
    {
        particles_new[i*dofs] = particles_old_cart[i*dofs] + 0.5*h*particles_old_cart[i*dofs + 3];
        particles_new[i*dofs + 1] = particles_old_cart[i*dofs + 1] + 0.5*h*particles_old_cart[i*dofs + 4];
        particles_new[i*dofs + 2] = particles_old_cart[i*dofs + 2] + 0.5*h*particles_old_cart[i*dofs + 5];
    }
    // Kick
	displace_from_poles();
	kick_scf(nparticles, particles_old_cart, particles_new, h);
	/*
    for(int i = 0; i < nparticles; i++)
    {
        Real *res = accel_gravity_scf(i);
        Real ax = res[0];
        Real ay = res[1];
        Real az = res[2];
        particles_new[i*dofs + 3] = particles_old_cart[i*dofs + 3] + h*ax;
        particles_new[i*dofs + 4] = particles_old_cart[i*dofs + 4] + h*ay;
        particles_new[i*dofs + 5] = particles_old_cart[i*dofs + 5] + h*az;
        delete [] res;
#ifdef DEBUG_NUMERICS
        if (isfinite3D(ax,ay,az) == false) {
            error("bad particle after kick at index %i\n", i);
        }
#endif
    }
	*/
    // Drift 1/2
    for(int i = 0; i < nparticles; i++)
    {
        particles_new[i*dofs] = particles_new[i*dofs] + 0.5*h*particles_new[i*dofs + 3];
        particles_new[i*dofs + 1] = particles_new[i*dofs + 1] + 0.5*h*particles_new[i*dofs + 4];
        particles_new[i*dofs + 2] = particles_new[i*dofs + 2] + 0.5*h*particles_new[i*dofs + 5];
#ifdef DEBUG_NUMERICS
        Real x = particles_new[i*dofs];
        Real y = particles_new[i*dofs+1];
        Real z = particles_new[i*dofs+2];
        if (isfinite3D(x,y,z) == false) {
            error("bad particle after second drift at index %i\n", i);
        }
#endif
    }

    // compute the spherical coordinates of the new configuration
    // and store the new configuration in *particles
    memcpy(particles, particles_new, sizeof(Real)*dofs*nparticles);
    for(int i = 0; i < nparticles; i++)
    {
        Real x = particles_new[i*dofs];
        Real y = particles_new[i*dofs + 1];
        Real z = particles_new[i*dofs + 2];
        Real r = sqrt(x*x + y*y + z*z);
        Real th = acos(z/r);
        Real phi = atan2(y,x);
        particles[i*dofs] = r;
        particles[i*dofs + 1] = th;
        particles[i*dofs + 2] = phi;
    }

    // cleanup
    delete [] particles_new;
    delete [] particles_old_cart;
}

//*************************************
// integrate the system of *particles
// with leapfrog over steps steps with
// a timestep of h per step
// and store the results to an animation
// file
//*************************************
void simulate(int steps, Real h, string _filename)
{
    // compute gamma values once and for all
    compute_gamma_tables();

    for(int i = 0; i < steps; i++)
    {
        cout << "-- step " << i << " start --" << endl;
        store_particles_3d_animation(_filename.c_str(), particles, nparticles, dofs);
        cout << "store done" << endl;
        displace_from_poles();
        compute_x_tables();
        cout << "tables done" << endl;
        integrate_leapfrog(h);
        cout << "integration done" << endl;
        cout << "-- step " << i << " end --" << endl;
    }

	// cleanup
    gpumem_cleanup();
}

//***********************************************************
//
//***********************************************************

void sandbox()
{
	gpu_test_kernel(particles, nparticles);

	printf("Press any key to continue...\n");
	getchar();
	exit(0);
}

//***********************************************************
// run the simulation code
//***********************************************************
void run() 
{
	// initialize random number generator
    srand(42);

    // init
    flush_file(outputParticleFile.c_str());

    // generate the system
    hernsphere();

    // simulate the dynamics
    simulate(nSteps, stepWidth, outputParticleFile.c_str());

    if (plotAndShow) {
        // create a gnuplot file
        printf("creating animation file...\n");
        plotParticles(outputParticleFile.c_str(), nSteps, outputGifFile.c_str());

        // play the animated gif
        playAnimation(outputGifFile.c_str());
    }
}

//***********************************************************
// show program help and quit
//***********************************************************
void help()
{
	printf("Usage: fieldcode [options]\n");
	printf("\n");
    printf("Options:\n");
    printf("  --help                Show help and quit\n");
    printf("  --outfileParticles    Output file for particle data (default: %s)\n", outputParticleFile.c_str());
    printf("  --outfileAnimation    Output file for animated gif (default: %s)\n", outputGifFile.c_str());
	printf("                        (Creating the gif needs gnuplot installed)\n");
	string computeOnGPUStr = computeOnGPU ? "true" : "false"; 
	printf("  --gpu                 Compute on GPU (true) or on CPU (false) (default: %s)\n", computeOnGPUStr.c_str());
	printf("  --timesteps           Number of timesteps to simulate (default: %d)\n", nSteps);
	printf("  \n");
    printf("Description: The self-consistent fieldcode is a fast (O(N)) algorithm\n");
	printf("for solving the cosmological N-Body problem. This program allows to\n");
	printf("run the algorithm either on the CPU or on a CUDA enabled graphics card\n");
	printf("See docs/thesis.pdf for more information\n");
	printf("Several paramters are currently hardcoded:\n");
	printf("The program simulates a Hernquist sphere with %d particles, a stepwidth\n", nparticles);
	printf("of %f and expansion coefficients of n_max=%d and l_max=%d\n", stepWidth, MAX_N, MAX_L);

#ifdef _DEBUG
	getchar();
#endif
	exit(0);
}

//***********************************************************
// process command line arguments
//***********************************************************
void parseCmdLineArgs(int argc, char* argv[])
{
	for (unsigned int i = 1; i < argc; i+=2) {
		string variableArg(argv[i]);
		string valueArg(argv[i+1]);

		if (variableArg=="--outfileParticles") {
			outputParticleFile = valueArg;
		}
		else if (variableArg=="--outfileAnimation") {
			outputGifFile = valueArg;
		}
		else if(variableArg=="--gpu") {
			computeOnGPU = (valueArg == "true") ? true : false;
		}
		else if(variableArg=="--timesteps") {
			nSteps = atoi(valueArg.c_str());
		}
		else 
		{
			help();
		}
	}
}

//***********************************************************
// the main function ^^
//***********************************************************
int main(int argc, char* argv[])
{
#if defined(DEBUG_SANDBOX)
	sandbox();
#endif

	// stop if not even number of optional arguments
	if (argc %2 == 0) {
		help();
	}

	// parse command line arguments
	parseCmdLineArgs(argc, argv);

	// run program
	run();

	// quit
	printf("Press any key to continue ...\n");
    getchar();

}

