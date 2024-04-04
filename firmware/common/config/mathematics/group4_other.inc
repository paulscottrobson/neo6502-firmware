// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      maths_other.config
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Bill Auger
//      Date :      1st December 2023
//      Reviewed :  No
//      Purpose :   Maths "co-processor"
//
// ***************************************************************************************
// ***************************************************************************************


	FUNCTION 32 Number to Decimal
		MATHProcessDecimal(DCOMMAND);
	DOCUMENTATION
		Helper function for tokeniser, do not use.

	FUNCTION 33 String to Number
		MATHConvertStringToNumber(DCOMMAND);
	DOCUMENTATION
		Convert the length prefixed string at \Param{4,5} to a constant in Register1.

	FUNCTION 34 Number to String
		MATHConvertNumberToString(DCOMMAND);
	DOCUMENTATION
		Convert the constant in Register1 to a length prefixed string which is stored at \Param{4,5}

	FUNCTION 35 Set Degree/Radian Mode
		MATHSetAngleMeasure(DPARAMS[0] != 0);
	DOCUMENTATION
		Sets the use of degrees (the default) when non zero, radians when zero.

