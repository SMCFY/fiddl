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

float Gesture::xNew;
float Gesture::xTemp;
float Gesture::xDelta;

float Gesture::yNew;
float Gesture::yTemp;
float Gesture::yDelta;

float Gesture::dist;

float Gesture::directionHyp;
float Gesture::directionH;
float Gesture::directionAngle;
float Gesture::points [20][2];

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
    dist = std::sqrt(std::pow(xNew-xTemp,2)+std::pow(yNew-yTemp,2));
        
    xTemp = xNew;
    yTemp = yNew;
    
    //std::cout << xDelta << "   ";
}

void Gesture::getDirection(float p [20][2])
{
    directionHyp = std::sqrt(std::pow(p[1][0]-p[20][0],2)+std::pow(p[1][1]-p[20][1],2));
    directionH = std::sqrt(std::pow(p[1][1]-p[20][1],2));
    
    directionAngle = std::asin(directionH/directionHyp);
    
    std::cout << directionAngle;
}

float Gesture::getVelocity()
{
    return std::pow(dist+1,4) ;
}

float Gesture::getVelocityX()
{
    return std::pow(xDelta+1,4);
}

float Gesture::getVelocityY()
{
    return std::pow(yDelta+1,4);
}


