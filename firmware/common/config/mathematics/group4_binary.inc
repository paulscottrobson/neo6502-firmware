// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      maths_binary.config
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Bill Auger
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
		Register1 := Register 1 + Register2

	FUNCTION 1 Subtraction
		if (MATHIsFloatBinary()) {
			MATHWriteFloat(MATHReadFloat(MATH_REG1)-MATHReadFloat(MATH_REG2),MATH_REG1);
		} else {
			MATHWriteInt(MATHReadInt(MATH_REG1)-MATHReadInt(MATH_REG2),MATH_REG1);
		}
	DOCUMENTATION
		Register1 := Register 1 - Register2

	FUNCTION 2 Multiplication
		if (MATHIsFloatBinary()) {
			MATHWriteFloat(MATHReadFloat(MATH_REG1)*MATHReadFloat(MATH_REG2),MATH_REG1);
		} else {
			MATHWriteInt(MATHReadInt(MATH_REG1)*MATHReadInt(MATH_REG2),MATH_REG1);
		}
	DOCUMENTATION
		Register1 := Register 1 * Register2

	FUNCTION 3 Decimal Division
		f2 = MATHReadFloat(MATH_REG2);
		if (f2 == 0.0) {
			*DERROR = 1;
		} else {
			MATHWriteFloat(MATHReadFloat(MATH_REG1)/f2,MATH_REG1);
		}
	DOCUMENTATION
		Register1 := Register 1 / Register2 (floating point)

	FUNCTION 4 Integer Division
		i1 = MATHReadInt(MATH_REG1);
		i2 = MATHReadInt(MATH_REG2);
		if (i2 == 0) {
			*DERROR = 1;
		} else {
			MATHWriteInt(i1/i2,MATH_REG1);
		}
	DOCUMENTATION
		Register1 := Register 1 / Register2 (integer result)

	FUNCTION 5 Integer Modulus
		i1 = MATHReadInt(MATH_REG1);
		i2 = MATHReadInt(MATH_REG2);
		if (i2 == 0) {
			*DERROR = 1;
		} else {
			MATHWriteInt(abs(i1) % abs(i2),MATH_REG1);
		}
	DOCUMENTATION
		Register1 := Register 1 mod Register2

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
		Parameter:0 := Register 1 compare Register2 : returns $FF, 0, 1 for less equal and greater.
		Note: float comparison is approximate because of rounding.

	FUNCTION 7 Power
		f1 = MATHReadFloat(MATH_REG1);
		f2 = MATHReadFloat(MATH_REG2);
		MATHWriteFloat((float)pow(f1,f2),MATH_REG1);
	DOCUMENTATION
		Register1 := Register 1 to the power of Register2 (floating point result whatever)

	FUNCTION 8 Distance (counter-rectangle)
		f1 = MATHReadFloat(MATH_REG1);
		f2 = MATHReadFloat(MATH_REG2);
		MATHWriteFloat((float)sqrt((f1*f1)+(f2*f2)),MATH_REG1);
	DOCUMENTATION
		Register1 := Square root of (Register1 * Register1) + (Register2 * Register2)

	FUNCTION 9 Direction (arctangent2)
		f1 = MATHReadFloat(MATH_REG1);
		f2 = MATHReadFloat(MATH_REG2);
		MATHWriteFloat(MATHConvertAngleFromDefault(atan2(f1,f2)),MATH_REG1);
	DOCUMENTATION
		Register1 := arctangent2(Register 1,Register 2) - angle in degrees/radians


		