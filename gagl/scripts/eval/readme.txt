eval.py can be used to evaluate a lensing image 
and show the corresponding fitness and relative error

Command:
	python ../../bin/lib/glass/glass.py eval.py x_s y_s z_s z_l  numfit typefit distfit timefit

	numfit, typefit, distfit, timefit are booleans which tell, if the 
	number of points, type of points, distanc between points and time between
	points should be taken into account in the fitness function calculation

Example:
	python ../../bin/lib/glass/glass.py eval.py 0.032 0.018 6.0 1.0 1 1 1 1

