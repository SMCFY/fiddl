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


float Gesture::diatonic [] = {-5.0f, -3.0f, -2.0f, 0.0f, 2.0f, 4.0f, 6.0f, 7.0f};
float Gesture::chromatic [] = {-4.0f, -3.0f, -2.0f, -1.0f, 0.0f, 1.0f, 2.0f, 3.0f};
float Gesture::pentatonic [] = {-7.0f, -5.0f, -3.0f, 0.0f, 2.0f, 5.0f, 7.0f, 9.0f};
float Gesture::minorPenta [] = {-7.0f, -4.0f, -2.0f, 0.0f, 3.0f, 5.0f, 8.0f, 10.0f};
float Gesture::phrygian [] = {-6.0f, -5.0f, -2.0f, -1.0f, 1.0f, 2.0f, 5.0f, 6.0f};

float Gesture::discretePitchScale [8]; //array size = 8
float Gesture::discretePitchVal;
int Gesture::pitchIndex;

float Gesture::spectralCentroid = 1000;

float Gesture::distBetweenFingers;
Point<float> Gesture::pinchP1;
Point<float> Gesture::pinchP2;

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
    f->totalPathLength = 0;
    f->pathAlpha = 1.0f;
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

Point<float> Gesture::getFingerPositionScreen(int index)
{
    return fingers[index]->pos;
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

                //if (fingers[i]->totalPathLength < 100) {
                    Path newSegment;
                    newSegment.startNewSubPath(fingers[i]->prevPos); // start of new segment
                    newSegment.lineTo(mis.getScreenPosition()); // end of new segment

                    fingers[i]->path.addPath(newSegment);
                    fingers[i]->totalPathLength++;
                    fingers[i]->pathAlpha *= 0.9;
               // }
            }
        }
}

int Gesture::getNumFingers()
{
    return fingers.size();
}

int Gesture::getSourceIndex(int index)
{
    return fingers[index]->sourceIndex;
}

void Gesture::drawPath(Graphics& g, Path p, int i) // reconstruct the stored path for each finger with updated appearance
{
    Path trail = p; // copy stored path
    p.clear(); // clear stored path
    
    Path::Iterator it(trail); 
    p.startNewSubPath(Point<float>(it.x1, it.y1)); // reinit the stored path

    Point<float> prevPos;
    Point<float> nextPos;

    int segmentNr = 0; // segment number

    while(it.next()) // iterate through the copy of the stored path
    {
        prevPos = nextPos;
        nextPos = Point<float>(it.x1, it.y1);

        if (fingers[i]->totalPathLength < 100) { 
            Path pathRender; // create a new segment connecting the extracted positions
            pathRender.startNewSubPath(prevPos);
            pathRender.lineTo(nextPos);
            
                PathStrokeType(segmentNr+1 - fingers[i]->totalPathLength, PathStrokeType::beveled, PathStrokeType::rounded).createStrokedPath(pathRender, pathRender);

            p.addPath(pathRender); // add segment to reconstructed path
            
            segmentNr++;
        }
    }
    g.setOpacity(fingers[i]->pathAlpha);
    g.fillPath(p);
        
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
    //Set scale here: 1 = Diatonic  2 = Pentatonic  3 = Minor Pentatonic  4 = Phrygian  5 = Chromatic
    setScale(2);
    
    if(floor ((Gesture::getFingerPosition(Gesture::getNumFingers()-1).y * 8)) < 8){
    pitchIndex = floor ((Gesture::getFingerPosition(Gesture::getNumFingers()-1).y * 8));
    }
    else
    {
        pitchIndex = 7;
    }
    
    discretePitchVal = discretePitchScale[pitchIndex];
    
    return discretePitchVal;
}

int Gesture::getPitchIndex()
{
    return pitchIndex;
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

void Gesture::setCentroid(float C)
{
    spectralCentroid = C;
}

float Gesture::getCentroid()
{
    return spectralCentroid;
}

void Gesture::setDistBetweenFingers(int i)
{
    pinchP1 = normalizeCoordinates(fingers[0]->pos);
    pinchP2 = normalizeCoordinates(fingers[i]->pos);
    
    distBetweenFingers = (std::sqrt(std::pow(pinchP2.x-pinchP1.x,2)+std::pow(pinchP2.y-pinchP1.y,2)));//;-0.1;
}

void Gesture::resetDistBetweenFingers()
{
    distBetweenFingers = 0.0f;
}

float Gesture::getDistBetweenFingers()
{
    return distBetweenFingers;
}

void Gesture::setScale(int index)
{
    switch(index){
        case 1: //Diatonic scale
            for(int i = 0; i < 8; i++)
            {
                discretePitchScale[i] = diatonic[i];
            }
            break;
        case 2: //Pentatonic scale
            for(int i = 0; i < 8; i++)
            {
                discretePitchScale[i] = pentatonic[i];
            }
            break;
        case 3: //Minor Pentatonic scale
            for(int i = 0; i < 8; i++)
            {
                discretePitchScale[i] = minorPenta[i];
            }
            break;
        case 4: //Phrygian scale
            for(int i = 0; i < 8; i++)
            {
                discretePitchScale[i] = phrygian[i];
            }
            break;
        case 5: //Chromatic scale
            for(int i = 0; i < 8; i++)
            {
                discretePitchScale[i] = chromatic[i];
            }
            break;
    }
}
