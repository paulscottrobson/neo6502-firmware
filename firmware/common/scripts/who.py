# ***************************************************************************************
# ***************************************************************************************
#
#      Name :      who.py
#      Authors :   Paul Robson (paul@robsons.org.uk)
#      Date :      31st December 2023
#      Reviewed :  No
#      Purpose :   Who list.
#
# ***************************************************************************************
# ***************************************************************************************

import sys,os,re

whoList = """
	Paul Robson
	Sascha Schneider
	Rien Matthijse
	Tsvetan Usunov
	Oliver Schmidt
	Veselin Sladkov
	David Given
	Bocianu Boczansky 
	Bill Auger

""".strip().split("\n")
whoList = [x.strip() for x in whoList]
whoList.sort(key = lambda x:x.split()[1])
print('static const char *whoList = "\\nThanks to : {0}\\n\\n";'.format(",".join(whoList)))
