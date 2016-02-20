# -*- coding: utf-8 -*-

import sys
glass_basis('basis.pixels')

from ga import *
import time
import sys

logfile = open('log_metaopt.txt', 'w')

#-------------------------------------------------------------------------------
# perform the meta optimization and write output to the folder plotdata2d
# as a 2D parameter landscape
# Parameters to optimize are (psize, mrate) while (xrate, erate) are fixed
# For all the runs of the GA, we have a fixed number of evaluations nevals
# that are generated during one run
#-------------------------------------------------------------------------------
def metaopt2d(threshold, xrate):		

	psize_steps = 5
	mrate_steps = 5
	ranges = ranges=[[0.0,0.16],[0.0,0.047],[0.0,10.5],[0.0,5.0]]		# ranges for the actual GA 
	minparms = []					# best parameters found with threshold optimization
	min_function_calls = -1
	iters = 1									# number of runs of the GA for every different 
												# meta-parameter set
	psize_range = [20,90]						# ranges for the meta-optimization
	mrate_range = [0.0, 0.4]
	psize_step = (psize_range[1]-psize_range[0])/float(psize_steps-1)
	mrate_step = (mrate_range[1]-mrate_range[0])/float(mrate_steps-1)	
	maxevals = 5000							# maximal allow number of fitness function calls
												# until termination for one GA
	
												
	# show start information
	print_start(psize_range, mrate_range, xrate, threshold)
			
	# create the fitness matrix
	fitmat_calls = []
	fitmat_min = []
	fitmat_mean = []
	for i in range(psize_steps):
		fitmat_calls.append(range(mrate_steps))
		fitmat_min.append(range(mrate_steps))
		fitmat_mean.append(range(mrate_steps))
	
	# optimize
	counter = 0
	for i in range(psize_steps):
		psize = psize_range[0] + int( float(i)*psize_step)
		for j in range(mrate_steps):
			
			# print current meta parameters
			mrate = mrate_range[0] + float(j)*mrate_step	
			print >>logfile,  psize, " ", mrate, " ", xrate, ":"
			print "\n", psize, " ", mrate, " ", xrate
			
			# perform the ga for these parameters
			callsum = 0
			minsum = 0
			meansum = 0
			for k in range(iters):
				print >>logfile, "calling GA"
				calls, mmin, mean = ga(psize, mrate, xrate, maxevals, ranges, threshold, False)
				counter += 1
				callsum = counter
				minsum = counter
				meansum = counter
				# TODO: print outcome
				
			# store statistics
			avg_calls = float(callsum)/float(iters)
			avg_min = float(minsum)/float(iters)
			avg_mean = float(meansum)/float(iters)
			print >>logfile, avg_calls
			print >>logfile, avg_min
			print >>logfile, avg_mean

			# update best meta parameters
			if min_function_calls == -1:
				min_function_calls = function_calls
				minparms = [psize, mrate, xrate]
			if function_calls < min_function_calls:
				min_function_calls = function_calls
				minparms = [psize, mrate, xrate]
				
			# write result to fitness matrix
			fitmat_calls[i][j] = avg_calls
			fitmat_min[i][j] = avg_min
			fitmat_mean[i][j] = avg_mean
	
	# write matrix to file
	file_calls = open("./results/fitmat_calls.dat", 'w')
	file_min = open("./results/fitmat_min.dat", 'w')
	file_mean = open("./results/fitmat_mean.dat", 'w')
	for line in fitmat_calls:
		for fitness in line:
			file_calls.write(repr(fitness) + "\t")
		file_calls.write("\n")
	for line in fitmat_min:
		for fitness in line:
			file_min.write(repr(fitness) + "\t")
		file_min.write("\n")
	for line in fitmat_mean:
		for fitness in line:
			file_mean.write(repr(fitness) + "\t")
		file_mean.write("\n")

	# print call matrix
	print fitmat_calls	

	# print best meta parameters
	print "\nbest parameters", minparms, " , evaluations: ", min_function_calls

	# cleanup master script
	file_calls.close()
	file_min.close()
	file_mean.close()
	logfile.close()
	
	# return optimized meta parameters
	return minparms

#-------------------------------------------------------------------------------
# print start parameters for meta optimization
#-------------------------------------------------------------------------------
def print_start(psize_range, mrate_range, xrate, threshold):
	
	print "***********Start of the meta optimization***************"
	print "Search range for population size M =", psize_range
	print "Search range for the mutation rate R_muatation =", mrate_range  
	print "Fixed mating rate R_mating = ", xrate
	print "Fitness threshold = ", threshold 
	print "********************************************************"

#-------------------------------------------------------------------------------
# print result of the meta optimization
#-------------------------------------------------------------------------------
def print_result(minparms, threshold):
	
	print ""
	print ""
	print "***********Result of the meta optimization**************"
	print "Fitness threshold: "
	print "threshold=", threshold
	print "Best found parameters:"  
	print "Population size M:", minparms[0]
	print "Mutation rate R_mutation:", minparms[1]
	print "Fixed mating rate R_mating:", minparms[2]
	print "********************************************************"
	print ""
	print ""

#-------------------------------------------------------------------------------
# DRIVER
#-------------------------------------------------------------------------------


# initialize processes for the GA script 
init_processes()

# metaoptimize
seed(42)
threshold = 0.2
xrate = 0.5
minparms = metaopt2d(threshold, xrate)

# cleanup master script and GA script
terminate_processes()
lensplot_cleanup()
logfile2.close()

# show result
print_result(minparms, threshold)


"""
init_processes()
res1 = ga(30, 0.3, 0.5, 100,ranges=[[0.0,0.16],[0.0,0.047],[0.0,10.5],[0.0,5.0]] , threshold=0.2, tofile = False)
#res2 = ga(18, 0.2, 0.5, 30,ranges=[[0.0,0.16],[0.0,0.047],[0.0,10.5],[0.0,5.0]] , threshold=0.2, tofile = False)
print res1
#print res2
terminate_processes()
lensplot_cleanup()
logfile2.close()
"""

"""
# some test to determine the time taken by one single GA
init_processes()

t0= time.clock()
res1 = ga(48, 0.3, 0.5, 5000,ranges=[[0.0,0.16],[0.0,0.047],[0.0,10.5],[0.0,5.0]] , threshold=0.2, tofile = False)
t= time.clock() - t0
print "time taken: ", t

terminate_processes()
lensplot_cleanup()
logfile2.close()
"""
