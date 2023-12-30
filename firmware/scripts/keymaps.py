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
		49:#	^49:~	^52:@	^31:" 	


	[XX]
"""

src = [x for x in keys.replace("\t"," ").split("\n") if not x.startswith("#")]
src = " ".join(src)
code = []
current = []

for e in [x for x in src.split() if x.strip() != ""]:
	if e.startswith("[") and e.endswith("]"):
		assert len(e) == 4
		if len(current) != 0:
			code = code + [len(current)+2] + current + [0xFF]
		current = [e[1],e[2]]
	else:
		m = re.match("^(\\^?)(\\d+)\\:(.)$",e)
		assert m is not None,"Bad key def "+e
		keyCode = int(m.group(2))
		keyCode = keyCode + 0x80 if m.group(1) == '^' else keyCode
		current = current + [ keyCode,m.group(3) ]

code += [0]
print(str(code)[1:-1])

