p=2147483647;
c=16807;
rnd=42;

# return a random integer number in [low, hi]
def myrand(low, hi):
	global rnd,c,p
	rnd=(c*rnd)%p
	delta = hi-low
	res = low + ( rnd%(delta+1) )
	return res


# return a random float in (0,1)
def myunirand():
	global rnd,p
	rnd = (c*rnd)%p
	val = float(rnd) / float(p)
	return val

#seed the generator
def myseed(seed):
	global rnd
	rnd = seed

 
