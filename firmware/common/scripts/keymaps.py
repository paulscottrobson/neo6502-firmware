# ***************************************************************************************
# ***************************************************************************************
#
#      Name :      keymaps.py
#      Authors :   Paul Robson (paul@robsons.org.uk)
#      Date :      29th December 2023
#      Reviewed :  No
#      Purpose :   Prompt creator (counts builds etc.)
#
# ***************************************************************************************
# ***************************************************************************************

import sys,os,re

keys = """
#
#		GB (e.g. United Kingdom) keyboard variations.
#
	[GB]

"""

src = [x for x in keys.replace("\t"," ").split("\n") if not x.startswith("#")]
src = " ".join(src)
code = []
current = []
code += [0]
print(str(code)[1:-1])

