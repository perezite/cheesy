# evaluation of lensing

# set up environment
import sys
import os
glass_basis('basis.pixels')

setup_log(stdout=False)						# disable output from glass
from environment import env
from misc.raytrace import raytrace, write_code, observables
from environment import Image
#from pytipsy import load_tipsy
from numpy import load
from math import cos,sin
from pylab import clf, title
from matplotlib.backends.backend_pdf import PdfPages
from math import *
from random import *
pp = PdfPages('output.pdf')
refparms = [0.016, 0.0047, 6.0, 1.0]

#-------------------------------------------------------------------------------
# The lensing procedure
#-------------------------------------------------------------------------------
def lensing(sx, sy, zs, zl, screenplot = False, pdfplot = False):

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
	
	#-------------------------------------------------------------------------------
	# Now plot the mass distribution and image locations. The mass has been 
	# converted into units of the critical density Kappa_crit. A value >= 1 
	# causes lensing.
	#-------------------------------------------------------------------------------	
	if screenplot==True or pdfplot==True:	
		kappa_plot(env().models[0], 0)
		img_plot(env().models[0])
		mytitle = ''			# title for the = list of lensing-parms.
		mytitle += "sx="
		mytitle += str(sx)
		mytitle += " sy="
		mytitle += str(sy)
		mytitle += " zs="
		mytitle += str(zs)
		mytitle += " zl="
		mytitle += str(zl)
		title(mytitle)
		if pdfplot == True:
			pp.savefig()
		if screenplot == True:
			show_plots()
		clf()
		
	output = observables(env().models[0], 0,0, d)
	return output

#-------------------------------------------------------------------------------
# calculate fitness of one chromosome 
# tested
#-------------------------------------------------------------------------------
def lensing_calc_fitness_restricted(chrom, numfit=True, typefit=True, distfit=True, timefit=True):

	sx = chrom[0]
	sy = chrom[1]
	zs = chrom[2]
	zl = chrom[3]
	#zs = 1.5
	#zl = 0.5

	# return big fitness value if parameters are
	# physically invalid
	if zl >= zs:
		return 1.0

	# generate the reference
	reference = get_reference()
	print reference
	print len(reference)
	      	      
	# generate the image
	ret = lensing(sx, sy, zs, zl, True, True)
	res = extract(ret)			# the phenotype

	# find spatial distance between points of phenotype and the reference
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
		#print str(reference[i][0]), " ", str(reference[i][1]), " ", str(res[i][0]), " ", str(res[i][1]), " ", d  
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
	
	# penalty for missing/extra points in current phenotype
	num = abs(len(reference)-len(res))
	numpen = 1.0*num 

	# add up the penalties	
	fitness = 0
	if(numfit):
		fitness += numpen
	if(typefit):
		fitness += typepen
	if(distfit):
		fitness += distpen
	if(timefit):
		fitness += timepen
	#fitness = numpen + typepen + distpen + timepen
	pens = [numpen, typepen, distpen, timepen]
	return pens, fitness

#-------------------------------------------------------------------------------
# returns the reference image to which the genetic algorithm should converge
#-------------------------------------------------------------------------------
def get_reference():
	data = lensing(refparms[0], refparms[1], refparms[2], refparms[3], False, True)
	res = extract(data)
	return res

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
# return the lensing data for the given parameters
#-------------------------------------------------------------------------------
def get_lensing(sx, sy, zs, zl):
	data = lensing(sx, sy, zs, zl, False, False)
	res = extract(data)
	return res

#-------------------------------------------------------------------------------
# print the relative error of a given chromosome
#-------------------------------------------------------------------------------
def relative_error(chrom):
	ref =  get_reference()
	#res = get_lensing(.0160, 0.0047, chrom[0], chrom[1])
	res = get_lensing(chrom[0], chrom[1], chrom[2], chrom[3])
	
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
# pretty print for a phenotype
#-------------------------------------------------------------------------------
def print_phenotype(image):
	for pixel in image:
		print pixel

#-------------------------------------------------------------------------------
# read the program arguments
# precondition: reference() must have already been called
#-------------------------------------------------------------------------------
def read_program_args():
	
	if len(sys.argv) < 10:
		print "not enough parameters"
		return
	
	args = [float(sys.argv[2]), float(sys.argv[3]), float(sys.argv[4]), float(sys.argv[5])]
	fitparms = [bool(float(sys.argv[6])), bool(float(sys.argv[7])), bool(float(sys.argv[8])), bool(float(sys.argv[9]))]
	return args, fitparms

#-------------------------------------------------------------------------------
# DRIVER
#-------------------------------------------------------------------------------



args, fitparms = read_program_args()
chrom = [args[0], args[1], args[2], args[3]]
pens, fit = lensing_calc_fitness_restricted(chrom, fitparms[0], fitparms[1], fitparms[2], fitparms[3]) 
print "relative errors:"
relerr = relative_error(chrom)
print "num fitness: ", repr(pens[0])
print "type fitness: ",  repr(pens[1])
print "dist fitness: ",  repr(pens[2])
print "time fitness: ",  repr(pens[3])
print "sum fitness: ", repr(fit)
print "phenotype: "
res = extract(lensing(chrom[0], chrom[1], chrom[2], chrom[3]))
print_phenotype(res)
print "reference: "
res = extract(lensing(refparms[0], refparms[1], refparms[2], refparms[3]))
print_phenotype(res)

#chrom = [.0160, 0.0047, 1.51, 0.51]
#fit = calc_fitness(chrom)
#print calc_fitness(chrom)
#relative_error([chrom[2], chrom[3]])

# cleanup
pp.close()
