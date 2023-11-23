import re

for s in open("forth-original.asm").readlines():
	s = s.rstrip().replace("\t"," ")
	s = ";;"+s[1:] if s.startswith("*") else s
	if s != "" and not s.strip().startswith(";"):
		comment = ""
		p = s.find(";")
		if p > 0:
			comment = "; "+s[p+1:]
			s = s[:p].rstrip()
		m = re.match("^([A-Z0-9\\_]*)\\s*([A-Z]*)\\s*(.*)\\s*",s)
		assert m is not None,"Failed on "+s
		s = ""
		if m.group(1) != "":
			s = m.group(1).lower() if m.group(2) == "EQU" else m.group(1).lower()+":"
		while len(s) < 8:
			s += " "
		cmd = m.group(2)
		cmd = ".word" if cmd == "DW" else cmd
		cmd = ".byte" if cmd == "DFB" else cmd
		cmd = ".text" if cmd == "ASC" else cmd
		cmd = "* = " if cmd == "ORG" else cmd
		cmd = "=" if cmd == "EQU" else cmd
		s = s + cmd.lower()
		while len(s) < 16:
			s += " "
		param = m.group(3).strip()
		if param.startswith("-"):
			param = "({0} & $FFFF)".format(param)
		if not param.startswith("'"):
			param = param.lower()
		s = s + param
		while len(s) < 48:
			s += " "
		s = (s + comment).rstrip()
	else:
		s = s.strip()
	if s.startswith(";;"):
		s = s.strip()
		if s.endswith("*"):
			s = s[:-1].strip()
		print("; {0}".format("*" * 80))
		print(";\n;{0}{1}\n;".format((42-(len(s)>>1))*' ',s[2:].strip()))
		print("; {0}".format("*" * 80))
		s = ""
	print(s)