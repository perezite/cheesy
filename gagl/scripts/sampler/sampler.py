# create a plot of the fitness landscape of
# only the first two parameters
# and afterwards only the third and fourth parameter

import sys
glass_basis('basis.pixels')

from lensing import *
import inspect, os

refparms = [0.016, 0.0047, 6.0, 1.0]

#-------------------------------------------------------------------------------
# sample the fitness landsacpe matrix of the first and second parameter
# and store it to the file fitness.txt
#-------------------------------------------------------------------------------
def sample_fitness(pranges, filename, nsteps = 20, numfit=True, typefit=True, distfit=True, timefit=True):
	
	# create the fitness matrix
	fitmat = []
	for i in range(nsteps):
		fitmat.append(range(nsteps))

	xstep = (pranges[0][1]-pranges[0][0])/float(nsteps)
	ystep = (pranges[1][1]-pranges[1][0])/float(nsteps)
	
	# generate the matrix
	for i in range(nsteps):
		x = xstep*float(i+1)
		for j in range(nsteps):
			print i, " ", j
			sys.stdout.flush()
			y = ystep*float(j+1)
			print "(i,j)=", i, " ", j, " ", "(x,y)=", x, " ", y
			sys.stdout.flush()
			fitness = lensing_calc_fitness_restricted([x,y,refparms[2],refparms[3]], numfit, typefit, distfit, timefit)	
			fitmat[i][j] = fitness

	# print matrix
	print fitmat
	sys.stdout.flush()
	
	# print matrix
	for i in range(nsteps):
		for j in range(nsteps):
			print str(fitmat[i][j]) , "\t",
		print "\n",
	sys.stdout.flush()

	# write matrix to file
	f = open(filename, 'w')
	outstr = ''
	for i in range(nsteps):
		for j in range(nsteps):
			outstr += str(fitmat[i][j]) + "\t"
		outstr += "\n"
	f.write(outstr)
	f.close()
	
#-------------------------------------------------------------------------------
# sample the fitness landsacpe matrix of the 3rd and 4th parameter
# and store it to the file fitness2.txt
#-------------------------------------------------------------------------------
def sample_fitness2(pranges, filename, nsteps = 20, numfit=True, typefit=True, distfit=True, timefit=True):
	
	# create the fitness matrix
	fitmat = []
	for i in range(nsteps):
		fitmat.append(range(nsteps))

	xstep = (pranges[0][1]-pranges[0][0])/float(nsteps)
	ystep = (pranges[1][1]-pranges[1][0])/float(nsteps)
	
	# generate the matrix
	for i in range(nsteps):
		x = xstep*float(i+1)
		for j in range(nsteps):
			print i, " ", j
			sys.stdout.flush()
			y = ystep*float(j+1)
			print "(i,j)=", i, " ", j, " ", "(x,y)=", x, " ", y
			sys.stdout.flush()
			fitness = lensing_calc_fitness_restricted([refparms[0],refparms[1],x,y], numfit, typefit, distfit, timefit)	
			fitmat[i][j] = fitness

	# print matrix
	print fitmat
	sys.stdout.flush()
	
	# print matrix
	for i in range(nsteps):
		for j in range(nsteps):
			print str(fitmat[i][j]) , "\t",
		print "\n",
	sys.stdout.flush()

	# write matrix to file
	f = open(filename, 'w')
	outstr = ''
	for i in range(nsteps):
		for j in range(nsteps):
			outstr += str(fitmat[i][j]) + "\t"
		outstr += "\n"
	f.write(outstr)
	f.close()

#-------------------------------------------------------------------------------
# Print start information
#-------------------------------------------------------------------------------
def print_start(sx_range, sy_range, zs_range, zl_range):
	
	print "***********Start of brute force search******************"
	print "Search range for s_x =", sx_range
	print "Search range for s_y =", sy_range
	print "Search range for z_s =", zs_range
	print "Search range for z_l =", zl_range
	print "********************************************************"

#-------------------------------------------------------------------------------
# Print result
#-------------------------------------------------------------------------------
def print_result():
	
	currentScriptFile = inspect.getfile(inspect.currentframe())
	outputDirectoryPath = os.path.dirname(os.path.abspath(currentScriptFile)) + "/results"
	print "********************************************************"
	print "Output data written to:", outputDirectoryPath
	print "********************************************************"
		
#-------------------------------------------------------------------------------
# DRIVER
#-------------------------------------------------------------------------------

samplesteps = 5
sx_range = [0.0, 0.16]
sy_range = [0.0,0.047]
zs_range = [0.0, 10.5]
zl_range = [0.0, 5.0]

# show startup information
print_start(sx_range, sy_range, zs_range, zl_range)

# sample landscape for first two parameters
ranges = [sx_range, sy_range]
print "all:"
sys.stdout.flush()
sample_fitness(ranges, 'results/allfit1.txt', samplesteps, numfit=True, typefit=True, distfit=True, timefit=True)
print "dist:"
sys.stdout.flush()
sample_fitness(ranges, 'results/distfit1.txt', samplesteps, numfit=False, typefit=False, distfit=True, timefit=False)
print "time:"
sys.stdout.flush()
sample_fitness(ranges, 'results/timefit1.txt', samplesteps, numfit=False, typefit=False, distfit=False, timefit=True)
print "numfit:"
sys.stdout.flush()
sample_fitness(ranges, 'results/numfit1.txt', samplesteps, numfit=True, typefit=False, distfit=False, timefit=False)
print "type:"
sys.stdout.flush()
sample_fitness(ranges, 'results/typefit1.txt', samplesteps, numfit=False, typefit=True, distfit=False, timefit=False)


# sample landscape for second two parameters

ranges = [zs_range, zl_range]
print "dist:"
sys.stdout.flush()
sample_fitness2(ranges, 'results/distfit2.txt', samplesteps, numfit=False, typefit=False, distfit=True, timefit=False)
print "time:"
sys.stdout.flush()
sample_fitness2(ranges, 'results/timefit2.txt', samplesteps, numfit=False, typefit=False, distfit=False, timefit=True)
print "numfit:"
sys.stdout.flush()
sample_fitness2(ranges, 'results/numfit2.txt', samplesteps, numfit=True, typefit=False, distfit=False, timefit=False)
print "type:"
sys.stdout.flush()
sample_fitness2(ranges, 'results/typefit2.txt', samplesteps, numfit=False, typefit=True, distfit=False, timefit=False)

# show output directory path
print_result()

