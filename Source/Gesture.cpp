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
float Gesture::absDistFromOrigin;
float Gesture::velMax;

float Gesture::directionDeltaX;
float Gesture::directionDeltaY;
float Gesture::absDirectionDeltaX;
float Gesture::absDirectionDeltaY;
int Gesture::directionBuffSize = 5;
String Gesture::direction;

bool Gesture::resetPos = false;

float Gesture::compWidth;
float Gesture::compHeight;

float Gesture::tapDist;
bool Gesture::isTap = false;

float Gesture::discretePitchVal;

Point<float> Gesture::normalizeCoordinates(Point<float> p)
{
    p.x = p.x / compWidth;
    
    p.y = ((compHeight - p.y) > 0 ? (compHeight - p.y) / compHeight : 0);

    return p;
}

void Gesture::addFinger(const MouseEvent& e)
{
    Gesture::Position* f = new Gesture::Position(e.source, e.position);
    fingers.add(f);
    f->path.startNewSubPath(f->pos);
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
    return normalizeCoordinates(fingers[index]->pos);
}

Path Gesture::getPath(int index)
{
    return fingers[index]->path;
}

void Gesture::updateFingers(const MouseInputSource& mis, int index)
{
        for (int i = 0; i < fingers.size(); i++)
        {
            if(fingers[i]->sourceIndex == index) // checks whether the stored input source exists or not
            {
                fingers[i]->prevPos = fingers[i]->pos;
                fingers[i]->pos = mis.getScreenPosition();

                Path newSegment;
                newSegment.startNewSubPath(fingers[i]->prevPos); // start of new segment
                newSegment.lineTo(mis.getScreenPosition()); // end of new segment

                fingers[i]->path.addPath(newSegment);
            }
        }
}

int Gesture::getNumFingers()
{
    return fingers.size();
}

void Gesture::drawPath(Graphics& g, Path p)
{
    //while(Path::Iterator(p).next())
    //{
        Point<float> prevPos = Point<float>(Path::Iterator(p).x1, Path::Iterator(p).y1);
        Path::Iterator(p).next();
        Point<float> nextPos = Point<float>(Path::Iterator(p).x1, Path::Iterator(p).y1);

        Path pathRender;
        pathRender.startNewSubPath(prevPos);
        pathRender.lineTo(nextPos);
        
        //PathStrokeType(getVelocity()*30-30, PathStrokeType::beveled, PathStrokeType::rounded).createStrokedPath(pathRender, pathRender); // velocity mapped to new segment thickness
        g.strokePath(p, PathStrokeType(std::pow(getVelocity()+1,2)*30-30));
    //}
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

void Gesture::setVelocity(float vel)
{
    dist = vel;
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
    } 
}

String Gesture::getDirection()
{
    return direction;
}

void Gesture::setTap(float p [2][2])
{
    tapDist = std::sqrt(std::pow(p[0][0]-p[1][0],2)+std::pow(p[0][1]-p[1][1],2));
    
    if (tapDist == 0)
        isTap = true;
    else
        isTap = false;
}

bool Gesture::tap()
{
    if(isTap)
        return true;
    else
        return false;
}

void Gesture::setAbsDistFromOrigin(float x, float y)
{
    absDistFromOrigin = std::sqrt(std::pow(0.5-x,2)+std::pow(0.5-y,2));
}

float Gesture::getAbsDistFromOrigin()
{
   return absDistFromOrigin;   
}

float Gesture::getDiscretePitch()
{
    return 2 * floor ((Gesture::getFingerPosition(Gesture::getNumFingers()-1).x * 12) - 6);
}

float Gesture::getVelocity()
{
    return dist*2;//std::pow(dist+1,4) ;
}

float Gesture::getVelocityX()
{
    return std::pow(xDelta+1,4);
}

float Gesture::getVelocityY()
{
    return std::pow(yDelta+1,4);
}

void Gesture::setVelocityMax(float vel)
{
    velMax = vel;
}

float Gesture::getVelocityMax()
{
    return velMax;
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
