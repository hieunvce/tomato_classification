//
// Created by henry on 3/4/18.
//
#include "colorProfile.h"
#include <math.h>


bool isRed(int l, int a, int b)
{
    float redTan=(float)abs(b)/abs(a);
    double redAtan=atan(redTan)*180/PI;
    if (a > 20 && l > 10 && l < 90)
        if (redAtan>0.0 && redAtan<45.0)
            return true;
    return false;
}

bool isGreen(int l, int a, int b)
{
    float greenTan=(float)abs(b)/abs(a);
    double greenAtan=atan(greenTan)*180/PI;
    if (a < -10 && l > 10 && l < 90)
        if (greenAtan>30.0 && greenAtan<80.0)
            return true;
    return false;
}

bool isYellow(int l, int a, int b)
{
    float yellowTan=(float)a/abs(b);
    double yellowAtan=atan(yellowTan)*180/PI;
    if (b > 30 && l > 42 && l < 90)
        if (yellowAtan>-10.0 && yellowAtan<45.0)
            return true;
    return false;
}

/**
 * color: Determine color base on *a value and *b value in L*a*b color space
 * @param a
 * @param b
 * @return
 */
Color color(int l, int a, int b)
{
    if (isRed(l,a,b))
        return RED;
    else if (isYellow(l,a,b))
        return YELLOW;
    else if (isGreen(l,a,b))
        return GREEN;
    else
        return OTHER;
}