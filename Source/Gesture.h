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
		static float getXPos();
		static float getYPos();
		static void setXPos(float x);
		static void setYPos(float y);
    
		static void setVelocity(float x, float y);
		static void getDirection(float p [20][2]);
        static float getVelocityX();
        static float getVelocityY();
        static float getVelocity();
    
        static void fillDirBuff(float x, float y);
    
	private: 
		static float xPos;
		static float yPos;
		
		static float xNew;
		static float xTemp;
        static float xDelta;
    
        static float yNew;
        static float yTemp;
        static float yDelta;
    
        static float dist;
    
        static float points [20][2];
        static float directionHyp;
        static float directionH;
        static float directionAngle;
};
