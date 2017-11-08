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
#include "SoundTouch.h"

using namespace soundtouch;

class TimeStretch : public DSP
{
	public:
		TimeStretch(AudioParameterFloat *pitch, AudioParameterFloat *tempo);
		~TimeStretch();

		void process(AudioBuffer<float> buffer) override;

	private:
	
	    SoundTouch soundTouch;
        int nSamples;
        AudioParameterFloat* pitch;
        AudioParameterFloat* tempo;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeStretch);
};
