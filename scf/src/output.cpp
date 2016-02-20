#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdarg.h>
#include "main.h"
#include "Real.h"
#include "output.h"
using namespace std;

const Real Pi = Real(M_PI);

void warning(const char *fmt, ...)
{
    va_list arg;

    va_start(arg, fmt);

    fprintf(stderr, "Warning - ");
    vfprintf(stderr, fmt, arg);
    fprintf(stderr, "\n");

    va_end(arg);
}

void error(const char *fmt, ...)
{
    va_list arg;

    va_start(arg, fmt);

    fprintf(stderr, "Error - ");
    vfprintf(stderr, fmt, arg);
    fprintf(stderr, "\n");

    va_end(arg);

    getchar();
    exit(1);
}

void store_density(int ncells, Real *particles, Real rmax, Real rmin, int nparticles, int dofs)
{
    ofstream file;
    file.open ("inputdens.txt");

    Real width = (rmax - rmin)/Real(ncells);

    for(int i = 0 ; i < ncells; i++)
    {
        Real mp = Real(i)*width + 0.5*width;		// midpoint of current cell

        // count all particles within this range
        int N = 0;
        Real min = Real(i)*width;
        Real max = Real(i+1)*width;
        for(int j = 0; j < nparticles; j++)
        {
            Real r = particles[j*dofs];
            if(	r >= min && r < max)
            {
                N++;
            }
        }

        // divide by volume of spherical shell
        Real V = (4.0/3.0)*Pi*( pow(max,Real(3.0)) - pow(min,Real(3.0)) );
        Real n = Real(N)/V;

        file << mp << "\t" << n << "\n";

    }

    file.close();
}

void store_potential(int ncells, Real *particles, Real rmax, Real rmin, int nparticles, int dofs)
{
    ofstream file_pot;
    ofstream file_acc;
    file_pot.open ("inputpot.txt");
    file_acc.open ("inputacc.txt");

    Real width = (rmax - rmin)/Real(ncells);
    //Real pot_vals[ncells];
	Real *pot_vals = new Real[ncells];

    // compute the potential
    for(int i = 0 ; i < ncells; i++)
    {
        Real mp = Real(i)*width + 0.5*width;		// midpoint of current cell
        Real th = Pi/2.0;							    // angles do not matter
        Real phi = Pi;
        Real x = mp*sin(th)*cos(phi);
        Real y = mp*sin(th)*sin(phi);
        Real z = mp*cos(th);

        // compute potential at this point
        Real pot = 0.0;
        for(int k = 0 ; k < nparticles; k++)
        {
            Real rk = particles[dofs*k];
            Real thk = particles[dofs*k + 1];
            Real phik = particles[dofs*k + 2];
            Real mk = particles[dofs*k + 6];
            Real xk = rk*sin(thk)*cos(phik);
            Real yk = rk*sin(thk)*sin(phik);
            Real zk = rk*cos(thk);
            Real dist = sqrt( (x-xk)*(x-xk) + (y-yk)*(y-yk) + (z-zk)*(z-zk) );
            pot += -mk/dist;
        }

        file_pot << mp << "\t" << pot << "\n";
        pot_vals[i] = pot;
    }

    // compute the acceleration
    // = -d/dr(phi)
    for(int i = 0 ; i < (ncells - 1); i++)
    {
        Real mp = Real(i+1)*width;
        Real rdist = width;
        Real valdist = pot_vals[i+1] - pot_vals[i];
        Real diff = valdist/rdist;
        Real a = -diff;
        file_acc << mp << "\t" << a << "\n";
    }

    file_pot.close();
	delete[] pot_vals;

}

void sample_accel3d(int nsteps, Real rmax, Real rmin, int nparticles, int dofs)
{
    Real rstep = rmax/Real(nsteps);
    Real thstep = Pi/(0.5*Real(nsteps));
    Real phistep = (2.0*Pi)/(0.5*Real(nsteps));
    ofstream file_acc;
    file_acc.open ("outputacc3d.txt");

    for(int i = 0; i < nsteps; i++)
    {
        for(int j = 0; j < nsteps; j++)
        {
            for(int k = 0; k < nsteps; k++)
            {
                //cout << ".." << endl;
                Real r = Real(i)*rstep;
                Real th = Real(j)*thstep;
                Real phi = Real(k)*phistep;
                Real x = r*sin(th)*cos(phi);
                Real y = r*sin(th)*sin(phi);
                Real z = r*cos(th);
                Real *res = reconstruct(r, th, phi);
                Real len = sqrt(res[2]*res[2] + res[3]*res[3] + res[4]*res[4]);
                Real a_x = res[2]/(10.0*len);
                Real a_y = res[3]/(10.0*len);
                Real a_z = res[4]/(10.0*len);
                file_acc << x << "\t" << y << "\t" << z << "\t" << a_x << "\t" << a_y << "\t" << a_z << "\n";
                delete [] res;
            }
        }
    }

    file_acc.close();
}

void store_particles_3d_raw(Real *particles, int nparticles, int dofs)
{
    ofstream file;
    file.open ("inputdens3d.txt");

    for(int i = 0; i < nparticles; i++)
    {
        Real r = particles[i*dofs];
        Real th = particles[i*dofs + 1];
        Real phi = particles[i*dofs + 2];
        Real x = r*sin(th)*cos(phi);
        Real y = r*sin(th)*sin(phi);
        Real z = r*cos(th);
        file << x << "\t" << y << "\t" << z << endl;
    }

    file.close();
}

void store_particles_3d_raw(string _filename, Real *particles, int nparticles, int dofs)
{
    ofstream file;
    string filename = "";
    filename.append(_filename);
    file.open (filename.c_str());

    for(int i = 0; i < nparticles; i++)
    {
        Real r = particles[i*dofs];
        Real th = particles[i*dofs + 1];
        Real phi = particles[i*dofs + 2];
        Real x = r*sin(th)*cos(phi);
        Real y = r*sin(th)*sin(phi);
        Real z = r*cos(th);
        file << x << "\t" << y << "\t" << z << endl;
    }

    file.close();
}

void store_particles_3d_animation(string _filename, Real *particles, int nparticles, int dofs)
{
    ofstream file;
    string filename = "";
    filename.append(_filename);
    file.open (filename.c_str(), ios::app);

    file << endl << endl;            // new gnuplot-animation frame
    for(int i = 0; i < nparticles; i++)
    {
        Real r = particles[i*dofs];
        Real th = particles[i*dofs + 1];
        Real phi = particles[i*dofs + 2];
        Real x = r*sin(th)*cos(phi);
        Real y = r*sin(th)*sin(phi);
        Real z = r*cos(th);
        file << x << "\t" << y << "\t" << z << endl;
    }

    file.close();
}

void reconstruction(int ncells, Real th, Real phi, Real rmax, Real rmin)
{
    ofstream file_dens;
    ofstream file_pot;
    ofstream file_acc;
    /*file_dens.open ("results/outputdens.txt");
    file_pot.open ("results/outputpot.txt");
    file_acc.open ("results/outputacc3d.txt");*/
    char filename_dens[128];
    char filename_pot[128];
    sprintf(filename_dens, "outputdens_%f_%f.txt", th, phi);
    sprintf(filename_pot, "outputpot_%f_%f.txt", th, phi);
    //cout << filename_dens << endl;
    //cout << filename_pot << endl;
    file_dens.open (filename_dens);
    file_pot.open (filename_pot);
    //file_pot.open("outputpot_1_570796_3_141593.txt");

    Real width = (rmax - rmin)/Real(ncells);

    // reconstruct density and potential
    for(int i = 0 ; i < ncells; i++)
    {
        //cout << i << endl;
        Real r = Real(i)*width + 0.5*width;		// midpoint of current cell
        //Real th = Pi/2.0;							// angles do not matter
        //Real phi = Pi;

        // get reconstructed density at this point
        Real *res = reconstruct(r, th, phi);

        file_dens << r << "\t" << res[0] << "\n";
        file_pot << r << "\t" << res[1] << "\n";
        delete [] res;
    }

    /*
    // compute acceleration field
    int ncells_prime = int( pow((Real)ncells,1.0/3.0) );  // number of cells in each dimension
    width = (rmax - rmin)/Real(ncells_prime);
    for(int i = 0 ; i < ncells_prime; i++)
    {
        for(int j = 0 ; j < ncells_prime; j++)
        {
            for(int k=0; k < ncells_prime; k++)
            {
                cout << i*j*k << endl;
                Real offset = (rmax-rmin)/2.0;
                Real x = -offset + Real(i)*width + 0.5*width;
                Real y = -offset + Real(j)*width + 0.5*width;
                Real z = -offset + Real(k)*width + 0.5*width;

                Real r = sqrt(x*x + y*y + z*z);
                Real th = acos(z/r);
                Real phi = atan(y/x);
                Real *res = reconstruct(r,th,phi);
                file_acc << x << "\t"  << y << "\t" << z << "\t" << res[2] << "\t" << res[3] << "\t" << res[4] << "\n";
                delete [] res;
            }
        }
    }
    */

    file_dens.close();
    file_pot.close();
    //file_acc.close();
}

void flush_file(string _filename)
{
    ofstream file;
    string filename = "";
    filename.append(_filename);
    file.open (filename.c_str(), ios::trunc);
    file.close();
}

void print_particles(Real *particles, int nparticles, int dofs)
{
    for(int i = 0; i < nparticles; i++)
    {
        cout << i << ": " << particles[i*dofs + 0] << " " << particles[i*dofs + 1];
        cout << " " << particles[i*dofs + 2] << " " << particles[i*dofs + 6] << endl;
    }
}

void print_particles_cart(Real *particles, int nparticles, int dofs)
{
    for(int i = 0; i < nparticles; i++)
    {
        Real r = particles[i*dofs + 0];
        Real th = particles[i*dofs + 1];
        Real phi = particles[i*dofs + 2];
        Real vx = particles[i*dofs + 3];
        Real vy = particles[i*dofs + 4];
        Real vz = particles[i*dofs + 5];
        Real x = r*sin(th)*cos(phi);
        Real y = r*sin(th)*sin(phi);
        Real z = r*cos(th);
        cout << i << ": (" << x << " , " << y;
        cout << " , " << z << ")\t";
        cout << " " << "(" << vx << " , " << vy;
        cout << " , " << vz << ")";
        cout << "\t" << particles[i*dofs + 6] << endl;
    }
}

void print_terms(int l, int m, Real r, Real *particles, int nparticles, int dofs)
{

    // cout << "(" << l << "," << m << "," << r << ")" << ": " << A_lm(r, l, m) << endl;
    cout << "particles: " << endl;
    print_particles(particles, nparticles, dofs);
    cout << "evaluation of C_" << l << "_" << m << "(" << r << "): " << endl;
    Real *coeffs = coefficients(r,l,m);
    delete [] coeffs;
}

//***********************************************************
// Write gnuplot script for plotting particle dynamics
// @param particleFile File with particle data to plot
// @param nSteps Number of timesteps in particleFile
// @param outputAnimationFile Output animated gif file
// @param outputScriptFile Output gnuplot script file
// @return If writing the script succeeded
//***********************************************************
bool createGnuplotScript(const char* particleFile, const unsigned int nSteps,
                            const char* outputAnimationFile, const char* outputScriptFile)
{
    ofstream outFile;
    outFile.open (outputScriptFile);

    if (outFile.good()) {
        outFile << "clear" << endl;
        outFile << "reset" << endl;
        outFile << "set terminal gif animate delay 10" << endl;
        outFile << "set output \"" << string(outputAnimationFile) << "\"" << endl;
        outFile << "unset autoscale" << endl;
        outFile << "set xrange [-3:3]" << endl;
        outFile << "set yrange [-3:3]" << endl;
        outFile << "set zrange [-3:3]" << endl;

        for (unsigned int i = 0; i < nSteps; i++) {
            outFile << "splot '" << particleFile << "' index " << i << " with dots" << endl;
        }
        outFile.close();
    } else {
        return false;
    }

    return true;
}

bool plotParticles(const char* particleFile, const int nSteps, const char* outputFile)
{
    string gnuplotTempScript = "gnuplotTempScript.gp";

    // create temporary gnuplot animation script
    if (createGnuplotScript(particleFile, nSteps, outputFile, gnuplotTempScript.c_str()) )
    {
        // call gnuplot with the animation script
        string gnuplotCall = "gnuplot " + gnuplotTempScript;
        int res = system(gnuplotCall.c_str());
        if (res != 0) {
            warning("calling gnuplot to plot the animation file %s seems to have failed", gnuplotTempScript.c_str());
        }

        // delete temporary gnuplot animation script
        res = std::remove(gnuplotTempScript.c_str());
        if (res != 0) {
            warning("temporary gnuplot file %s could not be deleted", gnuplotTempScript.c_str());
        }
    } else {
        return false;
    }

    return true;
}

bool playAnimation(const char *animationFile)
{
#if defined(__WIN32__) || defined(_WIN32)
    string playCall = "start " + string(animationFile);
    int res = system(playCall.c_str());
    if (res != 0) {
        warning("playing the animation file %s seems to have failed", animationFile);
    }
#else
#error playAnimation() not implemented
#endif


    return true;
}


