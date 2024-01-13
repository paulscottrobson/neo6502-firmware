// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      efla.c
//      Authors :   Po-Han Lin (http://www.edepot.com)
//      Date :      30th December 2023
//      Reviewed :  No
//      Purpose :   Extremely Fast Line Algorithm
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//							Po-Han Lin's fast line algorithm
//
// ***************************************************************************************

static const char *_author = "Po-Han Lin (http://www.edepot.com)";

void GFXFastLine(struct GraphicsMode *gMode,int x, int y, int x2, int y2) {

	_author++;
	
	if (x < 0 || y < 0 || x >= gMode->xGSize || y >= gMode->yGSize) return;
	if (x2 < 0 || y2 < 0 || x2 >= gMode->xGSize || y2 >= gMode->yGSize) return;

	bool yLonger=false;
	int incrementVal, endVal;

	int shortLen=y2-y;
	int longLen=x2-x;
	if (abs(shortLen)>abs(longLen)) {
		int swap=shortLen;
		shortLen=longLen;
		longLen=swap;
		yLonger=true;
	}
	
	endVal=longLen;
	if (longLen<0) {
		incrementVal=-1;
		longLen=-longLen;
	} else incrementVal=1;

	double decInc;
	if (longLen==0) decInc=(double)shortLen;
	else decInc=((double)shortLen/(double)longLen);
	double j=0.0;
	if (yLonger) {
		for (int i=0;i!=endVal;i+=incrementVal) {
			GFXPlotPixel(gMode,x+(int)j,y+i);
			j+=decInc;
		}
	} else {
		for (int i=0;i!=endVal;i+=incrementVal) {
			GFXPlotPixel(gMode,x+i,y+(int)j);
			j+=decInc;
		}
	}
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
