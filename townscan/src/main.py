'''

@author: Simon Schwegler
'''

import scanner
import sys

"""
# read program arguments from command line
"""
def readProgramArguments(): 
    argc = len(sys.argv)
    
    # set some standard values for program arguments
    inFile = "towns.txt"
    outFile = "output.txt"
    destination = "Bern"
    weekDay = "Mi"
    date = "01.01.2014"
    timeOfDay = "09.00"
    
    # stop if not even number of optional arguments
    if argc %2 == 0:
        showHelp()
    
    for i in range (1, argc, 2):
        variableArgument = sys.argv[i]
        valueArgument = sys.argv[i+1]
        
        if variableArgument == "--infile":
            inFile = valueArgument
        elif variableArgument == "--outfile":
            outFile = valueArgument
        elif variableArgument == "--destination":
            destination = valueArgument
        elif variableArgument == "--weekday":
            weekDay = valueArgument
        elif variableArgument == "--date":
            date = valueArgument
        elif variableArgument == "--timeofday":
            timeOfDay = valueArgument  
        else:
            showHelp()

    print "**********************************************************"
    print "Remark: Run townscan as 'townscan --help' to show command"
    print "        line options"
    print "**********************************************************"    
    return [inFile, outFile, destination, weekDay, date, timeOfDay]
            
"""
# show help and terminate
"""            
def showHelp():
    print "Usage: townscan [options]"
    print ""
    print "Options:"
    print "  --help                Show help and quit"
    print "  --infile              Input file with departure towns (default: towns.txt)" 
    print "  --outfile             Output file for results (default: output.txt)"
    print "  --destination         Destination (default: Bern)"
    print "  --weekday             Weekday for departure (default: Mi)"
    print "  --date                Date for departure (default: 01.01.2014)"
    print "  --timeofday           Time for departure (default: 09.00)"
    print ""
    print "Description: Townscan scans a list of towns and compares their respective"
    print "travel times to an other town by public transportation. The towns are" 
    print "sorted in ascending order of travel time and written to an output file "
    print "(in minutes)."
    print "Travel data is downloaded from fahrplan.search.ch"
    exit()

"""
# the main routine
"""
def main():    
    # read prgoram arguments from command line
    [inFile, outFile, destination, weekDay, date, timeOfDay] = readProgramArguments()
    
    # load townfile
    #towns = scanner.read_towns('../bin/towns.txt')
    towns = scanner.read_towns(inFile)
    # compute travel times
    #sorted_towns = scanner.sort_towns(towns, 'Schlieren', '09.00', 'Mo', '01.04.2014')
    sorted_towns = scanner.sort_towns(towns, destination, timeOfDay, weekDay, date)
    # show the result on screen
    print "Resulting list"
    print sorted_towns
    # write the result to a file
    #scanner.write_sorted_towns(sorted_towns, "../bin/output.txt")
    scanner.write_sorted_towns(sorted_towns, outFile)

"""
# DRIVER
"""
main()

