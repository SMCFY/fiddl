/*
  ==============================================================================

    Gesture.cpp
    Created: 23 Oct 2017 1:11:42pm
    Author:  geri

  ==============================================================================
*/

#include "Gesture.h"
#include "PlayComponent.h"

float Gesture::xPos;
float Gesture::yPos;

float Gesture::xNew;
float Gesture::xTemp;
float Gesture::xDelta;

float Gesture::yNew;
float Gesture::yTemp;
float Gesture::yDelta;


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

void Gesture::setVelocity(float x, float y)
{
    xNew  = x;
    yNew = y;
    
    xDelta = std::sqrt(std::pow(xNew-xTemp,2));
    yDelta = std::sqrt(std::pow(yNew-yTemp,2));
        
    xTemp = xNew;
    yTemp = yNew;
    
    std::cout << xDelta << "   ";
}

void Gesture::getDirection()
{
    
}
