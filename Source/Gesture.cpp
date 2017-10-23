/*
  ==============================================================================

    Gesture.cpp
    Created: 23 Oct 2017 1:11:42pm
    Author:  geri

  ==============================================================================
*/

#include "Gesture.h"

double Gesture::getXPos()
{
	return xPos;
}

double Gesture::getYPos()
{
	return yPos;
}

void Gesture::setXPos(double x)
{
	xPos = x;
}

void Gesture::setYPos(double y)
{
	yPos = y;
}