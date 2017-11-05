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
		TimeStretch();
		~TimeStretch();

		void process(AudioBuffer<float> buffer) override;

	private:
	
	    SoundTouch soundTouch;
        int nSamples;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeStretch);
};
