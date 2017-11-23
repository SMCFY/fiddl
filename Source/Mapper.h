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

enum GestureParameter {X_POSITION, Y_POSITION, ABS_DIST, d};
enum AudioParameter {GAIN, PITCH, DISCRETE_PITCH, TEMPO, HIGHPASS, LOWPASS, BANDPASS};

class Mapper
{
	public:
		static void routeParameters(int numFingers, bool isInPitchBar); // wrapper mapping functions
		static void updateParameters(); // update mappings when gesture values are updated
		
        static float inMax;
        static float inMin;
    
        static void setToggleSpace(int id);
        static int getToggleSpaceID();
		
    private:		
		static std::vector< std::pair <GestureParameter, AudioParameter> > mapping;

        static void mapToGain(float val);
        static void mapToPitch(float val);
        static void mapToTempo(float val);
        static void mapToLowPass(float val);
        static void mapToHighPass(float val);
        static void mapToBandPass(float val);
        static void mapToDiscretePitch(float val);
    
        // this method takes a string pair to map a "gestureParameter" to an "audioParameter"
        static void mapFromTo(GestureParameter gestureParameter, AudioParameter audioParameter);
    
        static float lin2log(float outMax, float outMin, float input);
    
        static int toggleSpaceID;
};
