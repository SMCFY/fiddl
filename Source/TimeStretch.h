/*
  ==============================================================================

    TimeStretch.h
    Created: 20 Oct 2017 3:13:32pm
    Authors: Michael Castanieto
             Gergely Csapo
             Jonas Holfelt

    Description: 
  ==============================================================================
*/

#pragma once

#include "DSP.h"

class TimeStretch : public DSP
{
	public:
		TimeStretch();
		~TimeStretch();

		void process(AudioBuffer<float> buffer) override;

	private:
		/* parameter variables */
		/* look at the AudioProcessorParameter class documentation
		   for more info on how to use these parameters */
		AudioParameterFloat* parameter1;
		AudioParameterFloat* parameter2;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeStretch);
};