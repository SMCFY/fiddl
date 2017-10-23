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
		double getXPos();
		double getYPos();
		static void setXPos(double x);
		static void setYPos(double y);

	private:
		static double xPos;
		static double yPos;
};
