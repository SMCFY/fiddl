/*
  ==============================================================================

    Gesture.h
    Created: 23 Oct 2017 1:11:42pm
    Author:  geri

    Description:  Receives mouse coordinates from the PlayComponent and calculates
    			  gesture parameters. 

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Gesture
{
	public:
        typedef struct Position { Point<float> pos; const MouseInputSource* mis; int sourceIndex;
                                    Position(const MouseInputSource& mouseInput, Point<float> point)
                                    {
                                        this->mis = &mouseInput;
                                        this->pos = point;
                                        this->sourceIndex = mouseInput.getIndex();
                                    }
                                } Position;
    
		static void setVelocity(float x, float y);
		static void setDirection(float p [][2]);
        static String getDirection();
        static float getVelocityX();
        static float getVelocityY();
        static float getVelocity();
    
        static void fillDirBuff(float x, float y);

        // multi touch
        static void addFinger(const MouseEvent& e); // adds new finger struct
        static void rmFinger(const MouseEvent& e); // removes finger 
        static Position* getFingerPosition(int index); // returns finger by its array index
        static void updateFingers(const MouseInputSource& mis, int index); // update finger coordinates

        static void setResetPos(bool reset);
        static bool getResetPos();

        static void setCompWidth(float w);
        static void setCompHeight(float h);
    
        static bool resetPos;
        static int directionBuffSize;
    
        static OwnedArray<Position> fingers; // stores the finger locations

	private:
        static Point<float> normalizeCoordinates(Point<float> p);

        static float compWidth;
        static float compHeight;

		static float xNew;
		static float xTemp;
        static float xDelta;
    
        static float yNew;
        static float yTemp;
        static float yDelta;
    
        static float dist;
    
        static float directionDeltaX;
        static float directionDeltaY;
        static float absDirectionDeltaX;
        static float absDirectionDeltaY;
        static String direction;
};
