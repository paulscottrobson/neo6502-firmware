# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		scanx.py
#		Purpose :	Check not assuming X on the stack
#		Date :		9th January 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,sys,math,re
#
#		Scan looking for expression/term calls, check previous modifies X or sets it
#		(couple of occasions this has been forgotten)
#
loads = {}
for root,dirs,files in os.walk("sources"):
	for f in files:
		if f.endswith(".asm"):
			previous = ""
			lineNumber = 1
			for s in open(root+os.sep+f).readlines():
				s = s.rstrip()
				isExpr = s.find("EvaluateTerm") >= 0
				isExpr |= s.find("EXPEvaluateExpression") >= 0
				isExpr |= s.find("EXPEvalNumber") >= 0
				isExpr |= s.find("EXPEvalInteger") >= 0
				isExpr |= s.find("EXPEvalInteger16") >= 0
				isExpr |= s.find("EXPEvalInteger8") >= 0
				isExpr |= s.find("EXPEvalString") >= 0

				isOk = previous.find("inx") >= 0
				isOk |= previous.find("dex") >= 0
				isOk |= previous.find("ldx") >= 0

				if isExpr and not isOk:
					print("# "+root+os.sep+f,lineNumber,previous)
					loads[root+os.sep+f] = 0

				lineNumber += 1
				if s.strip() != "":
					previous = s

print("subl "+" ".join([x for x in loads.keys()]))