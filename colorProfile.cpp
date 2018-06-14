//
// Created by henry on 3/4/18.
//
#include "colorProfile.h"

bool isRed(int a, int b)
{
    return (a >= -5) && (a <= 75) && (b >= 20) && (b <= 65) && abs(a - b) >= 0 && abs(a - b) <= 21;
}

bool isGreen(int a, int b)
{
    return (a >= -50) && (a <= -10) && (b >= 10) && (b <= 85) && abs(a - b) >= 40 && abs(a - b) <= 100;
}

bool isYellow(int a, int b)
{
    return (a >= -10) && (a <= 40) && (b >= 50) && (b <= 90) && abs(a - b) >= 25 && abs(a - b) <= 75;
}

/**
 * color: Determine color base on *a value and *b value in L*a*b color space
 * @param a
 * @param b
 * @return
 */
Color color(int a, int b)
{
    if (isRed(a,b))
        return RED;
    else if (isYellow(a,b))
        return YELLOW;
    else if (isGreen(a,b))
        return GREEN;
    else
        return OTHER;
}