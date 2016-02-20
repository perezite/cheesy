'''

@author: Simon Schwegler
'''

import re	# regular expressions
import urllib2


"""
# convert a string of the form x h y' (for example 3 h 32') 
# to minutes only
"""
def string2minutes(s):

	idx1 = s.find('h');
	idx2 = s.find("'");
	
	if idx1 != -1:
		hours = s[0:idx1]
	else: 
		hours = '0'
	minutes = s[idx1 + 1:idx2]
	
	#print "hours " + hours
	#print "minutes " + minutes
	
	minutes = int(hours)*60 + int(minutes)
	return minutes


"""
# extract all the duration data from the website text data
# and return them in minutes
"""
def get_durations(html_string):
	
	# look up all the matches for the regex
	p = re.compile('(\d*\s*h\s*)?(\d*\s*\'</div>)')
	m = p.findall(html_string)
		
	# due to the () () in the regex, the matches are returned
	# as [[chunk1, chunk2], [chunk1, chunk2], ...], so patch
	# these chunks together 
	matches = []
	for mymatch in m:
		fullmatch = ''
		for chunk in mymatch:
			fullmatch += chunk
		fullmatch = fullmatch[0:len(fullmatch) - 6]; # remove </div>
		matches.append(fullmatch)
	
	# convert data to minutes
	
	return matches


"""
# compute the minimal travel time from startloc to endloc at a certain
# time and weekday&date
"""
def travel_time(startloc, endloc, time, weekday, date):
	
	# assemble the URL
	url = 'http://fahrplan.search.ch/'
	url = url + startloc + '/'
	url = url + endloc + '?'
	url = url + 'time=' + time + '&date=' + weekday + '+' + date
	url = url + '&mode=arrival'
	#print url;
	print "Scanning:", startloc, "to", endloc
	
	# get html data and extract times
	response = urllib2.urlopen(url)
	html_string = response.read()
	timedata = get_durations(html_string)
	#print timedata
	
	# convert to minutes
	minutes = list()
	for s in timedata:
		m = string2minutes(s);
		minutes.append(m)
	
	# find minimum
	if len(minutes) == 0:
		mintime = -1
	else:
		mintime = min(minutes)
	return mintime;


"""
# compute the minimal travel time for a list of destinations
# at a certain date.
# loclist is a list of lists, where each inner list specifies a 
# route of the following format:
# 	[startloc, endloc, time, weekday, date].
# the function returns the minimal traval time for each 
# of the routes
"""
def travel_times(routes):
	times = list()
	
	for route in routes:
		startloc = route[0]
		endloc = route[1]
		time = route[2]
		weekday = route[3]
		date = route[4]
		ttime = travel_time(startloc, endloc, time, weekday, date)  
		times.append(ttime)
		
	return times


"""
# sort the list towns in ascending order of travel time to the
# location endloc at a certain departure time and a certain
# date and weekday.
# Returns a sorted list of tuples, where the first tuple entry 
# is the town and the second entry the travel time in minutes
""" 
def sort_towns(towns, endloc, time, weekday, date):
	sorted_towns = list()
	
	for town in towns:
		ttime = travel_time(town, endloc, time, weekday, date)
		sorted_towns.append([town, ttime])
	
	sorted_towns.sort(key=lambda tup: tup[1]) 
	return sorted_towns


"""
# read list of towns from a file called filename
"""
def read_towns(filename):
	with open(filename) as f:
		lines = f.read().splitlines()
		
	return lines

"""
# write the sorted list of towns to a file filename
"""
def write_sorted_towns(sorted_towns, filename):
	f = open(filename, "w")
	for item in sorted_towns:
		f.write(item[0] +  " " + repr(item[1]) + "\n")
	f.close()

