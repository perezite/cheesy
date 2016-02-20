from matplotlib import rc

#-------------------------------------------------------------------------------
# Tell glass we want to work with pixels.
#-------------------------------------------------------------------------------
glass_basis('basis.pixels')

#-------------------------------------------------------------------------------
# Load routines for tipsy files and raytracing.
#-------------------------------------------------------------------------------
from misc.raytrace import raytrace, write_code, observables
from environment import Image
#from pytipsy import load_tipsy
from numpy import load
from math import cos,sin

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


#-------------------------------------------------------------------------------
# Create the lensing object.
#-------------------------------------------------------------------------------
# clear()  # Only necessary when before regenerating models
g = 13.7
omega(0.3,0.7)
hubble_time(g)

obj = globject('my lens')
zlens(0.5)
pixrad(35)
maprad(1.0)

#-------------------------------------------------------------------------------
# And the source.
#-------------------------------------------------------------------------------
src = source(1.5)

#-------------------------------------------------------------------------------
# Now generate a single model using the particles and a source postion.
#-------------------------------------------------------------------------------
r = 0.010
t = 240
sx = r * cos(t * 3.1415/180.)
sy = r * sin(t * 3.1415/180.)

sx,sy = .0160, 0.0047  

model(1, mode='particles',
         data=[X,Y,M, [[sx,sy]], g])

#-------------------------------------------------------------------------------
# Raytrace model 0, using object 0 and source 0.
#-------------------------------------------------------------------------------
d = raytrace([env().models[0], 0,0])
print observables(env().models[0], 0,0, d)
write_code(env().models[0], 0,0, d)

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
kappa_plot(env().models[0], 0)
img_plot(env().models[0])

rc('text', usetex=False)
show_plots()

