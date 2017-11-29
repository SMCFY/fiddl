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

enum GestureParameter {X_POSITION, Y_POSITION, ABS_DIST, VELOCITY, VELOCITY_MAX};
enum AudioParameter {GAIN, PITCH, DISCRETE_PITCH, TEMPO, HIGHPASS_CUTOFF, HIGHPASS_Q, LOWPASS_CUTOFF, LOWPASS_Q, BANDPASS_CUTOFF, BANDPASS_Q, RELEASE,SUSTAINED_RELEASE};

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
        static void mapToLowPassCutoff(float val);
        static void mapToLowPassQ(float val);
        static void mapToHighPassCutoff(float val);
        static void mapToHighPassQ(float val);
        static void mapToBandPassCutoff(float val);
        static void mapToBandPassQ(float val);
        static void mapToDiscretePitch(float val);
        static void mapToRelease(float val);
        static void mapToSustainedRelease(float val);
    
        // this method takes a string pair to map a "gestureParameter" to an "audioParameter"
        static void mapFromTo(GestureParameter gestureParameter, AudioParameter audioParameter);
    
        static float lin2log(float outMax, float outMin, float input);
    
        static int toggleSpaceID;
};
