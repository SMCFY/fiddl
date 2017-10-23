/*
  ==============================================================================

    Gesture.cpp
    Created: 23 Oct 2017 1:11:42pm
    Author:  geri

  ==============================================================================
*/

#include "Gesture.h"

float Gesture::xPos;
float Gesture::yPos;

float Gesture::getXPos()
{
	return xPos;
}

float Gesture::getYPos()
{
	return yPos;
}

void Gesture::setXPos(float x)
{
	Gesture::xPos = x;
}

void Gesture::setYPos(float y)
{
	Gesture::yPos = y;
}
