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
		Register1 :=  -Register 1

	FUNCTION 17 Floor
		if (MATHIsFloatBinary()) {
			f1 = floor(MATHReadFloat(MATH_REG1));
			MATHWriteInt((int32_t)f1,MATH_REG1);
		}
	DOCUMENTATION
		Register1 := floor(Register 1)

	FUNCTION 18 Square Root
		f1 = MATHReadFloat(MATH_REG1);
		if (f1 >= 0) {
			MATHWriteFloat(sqrt(f1),MATH_REG1);
		} else {
			*DERROR = 1;
		}
	DOCUMENTATION
		Register1 := square root(Register 1)

	FUNCTION 19 Sine
		f1 = MATHReadFloat(MATH_REG1);
		MATHWriteFloat(sin(MATHConvertAngleToDefault(f1)),MATH_REG1);
	DOCUMENTATION
		Register1 := sine(Register 1) angles in degrees/radians

	FUNCTION 20 Cosine
		f1 = MATHReadFloat(MATH_REG1);
		MATHWriteFloat(cos(MATHConvertAngleToDefault(f1)),MATH_REG1);
	DOCUMENTATION
		Register1 := cosine(Register 1) angles in degrees/radians

	FUNCTION 21 Tangent
		f1 = MATHReadFloat(MATH_REG1);
		MATHWriteFloat(tan(MATHConvertAngleToDefault(f1)),MATH_REG1);
	DOCUMENTATION
		Register1 := tangent(Register 1) angles in degrees/radians

	FUNCTION 22 Arctangent
		f1 = MATHReadFloat(MATH_REG1);
		MATHWriteFloat(MATHConvertAngleFromDefault(atan(f1)),MATH_REG1);
	DOCUMENTATION
		Register1 := arctangent(Register 1) angles in degrees/radians

	FUNCTION 23 Exponent
		f1 = MATHReadFloat(MATH_REG1);
		MATHWriteFloat(exp(f1),MATH_REG1);
	DOCUMENTATION
		Register1 :=  e to the power of Register 1

	FUNCTION 24 Logarithm
		f1 = MATHReadFloat(MATH_REG1);
		if (f1 >= 0) {
			MATHWriteFloat(log(f1),MATH_REG1);
		} else {
			*DERROR = 1;
		}
	DOCUMENTATION
		Register1 := log(Register 1) natural logarithm

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
		Register1 := absolute value(Register 1)

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
		Register1 := sign(Register 1), returns -1 0 or 1

	FUNCTION 27 Random Decimal
		f1 = (rand() % 100000) / 100000.0;
		MATHWriteFloat(f1,MATH_REG1);
	DOCUMENTATION
		Register1 := random float from 0-1

	FUNCTION 28 Random Integer
		u1 = MATHReadInt(MATH_REG1);
		if (u1 > 0) {
			MATHWriteInt(rand() % u1,MATH_REG1);
		}
	DOCUMENTATION
		Register1 := random integer from 0 to (Register 1-1)
