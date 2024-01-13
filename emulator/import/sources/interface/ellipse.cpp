// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      ellipse.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      31st December 2023
//      Reviewed :  No
//      Purpose :   Midpoint Ellipse algorithm.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//                              Handle rendering symmetry
//
// ***************************************************************************************

void _GFXDrawSymmetry(struct GraphicsMode *gMode,int xs,int ys,int x,int y,int useSolidFill) {
    if (useSolidFill) {
        GFXFastLine(gMode,xs-x,ys+y,xs+x,ys+y);
        GFXFastLine(gMode,xs-x,ys-y,xs+x,ys-y);
    } else {
        GFXPlotPixelChecked(gMode,xs+x,ys+y);
        GFXPlotPixelChecked(gMode,xs+x,ys-y);
        GFXPlotPixelChecked(gMode,xs-x,ys+y);
        GFXPlotPixelChecked(gMode,xs-x,ys-y);
    }
}

// ***************************************************************************************
//
//                                  Draw boxed ellipse
//
// ***************************************************************************************

void GFXEllipse(struct GraphicsMode *gMode,int x1,int y1,int x2,int y2,int useSolidFill) {
    int xc = (x1 + x2) / 2;
    int yc = (y1 + y2) / 2;
    int rx = (x1-x2)/2;rx = (rx < 0) ? -rx:rx;
    int ry = (y1-y2)/2;ry = (ry < 0) ? -ry:ry;

    float dx, dy, d1, d2, x, y;
    x = 0;
    y = ry;
 
    // Initial decision parameter of region 1
    d1 = (ry * ry) - (rx * rx * ry) + 
                     (0.25 * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;
 
    // For region 1
    while (dx < dy) 
    {
        // Print points based on 4-way symmetry
        _GFXDrawSymmetry(gMode,xc,yc,x,y,useSolidFill);
        // cout << x + xc << " , " << y + yc << endl;
        // cout << -x + xc << " , " << y + yc << endl;
        // cout << x + xc << " , " << -y + yc << endl;
        // cout << -x + xc << " , " << -y + yc << endl;
 
        // Checking and updating value of
        // decision parameter based on algorithm
        if (d1 < 0)
        {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        }
        else
        {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }
 
    // Decision parameter of region 2
    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) + 
         ((rx * rx) * ((y - 1) * (y - 1))) -
          (rx * rx * ry * ry);
 
    // Plotting points of region 2
    while (y >= 0)
    {
 
        // Print points based on 4-way symmetry
        _GFXDrawSymmetry(gMode,xc,yc,x,y,useSolidFill);
        // cout << x + xc << " , " << y + yc << endl;
        // cout << -x + xc << " , " << y + yc << endl;
        // cout << x + xc << " , " << -y + yc << endl;
        // cout << -x + xc << " , " << -y + yc << endl;
 
        // Checking and updating parameter
        // value based on algorithm
        if (d2 > 0) 
        {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        }
        else
        {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }
}

// ***************************************************************************************
//
//      Date        Revision
//      ====        ========
//
// ***************************************************************************************
