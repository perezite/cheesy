'''

@author: Simon Schwegler
'''

"""
# snippet
"""
import urllib2
response = urllib2.urlopen('http://fahrplan.search.ch/uster/zuerich-hb?time=14.00&date=Mo+03.09.2012')
html = response.read()
print html
f = open('out.txt','w')
print >>f, html
