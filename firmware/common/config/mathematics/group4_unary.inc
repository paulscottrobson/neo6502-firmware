// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      maths_unary.config
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Bill Auger
//      Date :      1st December 2023
//      Reviewed :  No
//      Purpose :   Maths "co-processor" unary ops
//
// ***************************************************************************************
// ***************************************************************************************


	FUNCTION 16 Negate
		if (MATHIsFloatUnary()) {
			MATHWriteFloat(-MATHReadFloat(MATH_REG1),MATH_REG1);
		} else {
			MATHWriteInt(-MATHReadInt(MATH_REG1),MATH_REG1);
		}
	DOCUMENTATION
		Negate
		\newline TODO: describe the parameters

	FUNCTION 17 Floor
		if (MATHIsFloatBinary()) {
			f1 = floor(MATHReadFloat(MATH_REG1));
			MATHWriteInt((uint32_t)f1,MATH_REG1);
		}
	DOCUMENTATION
		Floor
		\newline TODO: describe the parameters

	FUNCTION 18 Square Root
		f1 = MATHReadFloat(MATH_REG1);
		if (f1 >= 0) {
			MATHWriteFloat(sqrt(f1),MATH_REG1);
		} else {
			*DERROR = 1;
		}
	DOCUMENTATION
		Square Root
		\newline TODO: describe the parameters

	FUNCTION 19 Sine
		f1 = MATHReadFloat(MATH_REG1);
		MATHWriteFloat(sin(TORADIANS(f1)),MATH_REG1);
	DOCUMENTATION
		Sine
		\newline TODO: describe the parameters

	FUNCTION 20 Cosine
		f1 = MATHReadFloat(MATH_REG1);
		MATHWriteFloat(cos(TORADIANS(f1)),MATH_REG1);
	DOCUMENTATION
		Cosine
		\newline TODO: describe the parameters

	FUNCTION 21 Tangent
		f1 = MATHReadFloat(MATH_REG1);
		MATHWriteFloat(tan(TORADIANS(f1)),MATH_REG1);
	DOCUMENTATION
		Tangent
		\newline TODO: describe the parameters

	FUNCTION 22 Arctangent
		f1 = MATHReadFloat(MATH_REG1);
		MATHWriteFloat(FROMRADIANS(atan(f1)),MATH_REG1);
	DOCUMENTATION
		Arctangent
		\newline TODO: describe the parameters

	FUNCTION 23 Exponent
		f1 = MATHReadFloat(MATH_REG1);
		MATHWriteFloat(exp(f1),MATH_REG1);
	DOCUMENTATION
		Exponent
		\newline TODO: describe the parameters

	FUNCTION 24 Logarithm
		f1 = MATHReadFloat(MATH_REG1);
		if (f1 >= 0) {
			MATHWriteFloat(log(f1),MATH_REG1);
		} else {
			*DERROR = 1;
		}
	DOCUMENTATION
		Logarithm
		\newline TODO: describe the parameters

	FUNCTION 25 Absolute Value
		if (MATHIsFloatUnary()) {
			MATHWriteFloat(fabs(MATHReadFloat(MATH_REG1)),MATH_REG1);
		} else {
			u1 = MATHReadInt(MATH_REG1);
			if (u1 & 0x80000000) {
				MATHWriteInt(-u1,MATH_REG1);
			}
		}
	DOCUMENTATION
		Absolute Value
		\newline TODO: describe the parameters

	FUNCTION 26 Sign
		u1 = 0;
		if (MATHIsFloatUnary()) {
			f1 = MATHReadFloat(MATH_REG1);
			if (f1 != 0.0) u1 = (f1 < 0) ? -1 : 1;
		} else {
			i1 = MATHReadInt(MATH_REG1);
			if (i1 != 0) u1 = (i1 < 0) ? -1 : 1;
		}
		MATHWriteInt(u1,MATH_REG1);
	DOCUMENTATION
		Sign
		\newline TODO: describe the parameters

	FUNCTION 27 Random Decimal
		f1 = (rand() % 100000) / 100000.0;
		MATHWriteFloat(f1,MATH_REG1);
	DOCUMENTATION
		Random Decimal
		\newline TODO: describe the parameters

	FUNCTION 28 Random Integer
		u1 = MATHReadInt(MATH_REG1);
		if (u1 > 0) {
			MATHWriteInt(rand() % u1,MATH_REG1);
		}
	DOCUMENTATION
		Random Integer
		\newline TODO: describe the parameters
