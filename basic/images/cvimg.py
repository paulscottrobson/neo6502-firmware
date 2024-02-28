# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		cvimg.py
#		Purpose :	Convert graphics file to/from text
#		Date :		28th February 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import sys,re
from gfxtext import *

if len(sys.argv) != 3:
	print("python cvimg.zip <graphics file> <text file>")
	print("python cvimg.zip <text file> <graphics file> ")
	sys.exit(1)

if sys.argv[1].endswith(".gfx"):
	GraphicsConvert().toText(sys.argv[1],sys.argv[2])
else:
	GraphicsConvert().toGraphics(sys.argv[1],sys.argv[2])
sys.exit(0)