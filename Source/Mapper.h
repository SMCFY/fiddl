/*
  ==============================================================================

    Mapper.h
    Created: 23 Oct 2017 11:56:31pm
    Author:  geri

    Description:  This class holds the AudioParameter objects, routes the gesture
    			  parameters to audio parameters and updates them. 

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Mapper
{
	public:
		static void routeParameters(int map); // wrapper mapping functions
		static void updateParameters(); // update mappings when gesture values are updated
		
        static float inMax;
        static float inMin;
    
        static void setToggleSpace(int t);
		
		private:		
		static std::vector< std::pair <std::string,std::string> > mapping;

        static void mapToGain(float val);
        static void mapToPitch(float val);
        static void mapToTempo(float val);
        static void mapToLowPass(float val);
    
        // this method takes a string pair to map a "gestureParameter" to an "audioParameter"
        static void mapFromTo(std::string gestureParameter, std::string audioParameter);
    
        static float lin2log(float outMax, float outMin, float input);
    
        static int toggleSpaceVal;
};
