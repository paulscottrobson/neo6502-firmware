// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      maths_binary.config
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      1st December 2023
//      Reviewed :  No
//      Purpose :   Maths "co-processor" (binary)
//
// ***************************************************************************************
// ***************************************************************************************


	FUNCTION 0 Addition
		if (MATHIsFloatBinary()) {
			MATHWriteFloat(MATHReadFloat(MATH_REG1)+MATHReadFloat(MATH_REG2),MATH_REG1);
		} else {
			MATHWriteInt(MATHReadInt(MATH_REG1)+MATHReadInt(MATH_REG2),MATH_REG1);
		}
	DOCUMENTATION
		Addition
		\newline TODO: describe the parameters

	FUNCTION 1 Subtraction
		if (MATHIsFloatBinary()) {
			MATHWriteFloat(MATHReadFloat(MATH_REG1)-MATHReadFloat(MATH_REG2),MATH_REG1);
		} else {
			MATHWriteInt(MATHReadInt(MATH_REG1)-MATHReadInt(MATH_REG2),MATH_REG1);
		}
	DOCUMENTATION
		Subtraction
		\newline TODO: describe the parameters

	FUNCTION 2 Multiplication
		if (MATHIsFloatBinary()) {
			MATHWriteFloat(MATHReadFloat(MATH_REG1)*MATHReadFloat(MATH_REG2),MATH_REG1);
		} else {
			MATHWriteInt(MATHReadInt(MATH_REG1)*MATHReadInt(MATH_REG2),MATH_REG1);
		}
	DOCUMENTATION
		Multiplication
		\newline TODO: describe the parameters

	FUNCTION 3 Decimal Division
		f2 = MATHReadFloat(MATH_REG2);
		if (f2 == 0.0) {
			*DERROR = 1;
		} else {
			MATHWriteFloat(MATHReadFloat(MATH_REG1)/f2,MATH_REG1);
		}
	DOCUMENTATION
		Decimal Division
		\newline TODO: describe the parameters

	FUNCTION 4 Integer Division
		i1 = MATHReadInt(MATH_REG1);
		i2 = MATHReadInt(MATH_REG2);
		if (i2 == 0) {
			*DERROR = 1;
		} else {
			MATHWriteInt(i1/i2,MATH_REG1);
		}
	DOCUMENTATION
		Integer Division
		\newline TODO: describe the parameters

	FUNCTION 5 Integer Modulus
		i1 = MATHReadInt(MATH_REG1);
		i2 = MATHReadInt(MATH_REG2);
		if (i2 == 0) {
			*DERROR = 1;
		} else {
			MATHWriteInt(abs(i1) % abs(i2),MATH_REG1);
		}
	DOCUMENTATION
		Integer Modulus
		\newline TODO: describe the parameters

	FUNCTION 6 Compare
		r = 0;
		if (MATHIsFloatBinary()) {
			f1 = MATHReadFloat(MATH_REG1);
			f2 = MATHReadFloat(MATH_REG2);
			float diff = (float_abs(f1)+float_abs(f2)) * 0.0000001f;
			if (float_abs(f1-f2) >= diff) {
				r = (f1 < f2 ? 0xFF:0x01);
			}
		} else {
			i1 = MATHReadInt(MATH_REG1);
			i2 = MATHReadInt(MATH_REG2);
			if (i1 != i2) r = (i1 < i2 ? 0xFF:0x01);
		}
		*DPARAMS = r;
	DOCUMENTATION
		Compare Numbers
		\newline TODO: describe the parameters
