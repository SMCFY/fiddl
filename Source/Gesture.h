/*
  ==============================================================================

    Gesture.h
    Created: 23 Oct 2017 1:11:42pm
    Author:  geri

  ==============================================================================
*/

#pragma once

class Gesture
{
	public:
		static float getXPos();
		static float getYPos();
		static void setXPos(float x);
		static void setYPos(float y);
	
	private:
		static float xPos;
		static float yPos;
};
