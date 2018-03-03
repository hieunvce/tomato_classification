//
// Created by henry on 3/4/18.
//
#include "colorProfile.h"

bool isRed(int a, int b)
{
    if ((a>=-5) && (a<=75) && (b>=20) && (b<=65) && abs(a-b)>=0 && abs(a-b)<=21)
        return true;
    return false;
}
bool isGreen(int a, int b)
{
    if ((a>=-50) && (a<=-10) && (b>=10) && (b<=85) && abs(a-b)>=40 && abs(a-b)<=100)
        return true;
    return false;
}
bool isYellow(int a, int b)
{
    if ((a>=-5) && (a<=75) && (b>=20) && (b<=65) && abs(a-b)>=0 && abs(a-b)<=21)
        return true;
    return false;
}
