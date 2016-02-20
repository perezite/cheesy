'''

@author: Simon Schwegler
'''

from scanner import *

"""
# test for scanner.get_durations()
"""
def get_durations_test():
    
    print "test with simple string:";
    print "------------------------";
    html_string = '42\'</div> 3 h 43\'</div>';
    timedata = get_durations(html_string);
    print timedata
    
    print ""
    print "test with full website (Uster-Zuerich):";
    print "------------------------";    
    response = urllib2.urlopen('http://fahrplan.search.ch/uster/zuerich-hb?time=14.00&date=Mo+05.01.2013')
    html_string = response.read();
    timedata = get_durations(html_string);
    print timedata

    print ""
    print "test with full website (Grosswangen-Zuerich):";
    print "------------------------";        
    response = urllib2.urlopen('http://fahrplan.search.ch/grosswangen,post/genf?time=18.15&date=Sa+10.01.2013');
    html_string = response.read();
    timedata = get_durations(html_string);
    print timedata

    print ""
    print "test with full website (Aesch,ZH-Zuerich HB):";
    print "------------------------";        
    response = urllib2.urlopen('http://fahrplan.search.ch/Aesch,ZH/Zuerich?time=09.00&date=Mo+05.01.2013&mode=arrival');
    html_string = response.read();
    timedata = get_durations(html_string);
    print timedata

"""
# test for scanner.string2minutes()
"""
def string2minutes_test():
    test1 = "3 h 43'"
    test2 = "42'"
    ret = string2minutes(test1)
    print "total minutes " + str(ret)    


"""
# test for scanner.travel_time()
"""
def travel_time_test():
    print "test from Grosswangen to Zuerich";
    print "--------------------------------";
    ret = travel_time('Grosswangen', 'Genf', '18.15', 'Sa', '10.01.2013');
    print ret;


"""
# test for scanner.travel_times()
"""
def travel_times_test():
    print "test for the following list of routes"
    routes = list()
    routes.append(['Grosswangen', 'Genf', '18.15', 'Sa', '10.01.2013'])
    routes.append(['Zuerich', 'Genf', '14.00', 'Sa', '10.01.2013'])
    print routes
    print "--------------------------------";    
    times = travel_times(routes)
    print times

 
"""
# test for scanner.sort_towns()
"""   
def sort_towns_test():
    towns = ['Grosswangen', 'Zuerich']
    endloc = 'Genf'
    time = '14.00'
    weekday = 'Sa'
    date = '10.01.2013'
    print "test for the following list of towns"
    print towns
    print "--------------------------------"; 
    sorted_towns = sort_towns(towns, endloc, time, weekday, date)
    print sorted_towns    
 
"""
# test for writing a list to a file
"""   
def write_towns_test():
    mylist = [['Uitikon', 21], ['Grosswangen', 98], ['Genf', 162]]
    f = open('testfile.txt', 'w')
    for item in mylist:
        f.write(item[0] +  " " + repr(item[1]) + "\n")
        

  
"""
# DRIVER
"""
get_durations_test()
#travel_time_test()
#travel_times_test()
#sort_towns_test()
#write_towns_test()