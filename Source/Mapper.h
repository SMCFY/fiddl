/*
  ==============================================================================

    Mapper.h
    Created: 23 Oct 2017 11:56:31pm
    Author:  geri

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Mapper
{
	public:
		static void routeParameters(); // wrapper mapping functions
		static void mapToGain(float val);

	private:
		static AudioParameterFloat *gainLevel;
};