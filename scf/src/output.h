#ifndef __OUTPUT_H
#define __OUTPUT_H

#include <iostream>
#include <fstream>
#include <string>
#include "Real.h"
using namespace std;



//***********************************************************
// print warning message
//***********************************************************
void warning(const char *fmt, ...);

//***********************************************************
// print error message and close
//***********************************************************
void error(const char *fmt, ...);

//***********************************************************
// compute and store density in file (only radial component)
//***********************************************************
void store_density(int ncells, Real *particles, Real rmax, Real rmin, int nparticles, int dofs);

//***********************************************************
// compute and store potential and acceleration
// in file (only radial component)
//***********************************************************
void store_potential(int ncells, Real *particles, Real rmax, Real rmin, int nparticles, int dofs);

//***********************************************************
// sample some output acceleration vectors in the field
//***********************************************************
void sample_accel3d(int nsteps, Real rmax, Real rmin, int nparticles, int dofs);

//***********************************************************
// store to file (3-dimensional) (directly store the points)
//***********************************************************
void store_particles_3d_raw(Real *particles, int nparticles, int dofs);

//***********************************************************
// store to file (3-dimensional) (directly store the points)
//***********************************************************
void store_particles_3d_raw(string _filename, Real *particles, int nparticles, int dofs);

//***********************************************************
// store to file (3-dimensional) (directly store the points)
// at the end of the file
// repeated calling generates a gnuplot-readable animation
// file
//***********************************************************
void store_particles_3d_animation(string _filename, Real *particles, int nparticles, int dofs);

//***********************************************************
// compute and store the reconstructed mass density,
// potential (both only radial part)
//***********************************************************
void reconstruction(int ncells, Real th, Real phi, Real rmax, Real rmin);

//***********************************************************
// flush the given file in folder /results
//***********************************************************
void flush_file(string _filename);

//***********************************************************
// print all the particles
//***********************************************************
void print_particles(Real *particles, int nparticles, int dofs);

//***********************************************************
// print all the particles in cartesian coordinates
//***********************************************************
void print_particles_cart(Real *particles, int nparticles, int dofs);

//***********************************************************
// compute and print all the terms of A_lm(r) for
// fixed l, m and r
// toggledebug1 must be set to true
//***********************************************************
void print_terms(int l, int m, Real r, Real *particles, int nparticles, int dofs);

//***********************************************************
// Plot particle dynamics as animated gif
// @param inputFile The input particle file
// @param nSteps Number of timesteps in inputFile
// @param outputFile Gif animated output file
// @return If plotting the particles succeded
//***********************************************************
bool plotParticles(const char* particleFile, const int nSteps, const char* outputFile);

//***********************************************************
// Play an animated gif
// @param animationFile The animation file to play
// @return If playing the file succeeded
//***********************************************************
bool playAnimation(const char *animationFile);

#endif
