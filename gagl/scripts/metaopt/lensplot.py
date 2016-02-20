# -*- coding: utf-8 -*-

import sys

#-------------------------------------------------------------------------------
# Load routines for tipsy files and raytracing.
#-------------------------------------------------------------------------------
#glass_basis('basis.pixels')					# tell glass we want to work with pixels
setup_log(stdout=False)						# disable output from glass
from environment import env
from misc.raytrace import raytrace, write_code, observables
from environment import Image
from numpy import load
from math import cos,sin
from pylab import clf, title
from matplotlib.backends.backend_pdf import PdfPages
from math import *
from random import *
pp = PdfPages('output.pdf')
saveplots = True

reference = []			# the reference data (from simulation or observation)
refparms = [0.016, 0.0047, 6.0, 1.0]

#-------------------------------------------------------------------------------
# The lensing procedure
#-------------------------------------------------------------------------------
def lensplot(sx, sy, zs, zl, screenplot = False, pdfplot = False):

	# skip if invalid
	if zl >= zs:
		sys.stderr.write("skipping invalid configuration in lensplot")
		return

	#-------------------------------------------------------------------------------
	# Now load the simulation.
	#-------------------------------------------------------------------------------
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
	#print sx, " ", sy, " ", zs, " ", zl
	src = source(zs)

	#-------------------------------------------------------------------------------
	# Now generate a single model using the particles and a source postion.
	#-------------------------------------------------------------------------------
	model(1, mode='particles',
		data=[X,Y,M, [[sx,sy]], g])

	#-------------------------------------------------------------------------------
	# Raytrace model 0, using object 0 and source 0.
	#-------------------------------------------------------------------------------
	d = raytrace([env().models[0], 0,0])

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
# returns the reference image to which the genetic algorithm should converge
#-------------------------------------------------------------------------------
def get_reference():
	data = lensplot(refparms[0], refparms[1], refparms[2], refparms[3], False)
	res = extract(data)
	return res

#-------------------------------------------------------------------------------
# return the lensing data for the given parameters
#-------------------------------------------------------------------------------
def get_lensing(sx, sy, zs, zl):
	data = lensplot(sx, sy, zs, zl, False)
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
# plot the reference image
#-------------------------------------------------------------------------------
def plot_reference():
	lensplot(refparms[0], refparms[1], refparms[2], refparms[3], False, True)

#-------------------------------------------------------------------------------
# do cleanup for this script
#-------------------------------------------------------------------------------
def lensplot_cleanup():
	pp.close()

#-------------------------------------------------------------------------------
# driver
# (for testing)
#-------------------------------------------------------------------------------



