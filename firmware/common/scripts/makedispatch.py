#!/bin/env python3

# ***************************************************************************************
# ***************************************************************************************
#
#      Name :      makedispatch.py
#      Authors :   Paul Robson (paul@robsons.org.uk)
#      Date :      22nd November 2023
#      Reviewed :  No
#      Purpose :   Build dispatch files.
#
# ***************************************************************************************
# ***************************************************************************************

import sys,os,re
from datetime import date


# ***************************************************************************************
#
# Configuration
#
# ***************************************************************************************

THIS_DIR     = os.path.realpath(os.path.dirname(__file__))
ROOT_DIR     = os.path.realpath(os.path.join(THIS_DIR , '..' , '..' , '..'))
DOCS_DIR     = os.path.join(ROOT_DIR , 'documents' , 'release' , 'source')
CFG_DIR      = os.path.join(ROOT_DIR , 'firmware' , 'common' , 'config'  )
CFG_FILE     = os.path.join(CFG_DIR  , 'dispatch.config'                 )
TEX_IN_FILE  = os.path.join(DOCS_DIR , 'api.tex.in'                      )
TEX_OUT_FILE = os.path.join(DOCS_DIR , 'api.tex'                         )
ANCHORS      = ( 'GROUP' , 'FUNCTION' , 'DOCUMENTATION' )


# ***************************************************************************************
#
# Represents function and associated code
#
# ***************************************************************************************

class Function(object):
	def __init__(self,funcID,funcName = None):
		self.funcID = int(funcID)
		self.docLines = []
		self.codeLines = []
		self.funcName = funcName if funcName is not None else "Function "+str(funcID)
	#
	def addDoc(self,docLine):
		self.docLines.append(docLine.rstrip())
	#
	def addCode(self,codeLine):
		self.codeLines.append(codeLine.rstrip())
	#
	def getID(self):
		return self.funcID
	#
	def render(self):
		print("\t\t\tcase {0}:".format(self.funcID))
		for l in self.codeLines:
			print("\t\t\t\t"+l)
		print("\t\t\t\tbreak;")


# ***************************************************************************************
#
# Represents group of functions
#
# ***************************************************************************************

class Group(object):
	def __init__(self,groupID,groupName = None,pageBreaks = None):
		self.groupID = int(groupID)
		self.groupName = groupName if groupName is not None else "Group "+str(groupID)
		self.pageBreaks = pageBreaks
		self.functions = {}
	#
	def getID(self):
		return self.groupID
	#
	def addFunction(self,func):
		assert func.getID() not in self.functions
		self.functions[func.getID()] = func
	#
	def render(self):
		print("\tcase {0}:".format(self.groupID))
		if self.groupID == 4:
			print("\t\tMATHCommon(DPARAMS);")		
		print("\t\tswitch (*DFUNCTION) {")
		funcs = [x for x in self.functions.keys()]
		funcs.sort()
		for f in funcs:
			self.functions[f].render()
		print("\t\t\tdefault:")
		print("\t\t\t\tWARN_GROUP(*DCOMMAND,*DFUNCTION);")
		print("\t\t\t\tbreak;")
		print("\t\t}")
		print("\t\tbreak;")


# ***************************************************************************************
#
# Represents a Dispatch API, a collection of groups
#
# ***************************************************************************************

class DispatchAPI(object):
	def __init__(self):
		self.currentFunction = None 
		self.currentGroup = None
		self.groups = {}
		self.parsingState = 'start'
		self.globalCodeLines = []
	#
	def addGroup(self,group):	
		assert group.getID() not in self.groups 
		self.currentGroup = group
		self.groups[group.getID()] = group 
		self.currentFunction = None 
	#
	def addFunction(self,func):
		assert self.currentGroup is not None
		self.currentFunction = func
		self.currentGroup.addFunction(func)
	#
	def addGlobalCode(self,codeLine):
		self.globalCodeLines.append(codeLine)
	#
	def addDoc(self,docLine):
		assert self.currentFunction is not None 
		self.currentFunction.addDoc(docLine)
	#
	def addCode(self,codeLine):
		assert self.currentFunction is not None
		self.currentFunction.addCode(codeLine)
	#
	def processLine(self,line):
		line  = re.sub(r'//.*' , '' , line).strip()
		words = line.split()
		head  = words.pop(0) if line.startswith(ANCHORS) else ''

		if line:
			if head == 'GROUP':
				group_n           = words.pop(0)
				page_breaks       = [ int(ea) for ea in words.pop(0)[1:].split(',') ] \
				                    if words and words[0].startswith(',') else []
				group_name        = ' '.join(words) or None
				self.parsingState = 'group'
				self.addGroup(Group(group_n , group_name , page_breaks))
			elif head == 'FUNCTION':
				func_n            = words.pop(0)
				func_name         = ' '.join(words)
				self.parsingState = 'function'
				self.addFunction(Function(func_n , func_name))
			elif head == 'DOCUMENTATION':
				doc_line          = ' '.join(words).strip()
				self.parsingState = 'docs'
				if doc_line:
					self.addDoc(doc_line)
			elif self.parsingState == 'start':
				code_line = line.rstrip()
				self.addGlobalCode(code_line)
			elif self.parsingState == 'function':
				code_line = line.rstrip()
				self.addCode(code_line)
			elif self.parsingState == 'docs':
				doc_line = line.rstrip()
				self.addDoc(doc_line)
				if doc_line == '\\newline': self.addDoc('\n')
	#
	def render(self):
		print("//\n// This file is automatically generated\n//\n\n")

		if self.globalCodeLines:
			for line in self.globalCodeLines: print(line)
			print('\n')

		print("switch (*DCOMMAND) {")
		groups = [x for x in self.groups.keys()]
		groups.sort()
		for k in groups:
			self.groups[k].render()
		print("\tdefault:")
		print("\t\tWARN_GROUP(*DCOMMAND,*DFUNCTION);")
		print("\t\tbreak;")
		print("}")

	# generate documentation binaries
	def renderTableHead(self,fn_table_tex,header):
		#fn_table_tex.append('\\centering\\textbf{%s}' % header)
		fn_table_tex.append('\\begin{longtable*}{ | c | l | p{12cm} | }')

		fn_table_tex.append('\\caption*{%s} \\\\' % header)
		fn_table_tex.append('\\hline')
		fn_table_tex.append("\\textbf{Function} & \\textbf{Assembly} & \\textbf{Description} \\\\")
		fn_table_tex.append('\\hline')
		fn_table_tex.append('\\endfirsthead')

		fn_table_tex.append('\\caption*{%s (continued)} \\\\' % header)
		fn_table_tex.append('\\hline')
		fn_table_tex.append("\\textbf{Function} & \\textbf{Assembly} & \\textbf{Description} \\\\")
		fn_table_tex.append('\\hline')
		fn_table_tex.append('\\endhead')

	def renderTableTail(self,fn_table_tex):
		fn_table_tex.append('\\end{longtable*}')

	def renderDocs(self):
		rev_date     = str(date.fromtimestamp(os.stat(CFG_FILE).st_ctime))
		fn_table_tex = []

		fn_table_tex.append('% makedispatch: BEGIN')
		for group_id in sorted(self.groups):
			group  = self.groups[group_id]
			header = "Group %s - %s Functions" % (group_id , group.groupName)
			fn_ids = sorted([ fn_id for fn_id in group.functions.keys() ])

			for fn_id in fn_ids:
				if fn_id == fn_ids[0]:
					if group_id != 1:
						fn_table_tex.append('\\pagebreak')
					self.renderTableHead(fn_table_tex , header)

				function = group.functions[fn_id]
				fn_name  = function.funcName
				docLines = function.docLines if function.docLines[-1] else function.docLines[0:-2]

				fn_table_tex.append('\\ApiFnRow{%s}{%s}{' % (fn_id , fn_name))
				fn_table_tex.extend([ '\n'.join(docLines) , '}' ])
			self.renderTableTail(fn_table_tex)
		fn_table_tex.extend([ '' , '% makedispatch: END' , '' ])

		with open(TEX_IN_FILE  , 'r' , encoding='utf-8'               ) as texi_file:
			tex_in_lines = texi_file.readlines()
		with open(TEX_OUT_FILE , 'w' , encoding='utf-8' , newline='\n') as texi_file:
			texi_file.write('%\n% This file is automatically generated\n%\n\n\n')
			for tex_in_line in tex_in_lines:
				tex_subst = '\n'.join(fn_table_tex).replace(r'$' , '\$').replace('\\' , r'\\')
				tex_out   = re.sub(r'@DATE@'      , rev_date  , tex_in_line)
				tex_out   = re.sub(r'@FN_TABLES@' , tex_subst , tex_out    )

				texi_file.write(tex_out)

	def sendFile(self , in_file):
		# accept input file as path or filename - no assumption of PWD
		in_file = in_file if os.path.isfile(in_file) else os.path.join(CFG_DIR , in_file)

		try:
			for l in open(in_file).readlines():
				if l.strip().startswith("include"):
					self.sendFile(l.strip()[7:].strip())
				else:
					self.processLine(l)
		except:
			raise Exception("error reading input file: " + os.path.realpath(in_file))

	def validateDocs(self):
		fn_err_fmt  = "ERROR: Group (%d) has no 'FUNCTIONS' entries"
		doc_err_fmt = "ERROR: Function (%d,%d) has no 'DOCUMENTATION' entry"

		for group_id in sorted(self.groups):
			if not self.groups[group_id].functions:
				raise Exception(fn_err_fmt % group_id)

			for fn_id in sorted(self.groups[group_id].functions):
				if not self.groups[group_id].functions[fn_id].docLines:
					raise Exception(doc_err_fmt % (group_id , fn_id))


api = DispatchAPI()
for f in sys.argv[1:]:
	api.sendFile(f)
api.render()

api.validateDocs()
api.renderDocs()
