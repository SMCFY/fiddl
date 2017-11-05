/*
  ==============================================================================

    Gesture.cpp
    Created: 23 Oct 2017 1:11:42pm
    Author:  geri

  ==============================================================================
*/

#include "Gesture.h"

OwnedArray<Gesture::Position> Gesture::fingers;
//Gesture::Position Gesture::fing[3];

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

void Gesture::addFinger(const MouseEvent& e)
{
    Gesture::Position* f = new Gesture::Position();
    f->xPos = e.position.x;
    f->yPos = e.position.y;
    fingers.add(f);
}

void Gesture::rmFinger(const MouseEvent& e)
{
    fingers.removeObject(getFinger(e));
}

Gesture::Position* Gesture::getFinger(const MouseEvent& e)
{
    for (int i = 0; i < fingers.size(); i++)
    {
        //Gesture::Position *f = fingers[i];
        if (fingers[i]->xPos == e.position.getX() && fingers[i]->yPos == e.position.getY())
            return fingers[i];
    }
    return nullptr;
}

Gesture::Position* Gesture::getFingerPosition(int index)
{
    return fingers[index];
}

void Gesture::updateFingers(const MouseEvent& e)
{
    for (int i = 0; i < fingers.size(); i++)
    {
        fingers[i]->xPos = e.position.x;
        fingers[i]->yPos = e.position.y;
    }
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


