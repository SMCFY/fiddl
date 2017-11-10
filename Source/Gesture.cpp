/*
  ==============================================================================

    Gesture.cpp
    Created: 23 Oct 2017 1:11:42pm
    Author:  geri

  ==============================================================================
*/

#include "Gesture.h"

OwnedArray<Gesture::Position> Gesture::fingers;

float Gesture::xNew;
float Gesture::xTemp;
float Gesture::xDelta;

float Gesture::yNew;
float Gesture::yTemp;
float Gesture::yDelta;

float Gesture::dist;

float Gesture::directionDeltaX;
float Gesture::directionDeltaY;
float Gesture::absDirectionDeltaX;
float Gesture::absDirectionDeltaY;
int Gesture::directionBuffSize = 5;
String Gesture::direction;

bool Gesture::resetPos = false;

float Gesture::compWidth;
float Gesture::compHeight; 

Point<float> Gesture::normalizeCoordinates(Point<float> p)
{
    p.x = p.x / compWidth;
    
    p.y = ((compHeight - p.y) > 0 ? (compHeight - p.y) / compHeight : 0);

    return p;
}

void Gesture::addFinger(const MouseEvent& e)
{
    Gesture::Position* f = new Gesture::Position(e.source, normalizeCoordinates(e.position));
    fingers.add(f);
}

void Gesture::rmFinger(const MouseEvent& e)
{
    for (int i = 0; i < fingers.size(); i++)
    {
        if (fingers[i]->sourceIndex == e.source.getIndex())
        {
            fingers.removeObject(fingers[i]); // remove stored input source from the array which matches the MouseEvent
        }
    }
}

Point<float> Gesture::getFingerPosition(int index)
{
    return fingers[index]->pos;
}

void Gesture::updateFingers(const MouseInputSource& mis, int index)
{
        for (int i = 0; i < fingers.size(); i++)
        {
            if(fingers[i]->sourceIndex == index) // checks whether the stored input source exists or not
            {
                fingers[i]->pos = normalizeCoordinates(mis.getScreenPosition());
                //std::cout << index << " " << fingers[i]->sourceIndex << " - " << fingers[i]->pos.x << " " << fingers[i]->pos.y << " " << std::endl;
            }
        }
}

int Gesture::getNumFingers()
{
    return fingers.size();
}

void Gesture::setVelocity(float x, float y)
{
    xNew  = x;
    yNew = y;
    
    if(resetPos)
    {
        xTemp = xNew;
        yTemp = yNew;
        resetPos = false;
    }
    
    xDelta = std::sqrt(std::pow(xNew-xTemp,2));
    yDelta = std::sqrt(std::pow(yNew-yTemp,2));
    dist = std::sqrt(std::pow(xNew-xTemp,2)+std::pow(yNew-yTemp,2));
        
    xTemp = xNew;
    yTemp = yNew;
}

void Gesture::setDirection(float p [directionBuffSize][2])
{
    absDirectionDeltaX = std::sqrt(std::pow(p[0][0]-p[directionBuffSize-1][0],2));
    absDirectionDeltaY = std::sqrt(std::pow(p[0][1]-p[directionBuffSize-1][1],2));
    
    directionDeltaX = p[directionBuffSize-1][0]-p[0][0];
    directionDeltaY = p[directionBuffSize-1][1]-p[0][1];
    
    if(directionDeltaX > 0 && directionDeltaX != 0)
    {
        if(absDirectionDeltaY > absDirectionDeltaX)
        {
            if(directionDeltaY > 0)
            {
                direction = "UP";
            } else if (directionDeltaY < 0)
            {
                direction = "DOWN";
            }
        } else
        {
            direction = "RIGHT";
        }
    } else if (directionDeltaX < 0)
    {
        if(absDirectionDeltaY > absDirectionDeltaX)
        {
            if(directionDeltaY > 0)
            {
                direction = "UP";
            } else if (directionDeltaY < 0)
            {
                direction = "DOWN";
            }
        } else
        {
            direction = "LEFT";
        }
    } /*else if (directionDeltaX == 0 && directionDeltaY == 0)
        direction = "TAP";
    */
    //std::cout << p[directionBuffSize-1][0] << "  " << p[0][0] << "  " << p[1][0];
    //std::cout << direction << "\n";
    //std::cout << "X:  " << directionDeltaX << "   Y:   " << directionDeltaY << "   ";
}

String Gesture::getDirection()
{
    return direction;
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

bool Gesture::getResetPos()
{
    return resetPos;
}

void Gesture::setResetPos(bool reset)
{
    Gesture::resetPos= reset;
}

void Gesture::setCompWidth(float w)
{
    Gesture::compWidth = w;
}

void Gesture::setCompHeight(float h)
{
    Gesture::compHeight = h;
}
