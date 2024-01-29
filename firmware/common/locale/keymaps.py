# ***************************************************************************************
# ***************************************************************************************
#
#      Name :      keymaps.py
#      Authors :   Paul Robson (paul@robsons.org.uk)
#      Date :      29th January 2024
#      Reviewed :  No
#      Purpose :   Locale Processor
#
# ***************************************************************************************
# ***************************************************************************************

import sys,os,re

# ***************************************************************************************
#
#										Keymap class
#
# ***************************************************************************************

class Keymap(object):
	#
	#		Set up and load in keymap data
	#
	def __init__(self,mapfile):
		stubName = mapfile.split(os.sep)[-1]  									# Identify mapping from file name
		assert len(stubName) == 9,"Keymaps can only currently use 2 character IDs"
		self.mapName = stubName[:2].lower()
		self.currentMapping = None 
		self.mappings = {}  													# Mappings for each square bracket type in raw format.
		for s in open(mapfile).readlines():
			s = s if s.find("#") < 0 else s[:s.find("#")]  						# Remove comments and tidy up.
			s = s.strip()
			if s.startswith("[") and s.endswith("]"):  							# New group
				s = s[1:-1].lower()  											# Get name
				if s not in self.mappings:  									# Create it if needed.
					self.mappings[s] = []
				self.currentMapping = self.mappings[s]  						# Make it current
			elif s != "":
				assert self.currentMapping is not None,"No mapping in keyboard file"
				self.currentMapping.append(s)

		self.mapMasks = { "noshift":"KEY_NOSHIFT", "shift":"KEY_SHIFT", "alt":"KEY_ALT","altgr":"KEY_ALTGR" }
	#
	#		Render the mapping,
	#
	def render(self):
		print("//\n//\tMapping for {0}\n//\n".format(self.mapName))
		data = self.createMapping()  											# create the raw mapping data
		print("{0},'{1}','{2}',".format(len(data)+3,							# output header - total length, map name (2 chars)
										self.mapName[0],self.mapName[1]))
		print("\t{0},".format(",".join(data))) 						 			# output mapping data
	#
	#		Convert the locale data to the internal format, sequence of records ending in $FF
	#
	def createMapping(self):
		mapData = []
		for section in self.mappings.keys():  									# Scan through the mappings.
			assert section in self.mapMasks,"Unknown section "+section  		# section exists
			for mapping in self.mappings[section]:  							# work through its mapping
				mapData += self.createSingleMapping(mapping,self.mapMasks[section])
		mapData.append("0xFF")  												# end of list
		return mapData
	#
	#		Convert a single mapping
	#
	def createSingleMapping(self,mapping,keyMask):
		m = re.match("^(\\d+)\\:(.*)$",mapping)  								# process mapping
		assert m is not None,"Bad mapping "+mapping
		keyID = m.group(1) 		 												# check the key ID
		assert int(keyID) < 128,"Bad key id in "+mapping
		if len(m.group(2)) == 1:  												# character to map to
			keyCode = str(ord(m.group(2)[0]))
		else:
			mn = re.match("^\\d+$",m.group(2))   								# must be ASCII value
			assert mn is not None,"Bad mapping character "+mapping
			keyCode = m.group(2)
		assert int(keyCode) >= 32 and int(keyCode) < 127,"Bad character value : "+mapping
		return [keyMask,keyID,keyCode]

mapList = []
for root,dirs,files in os.walk("locale"):
	for f in [x for x in files if x.endswith(".locale")]:
		fName = root+os.sep+f
		mapList.append(Keymap(fName))

for m in mapList:
	m.render()
print("0")

