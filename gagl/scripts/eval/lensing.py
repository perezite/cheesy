# -*- coding: utf-8 -*-
# this script has to be called by ga.py and does not compute
# on its own
# it performs the fitness by creating a reference lensing image
# and comparing it with a sample lensing image from the given
# genome

import sys
sys.path.append('/home/kurs/simons/GAGL/programme/lensing_restricted2')
glass_basis('basis.pixels')

setup_log(stdout=False)						# disable output from glass
from environment import env
from misc.raytrace import raytrace, write_code, observables
from environment import Image
#from pytipsy import load_tipsy
from numpy import load
from math import cos,sin
from pylab import clf, title
from math import *
from random import *
import pickle
reference = []			# the reference data (from simulation or observation)	
reference_exists = False	# will be set to true, as soon as reference is filled with data

#-------------------------------------------------------------------------------
# The lensing procedure
#-------------------------------------------------------------------------------
def lensing(sx, sy, zs, zl):

	#-------------------------------------------------------------------------------
	# Now load the simulation.
	#-------------------------------------------------------------------------------
	#file='ddehnensphere_0.std'
	#t = load_tipsy(file, merge=['r', 'm'], memmap=False)
	#X,Y,Z = t.p.r.T
	#M = t.p.m

	file = 'bigsim.txt.npz'
	data = load(file)
	X = data['arr_0']
	Y = data['arr_1']
	Z = data['arr_2']
	M = data['arr_3']

	#-------------------------------------------------------------------------------
	# For this simulation we only consider the stars.
	#-------------------------------------------------------------------------------
	X = X[35471:35471+554370]
	Y = Y[35471:35471+554370]
	Z = Z[35471:35471+554370]
	M = M[35471:35471+554370] 

	M *= 1e10 # Only for bigsim (converts to Msun)

	#print '%i particles' % len(M)
	#print 'Total mass is %.4e [Msun]' % sum(M)

	#result=[]
	#-------------------------------------------------------------------------------
	# Create the lensing object.
	#-------------------------------------------------------------------------------
	clear()  # Only necessary when before regenerating models
	g = 13.7
	omega(0.3,0.7)
	hubble_time(g)

	obj = globject('my lens')
	zlens(zl)
	pixrad(10)
	maprad(1.0)

	#-------------------------------------------------------------------------------
	# And the source.
	#-------------------------------------------------------------------------------
	src = source(zs)

	#-------------------------------------------------------------------------------
	# Now generate a single model using the particles and a source postion.
	#-------------------------------------------------------------------------------
	#r = 0.010
	#t = 240
	#sx = r * cos(t * 3.1415/180.)
	#sy = r * sin(t * 3.1415/180.)

	#sx,sy = .0160, 0.0047  

	model(1, mode='particles',
		data=[X,Y,M, [[sx,sy]], g])

	#-------------------------------------------------------------------------------
	# Raytrace model 0, using object 0 and source 0.
	#-------------------------------------------------------------------------------
	d = raytrace([env().models[0], 0,0])
	#print observables(env().models[0], 0,0, d)
	#write_code(env().models[0], 0,0, d)

	#-------------------------------------------------------------------------------
	# The plots assume that the source already knew where the images were. We
	# add them manually here. This isn't necessary if you want to just run many
	# different models.
	#-------------------------------------------------------------------------------
	for img,t,_,parity in d:
	    src.add_image(Image((img.real, img.imag), parity))
			
	output = observables(env().models[0], 0,0, d)
	return output

#-------------------------------------------------------------------------------
# calculate fitness of one genome 
# tested
# remark: modified to be two dimensional
#-------------------------------------------------------------------------------
def calc_fitness(genome):

	sx = 0.016
	sy = 0.0047
	zs = genome[0]
	zl = genome[1]
	
	# return big fitness value if parameters are
	# physically invalid
	if zl >= zs:
		return 100.0

	# for the lensing, the stdout has to be redirected because glass
	# calls print at some points
	old = sys.stdout			
	new = sys.stderr
	sys.stdout = new

	# create the reference data if there is none yet
	if not reference_exists:
		#sys.stderr.write("creating reference\n") 
		create_reference()
	      	      
	# generate the image
	ret = lensing(sx, sy, zs, zl)
	res = extract(ret)			# the phenotype

	#print >>sys.stderr, "ref: ", reference
	#print >>sys.stderr, "res: ", res
	

	# redirect stdout back to where it was at beginning of the function call
	sys.stdout = old

	# find distance between points of phenotype and the reference
	distpen = 0.0	
	for i in range(len(reference)):
		if not(i in range(len(res))):		# break if number of ref-points exceeds number of current points
			break
		d = (reference[i][0]-res[i][0])*(reference[i][0]-res[i][0]) + (reference[i][1]-res[i][1])*(reference[i][1]-res[i][1]) 
		d = sqrt(d)
		#print str(reference[i][0]), " ", str(reference[i][1]), " ", str(res[i][0]), " ", str(res[i][1]), " ", d  
		distpen += d

	# find temporal distance between points of phenotype and the reference
	timepen = 0.0	
	for i in range(len(reference)):
		if not(i in range(len(res))):		# break if number of ref-points exceeds number of current points
			break
		d = (reference[i][3]-res[i][3])*(reference[i][3]-res[i][3])
		d = sqrt(d)
		timepen += d

	# penalty for every two compared points with different point type
	typepen = 0.0	
	for i in range(len(reference)):	
		if not(i in range(len(res))):
			break
		t1 = reference[i][2]
		t2 = res[i][2]
		if(not(t1==t2)):
			typepen += 0.5

	# from itertools import izip
	#for r1,r2 in izip(reference,res):
	#	t1 = r1[2]
	#	t2 = r2[2]
	#	if t1 != t2:
	#		typepen += 0.5
		
	# penalty for missing/extra points in current phenotype
	num = abs(len(reference)-len(res))
	numpen = 1.0*num 
		
	# add up the penalties	
	fitness = numpen + typepen + distpen + timepen
	return fitness

#-------------------------------------------------------------------------------
# fill the reference variable with data to which the genetic algorithm should converge
#-------------------------------------------------------------------------------
def create_reference():
	global reference
	global reference_exists
	reference = lensing(.0160, 0.0047, 1.5, 0.5)
	reference = extract(reference)
	reference_exists = True
	return reference

#-------------------------------------------------------------------------------
# convert data obtained from lensing() into an appropriate format
#-------------------------------------------------------------------------------
def extract(data):
	res = []

	# maybe there is no data at all
	if data is None:
		return res

	#first elm contains no delay, so extract it separately
	x = data[0][0].real
	y = data[0][0].imag
	par = data[0][1]
	delay = 0.0
	res.append([x,y,par,delay])

	# extract rest of the image list
	for i in range(1, len(data)): 
		x = data[i][0].real
		y = data[i][0].imag
		par = data[i][1]
		delay = data[i][2]
		res.append([x,y,par,delay])

	return res
	
#-------------------------------------------------------------------------------
# read the program arguments
# precondition: reference() must have already been called
#-------------------------------------------------------------------------------
def read_program_args():
	
	if len(sys.argv) < 2:
		print "not enough parameters"
		return
	
	filename = sys.argv[1]
	procid = sys.argv[2]
	args = [filename, procid]
	return args

#-------------------------------------------------------------------------------
# parse the data received by the master script
#-------------------------------------------------------------------------------
def parse_input(inputstring):

	#print >>sys.stderr, "parse: ", inputstring

	data = inputstring.split(";")
	
	parms = []
	for elm in data:
		#sys.stderr.write(elm + "\n")
		#sys.stderr.write(str(program_args[1]) + " got: " + elm)
		strnums = elm.split(',')
		nums = []
		for strnum in strnums:
			nums.append(float(strnum))
		parms.append(nums)
	return parms

#-------------------------------------------------------------------------------
# parse the data passed back to the master script
#-------------------------------------------------------------------------------
def parse_output(fitnesses):
	
	output = []
	for elm in fitnesses:
		s = repr(elm)
		output.append(s)
	output = ';'.join(output)
	#sys.stderr.write(str(output) + "\n")
	return output


#-------------------------------------------------------------------------------
# driver
#-------------------------------------------------------------------------------

# read program args
program_args = read_program_args()

while True:
	# read genomes from master script
	inputstring = pickle.load(sys.stdin)

	# check for termination signal from master
	if inputstring == "signal_terminate":
		#sys.stderr.write("got a termination signal")
		sys.exit(0)

	parms = inputstring

	# calculate fitness of genomes 
	fitnesses = []
	for genome in parms:
		fitness = calc_fitness(genome)
		fitnesses.append(fitness)

	output = parse_output(fitnesses)

	# send results back to master script 
	pickle.dump(fitnesses, sys.stdout)
	sys.stdout.flush()

