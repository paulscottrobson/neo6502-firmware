# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		tokens.py
#		Purpose :	Tokens and Token set classes
#		Date :		24th November 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

# *******************************************************************************************
#
#							Class representing an individual token
#
# *******************************************************************************************

class Token(object):
	def __init__(self,tokenID,token):
		self.modifier = ""
		if token != ":" and token.find(":") >= 0:
			self.modifier = token[token.find(":")+1:]
			token = token[:token.find(":")]
		self.token = token
		self.tokenID = tokenID
	#
	def getName(self):
		return self.token 
	def getID(self):
		return self.tokenID
	def getModifier(self):
		return self.modifier

# *******************************************************************************************
#
#								Class representing all the tokens
#
# *******************************************************************************************

class TokenSet(object):
	def __init__(self):
		self.create()

	def getByID(self,id):
		return self.idToToken if id in self.idToToken else None
	def getByName(self,name):
		name = name.strip().lower()
		return self.nameToToken[name] if name in self.nameToToken else None

	def getRange(self,start):
		assert start in self.idToToken
		ids = []
		while start in self.idToToken:
			ids.append(self.idToToken[start])
			start += 1
		return ids 
		
	def getAllTokenNames(self):
		return [x for x in self.nameToToken.keys() if x != ""]

	def add(self,start,tokens,padSize = None):
		self.nextToken = start if start is not None else self.nextToken
		start = self.nextToken
		for s in tokens.upper().strip().split():
			self.addToken(self.nextToken,s)
			self.nextToken += 1
		if padSize is not None:
			while self.nextToken < start + padSize:
				self.addToken(self.nextToken,"")
				self.nextToken += 1

	def addToken(self,tokenID,tokenText):
		tokenText = tokenText.strip().lower()
		assert tokenID not in self.idToToken
		if tokenText != "":
			assert tokenText not in self.nameToToken
		token = Token(tokenID,tokenText)
		self.idToToken[tokenID] = token 
		self.nameToToken[token.getName()] = token

	def create(self):
		self.nextToken = None 
		self.idToToken = {}
		self.nameToToken = {}
		#
		#		Binary tokens between $20 and $3F
		#
		self.add(0x20,"""
			 +:3	-:3 	*:4 	/:4		>>:4	<<:4	%:4 	\\:4
			 &:1	|:1		^:1		>:2		>=:2	<:2		<=:2	<>:2
			 =:2 	!:5 	?:5
			""") 		
		#
		#		Unary tokens from $80-$B0
		#
		self.add(0x80,"""
			!!STR 	!!DEC	$ 		(		RAND(	RND(	ISVAL(	FRAC(	INT(	
			TIME	EVENT(	INKEY$(	ASC(	CHR$(	SQR( 	LEN(  	ABS(  	SGN( 	
			VAL( 	STR$( 	MID$(	LEFT$( 	RIGHT$(	SIN( 	COS(	TAN(	ATAN(	
			SQRT(	LOG(	EXP( 	KEY(		
			""",48)
		#
		#		Structure tokens
		#
		self.add(None,"""
			WHILE:+	WEND:-		IF:+	ENDIF:-	DO:+	LOOP:- 		REPEAT:+ 	UNTIL:-
			PROC:+ 	ENDPROC:-	FOR:+ 	NEXT:- 	CASE:+ 	ENDCASE:- 	!!UN1:+ 	THEN:-
 			""",16)
		#
		#		Keyword tokens (major)
		#
		self.add(None,"""
			!!END 	!!SH1	!!SH2	TO 		STEP	LET 	PRINT	INPUT	
			CALL 	SYS 	EXIT	, 		; 		: 		' 		)		
			READ 	DATA 	[		]		# 		. 		ELSE	WHEN
			""")
		#
		#		Keyword tokens (minor)
		#
		self.add(0x100,"""
			CLEAR 	NEW 	RUN 	STOP 	END 	ASSERT 	LIST 	SAVE 	
			LOAD	GOSUB 	GOTO	RETURN 	RESTORE	DIM	
			""")

if __name__ == "__main__":
	ts = TokenSet()
	print(ts.getRange(0x80))
	print(ts.getRange(0x100))
	print(ts.getRange(0x20))
	print(ts.nameToToken.keys())
	print(ts.getByName("!!str"))

	print(ts.nameToToken.keys())


