# -*- coding: utf-8 -*-
# full optimization

import sys
import glass

from math import *
from random import *
from lensplot import *
from copy import *
from myrand import *
import subprocess
import pickle

# program configuration
nparms = 4
n_processors = 8

# global variables
processes = []									# all the processes used in the program
minc = []
meanc = []
callsc = []
minchrom = []

# debug configuration
debug = True

# logs
logfile2 = open('log_ga.txt', 'w')

# globals
refparms = [0.016, 0.0047, 6.0, 1.0]

# keep track of number of fitness function calls
function_calls = 0
best_fitness = 1000000.

#-------------------------------------------------------------------------------
# fill the processes list with idling processes
#-------------------------------------------------------------------------------
def init_processes():
	myprocesses = []
	for i in range(n_processors):
		program_args = ['python', glass.getGlassMainScriptPath(), 'lensing.py', 'argfile.txt', str(i)]
		p = subprocess.Popen(program_args, bufsize=-1, stdin=subprocess.PIPE, stdout=subprocess.PIPE)	
		myprocesses.append(p)
		
	global processes
	processes = myprocesses
	
#-------------------------------------------------------------------------------
# send a termination signal to the subprocesses
#-------------------------------------------------------------------------------
def terminate_processes():
	ret = []
	for i in range(n_processors):
		pickle.dump("signal_terminate", processes[i].stdin)
		
#-------------------------------------------------------------------------------
# return the slicing list for j jobs and p processors
#-------------------------------------------------------------------------------
def slicing(j, p):	

	slices = []
	ssum = 0      

	for i in range(p):
		njobs = floor(float(j)/float(p))
		if i < (j%p):
			njobs += 1
		njobs = int(njobs)
		slices.append([])
		for k in range(njobs):
			slices[i].append(ssum + k)		 
		ssum += njobs

	return slices

#-------------------------------------------------------------------------------
# compute the fitness of the whole population in parallel
#-------------------------------------------------------------------------------
def calc_fitness_population(pop, changemask = None):	

	popsize = len(pop)/2
	popsize = int(popsize)
	fitpop = pop							# same population as before, but with correct fitness
	
	# if no changemask has been passed, just evaluate everything
	if(changemask==None):
		psize = len(pop)/2
		psize = int(psize)
		changemask = []
		for i in range(psize):
			changemask.append(True)
	
	# generate the reduced population array only containing the changed
	# citizens since last generation
	red_pop = []
	for i in range(len(changemask)):
		if changemask[i] == True:
			red_pop.append(pop[2*i])
			red_pop.append(pop[2*i+1])
	red_fitpop = copy(red_pop)
	red_popsize = len(red_pop)/2
	red_popsize = int(red_popsize)
	
	# distribute the work over the processors
	slices = slicing(red_popsize, n_processors)
	
	# send parameters to processes
	for i in range(len(processes)):
		args = []						# list of parameters to be passed to this process
		for idx in slices[i]:
			args.append(red_pop[idx*2])
		pickle.dump(args, processes[i].stdin)
		processes[i].stdin.flush()
	args = []
		
	# wait for an answer from the subprocesses
	results = range(len(processes))					# results must be in same order as processes !
	for i in range(len(processes)):
		results[i] =  pickle.load(processes[i].stdout)	
		
	# parse the fitnesses received from processes
	# and put them into the population array
	for i in range(len(processes)):
		for j in range(len(results[i])):
			fitness = results[i][j]
			idx = slices[i][j]
			red_fitpop[2*idx+1] = fitness
			
	# copy the fitnesses from the reduced population 
	# array back to the full population
	inserted = 0
	for i in range(len(changemask)):
		if changemask[i] == True:
			fitpop[2*i] = copy(red_fitpop[2*inserted])
			fitpop[2*i+1] = red_fitpop[2*inserted + 1]
			inserted += 1
      
	# update number of function calls
	global function_calls
	function_calls += red_popsize
      
	return fitpop
#-------------------------------------------------------------------------------
# return the next smallest even integer number or num itself if nums is even
# num must be integer
# tested
#-------------------------------------------------------------------------------
def flooreven(num):
	if num % 2 == 0: 
		return num
	else:
		return num-1
		
#-------------------------------------------------------------------------------
# return random variable within (0,1)
# to be used to for consistency with MATLAB rand() function
# tested
#-------------------------------------------------------------------------------
def unirand():
	trial = 0.0
	while trial == 0.0:
		trial = random()
	return trial	

#-------------------------------------------------------------------------------
# update statistic variables
# tested
#-------------------------------------------------------------------------------
def statistics(pop):

	global minc, meanc, callsc, function_calls
	psize = len(pop)/2	
	psize = int(psize)

	minc.append(pop[1])
	minchrom.append(pop[0])
	callsc.append(function_calls)
	costsum = 0
	for i in range(psize):
		costsum += pop[i*2+1]
	meanc.append(costsum/float(psize))	

#-------------------------------------------------------------------------------
# store statistic arrays to files
# precondition: population must be sorted
# tested
#-------------------------------------------------------------------------------
def store_statistics():

	f1 = open('results/minc.txt', 'w')
	f2 = open('results/meanc.txt', 'w')
	f3 = open('results/minchrom.txt', 'w')
	for i in range(len(minc)):
		outstr1 = ''
		outstr1 += str(i)
		outstr1 += " " 
		outstr1 += str( minc[i] )
		outstr1 += "\n"
		outstr2 = ''
		outstr2 += str(i)
		outstr2 += " " 
		outstr2 += str (meanc[i] )
		outstr2 += "\n"
		outstr3 = ''
		outstr3 += str(minchrom[i][0]) + "\t" + str(minchrom[i][1]) + "\t"
		if nparms == 4:
			outstr3 += str(minchrom[i][2]) + "\t" + str(minchrom[i][3]) + "\n"
		else:
			outstr3 += str(1.5) + "\t" + str(0.5) + "\n"
		f1.write(outstr1)
		f2.write(outstr2)
		f3.write(outstr3)
	f1.close()
	f2.close()
	f3.close()
  
#-------------------------------------------------------------------------------
# store the population
#-------------------------------------------------------------------------------
def store_population(pop, generation):
		
	filename = "results/population" + str(generation) + ".txt"
	f1 = open(filename, 'w')
	popsize = len(pop)/2
	popsize = int(popsize)

	for i in range(popsize):
		outstr1 = ''
		outstr1 += repr(pop[2*i][0])
		outstr1 += "\t"
		outstr1 += repr(pop[2*i][1])
		outstr1 += "\t"
		outstr1 += repr(pop[2*i][2])
		outstr1 += "\t" 
		outstr1 += repr(pop[2*i][3])
		outstr1 += "\t"
		outstr1 += repr(pop[2*i + 1])
		outstr1 += "\n"
		f1.write(outstr1)

	f1.close()
  
#-------------------------------------------------------------------------------
# create population with uniformly distributed citizens
# tested
#-------------------------------------------------------------------------------
def create_population(n, parmranges):

	n = flooreven(n)
	pop = range(2*n)						# the population	

	l = int( float(n)**(1.0/float(nparms)) )			# number of linearly spaced individuals along one side
	l = int(l)
	rngs = range(nparms)
	deltas = range(nparms)
	for i in range(nparms):
		rngs[i] = parmranges[i][1] - parmranges[i][0]	
		deltas[i] = rngs[i]/float(l + 1) 

	for i in range(l**nparms):
		coords = range(nparms)
		for k in range(nparms):	
			coord = ( i % l**(k+1) ) / (l**k)
			coords[k] = int(coord)
		#assign
		pop[2*i] = []
		for k in range(nparms):
			pop[2*i].append(float(coords[k]+1)*deltas[k])
		pop[2*i] = repair(pop[2*i], parmranges)
		fitness = 0
		pop[2*i+1] = fitness
		
	# distribute the rest of the individuals randomly
	for i in range(n-l**nparms):
		fitness = 0
		idx = l**nparms + i	
		pop[2*idx] = create_random_chromosome(parmranges)
		pop[2*idx] = repair(pop[2*idx], parmranges)
		pop[2*idx+1] = fitness	
	
	return pop

#-------------------------------------------------------------------------------
# create population with n random citizens
# tested
#-------------------------------------------------------------------------------
def create_population_random(n, parmranges):

	n = flooreven(n)
	pop = range(2*n)						# the population	
	
	# distribute the rest of the individuals randomly
	for i in range(n):
		fitness = 0
		idx = i	
		pop[2*idx] = create_random_chromosome(parmranges)
		#pop[2*idx] = repair(pop[2*idx], parmranges)
		pop[2*idx+1] = fitness	
	
	print "result:"
	print pop
	return pop

#-------------------------------------------------------------------------------
# create a chromosome with random parameters
# tested
#-------------------------------------------------------------------------------
def create_random_chromosome(parmranges):

	parms = []	

	for rng in parmranges:
		lo = rng[0]
		hi = rng[1]	
		parm = lo + myunirand()*(hi-lo)
		parms.append(parm)
		
	return parms

#-------------------------------------------------------------------------------
# pretty print for population
# tested
#-------------------------------------------------------------------------------
def print_population(pop):
      
	l = int (len(pop)/2)
	for i in range(l):
		outstr = ''
		outstr += str(i)
		outstr += ": "
		outstr += str(pop[i*2])
		outstr += " ("
		outstr += str(pop[i*2 + 1]);
		outstr += ")"	
		print >>logfile2, outstr

#-------------------------------------------------------------------------------
# only print the best citizen
# precondition: population is sorted
# tested
#-------------------------------------------------------------------------------
def print_best(pop):
		  
	outstr = "best: "
	outstr += str(pop[0])
	outstr += " ("
	outstr += str(pop[1]);
	outstr += ")"	
	print >>logfile2, outstr

#-------------------------------------------------------------------------------
# print the relative error of a given chromosome
#-------------------------------------------------------------------------------
def relative_error(chrom):
	ref =  get_reference()
	res = get_lensing(refparms[0], refparms[1], chrom[0], chrom[1])
	
	if len(ref) != len(res):
		print "image has not same number of lensing points as reference"
		return
	
	print ref
	print res

	print "spatial:"
	for i in range(len(ref)):
		print "point ", i
		x_ref = ref[i][0]
		y_ref = ref[i][1]
		x_res = res[i][0]
		y_res = res[i][1]
		num = sqrt ( (x_ref - x_res)**2 + (y_ref - y_res)**2 )
		den = sqrt ( x_ref**2 + y_ref**2 )
		rel_err = num/den
		print rel_err

	print "temporal:"		
	for i in range(len(ref)):
		print "point ", i
		t_ref = ref[i][3]
		t_res = res[i][3]
		if(t_ref == 0):
			continue
		num = sqrt( (t_ref - t_res)**2 )
		den = sqrt( t_ref**2 )	
		rel_err = num/den
		print rel_err
		
#-------------------------------------------------------------------------------
# sort the citizens by their fitness
# with builtin sorting
# tested
#-------------------------------------------------------------------------------
def sort_population(pop):

	# create temporary list
	templist = []						# we have to reshuffle the list to be in correct format
	l = int (len(pop)/2)
	for i in range(l):
		elm = [pop[i*2], pop[i*2+1]]
		templist.append(elm)

	# sort temporary list
	templist = sorted(templist, key=lambda cit: cit[1]) 
	
	# bring back to original form
	l = int (len(pop)/2)
	for i in range(l):
		pop[i*2] = (templist[i])[0]
		pop[i*2 + 1] = (templist[i])[1] 


#-------------------------------------------------------------------------------
# return a vector of the positions of all the twins of chrom
#-------------------------------------------------------------------------------
def find_twins(pop, chrom):
	
	popsize = len(pop)/2
	popsize = int(popsize)
	res = []
	first = False

	for i in range(popsize):
		if pop[i*2] == chrom:
			if first == False:			# first occurence is the original chromosome
				first = True
			else:
				res.append(i)

	return res

#-------------------------------------------------------------------------------
# mutate the population
# with elitism
# precondition: first nelite citizens must be the nelite best ones
#-------------------------------------------------------------------------------
def mutate(pop, parmranges, mrate):
	
	psize = len(pop)/2
	psize = int(psize)	
	chromsize = len(parmranges)	# length of one chromosome
	nelite = 1
	nmut = ceil(float(psize-nelite)*float(chromsize)*mrate)	
	nmut = int(nmut)

	# generate the changemask
	changemask = []
	for i in range(psize):
		changemask.append(False)

	# generate random numbers
	mcits = []
	mgenes = []
	for i in range(nmut):
		val = ceil(myunirand()*(psize-nelite))
		mcits.append(val)
	for i in  range(nmut):
		val = ceil(myunirand()*chromsize) - 1
		mgenes.append(val)
	mcits.sort()
	
	# do the mutation
	for i in range(nmut):
		mcit = mcits[i]	# which citizen
		mgene = mgenes[i] 	# which gene
		mcit = int(mcit)
		mgene = int(mgene)	
		lo = parmranges[mgene][0]
		hi = parmranges[mgene][1]		
		pop[mcit*2][mgene] = lo + myunirand()*(hi-lo) 
		pop[mcit*2] = repair(pop[mcit*2], parmranges)
		changemask[mcit] = True
	
	return changemask

#-------------------------------------------------------------------------------
# pairing and mating of the citizens
# precondition: pop must be sorted
# pop is modified
#-------------------------------------------------------------------------------
def mate(pop, xrate, parmranges):

	psize = len(pop)/2	
	psize = int(psize)
	keep = floor(xrate*psize)					# number of individuals that are kept
									# and can become parents
	keep = int(keep)
	keep = flooreven(keep)
	nmatings = (psize - keep)/2	
	nmatings = int(nmatings)
	nparms = len(parmranges)

	# generate the changemask
	changemask = []
	for i in range(keep):
		changemask.append(False)
	for i in range(keep, psize):
		changemask.append(True)

	# generate cumulative sum of kept part of population
	csum = [0]				# cumulative sum in table on page 39
	ssum = 0				# sum in denominator in eqn. (2.11)
	for i in range(keep):
		ssum += (i+1)
	for i in range(keep-1):
		P = float(keep - i)/float(ssum)
		csum.append(csum[i] + P)
	csum.append(1.0)			# last entry is not exactly one 
						# if actually computed
	
	# generate random values
	picks1 = []
	picks2 = []
	xps = []
	betas = []
	for i in range(nmatings):
		picks1.append( myunirand() )
	for i in range(nmatings):
		picks2.append( myunirand() )
	for i in range(nmatings):
		xps.append( myunirand() )
	for i in range(nmatings):
		betas.append( myunirand() )

	# mating with rank weighted pairing and single point crossover
	for i in range(nmatings):

		# find mom and dad
		pick1 = picks1[i]		# pick for mom
		pick2 = picks2[i]		# pick for dad
		mom = 0
		dad = 0
		for j in range(1,keep+1):
			if pick1 <= csum[j] and pick1 > csum[j-1]:
				mom = j-1
			if pick2 <= csum[j] and pick2 > csum[j-1]:
				dad = j-1

		# generate offspring as copies of mom and dad
		idx1 = keep + 2*i
		idx2 = keep + 2*i + 1
		pop[2*idx1] = copy(pop[2*mom])	# chromosome of offspring1
		pop[2*idx1+1] = 0			# fitness of offspring1
		pop[2*idx2] = copy(pop[2*dad])	# chromosome of offspring1
		pop[2*idx2+1] = 0			# fitness of offspring1	
	
		# single point crossover
		xp = ceil(xps[i]*nparms)-1
		xp = int(xp)
		beta = betas[i]				
		pop[2*idx1][xp] = pop[2*mom][xp] - beta*(pop[2*mom][xp] - pop[2*dad][xp])
 		pop[2*idx2][xp] = pop[2*dad][xp] + beta*(pop[2*mom][xp] - pop[2*dad][xp])
		
		# swap all the genes to the right of the crossover point
		# between the offspring
		for j in range(xp+1, nparms):
			temp = pop[2*idx2][j]
			pop[2*idx2][j] = pop[2*idx1][j]
			pop[2*idx1][j] = temp 

		# repair the chromosomes of the offspring
		pop[2*idx1] = repair(pop[2*idx1], parmranges)
		pop[2*idx2] = repair(pop[2*idx2], parmranges)

	# randomize all the twins
	for i in range(psize):
		chrom = pop[2*i]
		twinpos = find_twins(pop, chrom)
		for p in twinpos:
			twinchrom = pop[2*p]
			twinchrom = copy( create_random_chromosome(parmranges) )
			pop[2*p] = copy(twinchrom)
			changemask[p] = True
	
	return changemask

#-------------------------------------------------------------------------------
# check for physically invalid parameters
# note: deprecated
#-------------------------------------------------------------------------------
def repair(chrom, parmranges):

	return chrom 

#-------------------------------------------------------------------------------
# plot the image of the reference into the pdf file
#-------------------------------------------------------------------------------
def plot_reference():
	return lensplot(refparms[0], refparms[1], refparms[2], refparms[3], screenplot = False, pdfplot = True)

#-------------------------------------------------------------------------------
# genetic algorithm
#-------------------------------------------------------------------------------
def ga(psize, mrate, xrate, maxevals, ranges, threshold, tofile = False):

	optfit = -9999999
	
	# initialize this instance of the script
	#seed(42)
	psize = flooreven(psize)
	global function_calls
	global best_fitness
	global logfile
	global minc, meanc, callsc, minchrom
	function_calls = 0
	best_fitness = 1000000.0
	generation = 0
	logfile = open('log_ga.txt', 'a')
	minc = []
	meanc = []
	callsc = []
	minchrom = []
	print >>logfile2, "starting new GA:", "psize=", psize, " mrate=", mrate, " xrate=", xrate,
	print >>logfile2, " maxevals=", maxevals, " ranges=", ranges,
	print >>logfile2, " threshold=", threshold  
	 
	# initialize
	# note: moved that to the master script
		
	# create initial population
	pop = create_population(psize, ranges)	
	pop = calc_fitness_population(pop)
	sort_population(pop)
	#print_population(pop)
	#store_population(pop, 0)

	# perform genetic algorithm 
	while best_fitness > threshold and function_calls < maxevals:			
		
		print >>logfile2, "starting generation ", generation
		print "starting generation ", generation 
					
		# mate
		changemask_mating = mate(pop, xrate, ranges)
		
		# mutate
		changemask_mutation = mutate(pop, ranges, mrate)
	
		# compute the combined changemask
		changemask = range(len(changemask_mating))
		for j in range(psize):
			changemask[j] = changemask_mating[j] or changemask_mutation[j]
		
		# evaluate the fitnesses and sort
		pop = calc_fitness_population(pop, changemask)		
		sort_population(pop)

		# update the number of function calls and the best fitness
		if pop[1] < best_fitness:
			best_fitness = pop[1]
		generation += 1
		if(generation % 10 == 0):		
			print "generation: ", generation, " __ ",
			print function_calls, " evaluations  __",
			print best_fitness, " is best fitness", " | " 
		#store_population(pop, generation)

		# store the statistics
		statistics(pop)
		
		# store the population to the log
		#print_population(pop)

	# print the statistics
	if tofile:
		store_statistics()

	print_population(pop)
	
	# cleanup
	# note: moved that to the master script
	
	# return
	best = [pop[0], pop[1]]
	return callsc, minc, meanc

#-------------------------------------------------------------------------------
# DRIVER
#-------------------------------------------------------------------------------
